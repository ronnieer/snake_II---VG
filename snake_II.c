#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "uart.h"
#include "max72xx.h"
#include "millis.h"
#include "characters.h"


#define	SCL 5
#define	SDA 3
#define	CS 2

test = 0x0F;
const brightness = 0x0A;
const scanlimit = 0x0B;
const decodeMode = 0x09;
const shutdown = 0x0C;

/*
=====================================================================================================================================================
========================  SNAKE'S BODY STRUCTURE DECLARATION  =======================================================================================
=====================================================================================================================================================
*/

typedef struct{
    unsigned char x;
    unsigned char y;
}snakePosition;

typedef enum{
    PRE_GAME,
    END_GAME,
    RUN_GAME,
    WIN_GAME
}GameStatus;

GameStatus gameStatus;

/*
=====================================================================================================================================================
========================  FUNCTIONS DECLARATION  ====================================================================================================
=====================================================================================================================================================
*/

unsigned char snakeLength = 2;
unsigned char horisontal = 128;
unsigned char vertical = 128;
bool isBreakMessageFlag = false;

void initPorts(void);
void int1Interrupt(void);
void snakesRandomFood(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten);
void readJoystick(void);
void printText(char *string[40]);
void clearMax7219(void);
void enterpretJoystick(snakePosition snakePositions[128]);
void snakeGraphics(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten);
void setupADC(void);

/*
=====================================================================================================================================================
========================  SNAKE II GAME MAIN FUNCTION  ==============================================================================================
=====================================================================================================================================================
*/


int main(){

    
    snakePosition snakePositions[128];
    initPorts();
    int1Interrupt();
    sei();  
	init_serial();
	max7219_init();
    int cnt = 0;
    // for(int cnt = 0; cnt < 128; cnt++){
    //     snakePositions[cnt].x = 200;
    //     snakePositions[cnt].y = 200;
    // }
    
    // snakePositions[0].x = 0;
    // snakePositions[0].y = 0;
    // unsigned char *isJoyStickUpdate = 1;
    // unsigned char randomFoodX = 16;
    // unsigned char randomFoodY = 8;
    // int8_t *isRandomFoodEaten = 1;
    gameStatus = PRE_GAME;
	while(true){

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  PRE GAME MESSAGE, WAITING FOR GAME START  ---------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        while(gameStatus == PRE_GAME){
            printText(" GAME");
        }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  RUN GAME  -----------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        for(int cnt = 0; cnt < 128; cnt++){     // SET SNAKE ARRAY FOR START (SNAKE BODY PARTS)
            snakePositions[cnt].x = 200;
            snakePositions[cnt].y = 200;
        }    

        snakePositions[0].x = 0;
        snakePositions[0].y = 0;
        unsigned char *isJoyStickUpdate = 1;
        unsigned char randomFoodX = 16;
        unsigned char randomFoodY = 8;
        int8_t *isRandomFoodEaten = 1;        

        while(true){
            snakeGraphics(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);
            if(gameStatus == END_GAME || gameStatus == WIN_GAME) break;
            snakesRandomFood(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);
            max7219b_out();      
            _delay_ms(700);
            readJoystick();
            enterpretJoystick(snakePositions);
        }
        isBreakMessageFlag = false;
        unsigned char counter = 0;

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  GAME WON, SCROLL WIN ONES  ------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        while(gameStatus == WIN_GAME){// && !isBreakMessageFlag){
            printText(" WIN");
            gameStatus = PRE_GAME;
        }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  GAME LOST, SCROLL END ONES  ------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        isBreakMessageFlag = false;
        counter = 0;
        while(gameStatus == END_GAME){ // && !isBreakMessageFlag){
            printText(" END");
            gameStatus = PRE_GAME;
        }        
	}

	return 0;
}


/*
=====================================================================================================================================================
========================  INT0 INTERRUPT ENA  =======================================================================================================
=====================================================================================================================================================
*/

void int1Interrupt(void){
    SREG &= ~0x80;
    // MCUCR |= 0x01;
    // MCUCR |= 0x02;
    EICRA |= 0x0C;  //INTERRUPT 1, FALLING EDGE
    EIMSK |= 0x02;  //EXTERNAL PIN INTERRUPT ENABLED
    PCICR |= 0X02;  //PIN CHANGE ITERRUPT CONTROL REGISTER
    PCIFR |= 0x02;
    PCMSK1 |= 0x04; //PORTD BIT 5 = INT1
    EIFR = 0x00;
    SREG |= 0x80;   //Global Interrupt ENA
}

/*
=====================================================================================================================================================
========================  TEXT SCROLLERS  ============================================================================================================
=====================================================================================================================================================
*/

void printText(char *string[40]){
    char textGame[40];
    char textScrollbox[40][8];
    //strcpy(textGame, *string);

    for(uint8_t charPos = 0; charPos < 40; charPos++){
        for(uint8_t charLineElement = 0; charLineElement < 8; charLineElement++){
            char character = textGame[charPos];
            textScrollbox[charPos][charLineElement] = 0x00;
        }
    }    


    unsigned char posCnt = 0;
    unsigned char posOffset = 0;
    unsigned char strLength = strlen(string);
    bool isStartTerm = true;
    EIFR &= 0x01;
    while(true){
        if(isStartTerm == true || posOffset >= 8 * ((strlen(string)) + 1)){
            
            isStartTerm = false;
            posOffset = 0;
            strcpy(textGame, string);
            

            for(uint8_t charPos = 0; charPos < strlen(textGame); charPos++){
                for(uint8_t charLineElement = 0; charLineElement < 8; charLineElement++){
                    char character = textGame[charPos];
                    textScrollbox[charPos][charLineElement] = myCharacters[character][charLineElement];           
                }
            }
        }
        
        for(uint8_t charPos = 0; charPos < strlen(textGame); charPos++){
            for(uint8_t charLineElement = 0; charLineElement < 8; charLineElement++){
                for(uint8_t charLinePos = 0; charLinePos < 32; charLinePos++){
                    if(((textScrollbox[charPos][charLineElement] >> charLinePos) & 0x01) == 0x01){
                        max7219b_set(charLinePos + charPos * 8 - posOffset + 8, charLineElement);
                        max7219b_out();
                    } 
                    else{ 
                        max7219b_clr(charLinePos + charPos * 8 - posOffset + 8, charLineElement);
                        max7219b_out();
                    } 
                }
            }
        }
        clearMax7219();
        posOffset++;
        if(isBreakMessageFlag == true){
            isBreakMessageFlag = false;
            break;
        }
        if(posOffset >= 8 * (strLength + 1)) break;
    }

}

void clearMax7219(void){
    for(unsigned char y = 0; y < 8; y++){
        for(unsigned x = 0; x < 16; x++){
            max7219b_clr(x, y);
            max7219b_out();
        }
    }
}


/*
=====================================================================================================================================================
========================  JOYSTICK MOVEMENTS  =======================================================================================================
=====================================================================================================================================================
*/

void enterpretJoystick(snakePosition snakePositions[128]){

    if((horisontal > 192) && (snakePositions[0].x < 15)){
        snakePositions[0].x++;
    } 

    if((horisontal < 64) && (snakePositions[0].x >= 1)){
        snakePositions[0].x--;
    }

    if((vertical > 192) && (snakePositions[0].y < 7)){
        snakePositions[0].y++;
    }

    if((vertical < 64) && (snakePositions[0].y >= 1)){
        snakePositions[0].y--;
    }    
}

/*
=====================================================================================================================================================
========================  SNAKE BODY GRAPHICS  ======================================================================================================
=====================================================================================================================================================
*/

void snakeGraphics(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten){

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  SET SNAKE HEAD AND COPY BODY SEGMENTS BY ITERATION  -----------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    unsigned char i;
    max7219b_set(snakePositions[0].x, snakePositions[0].y);
    max7219b_out();  
    for(i = 0; i < snakeLength; i++){
        snakePositions[snakeLength - i] = snakePositions[snakeLength - i - 1];       
    }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  ADD SEGMENTS WHEN EATING  -------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    if((snakePositions[0].x == *randomFoodX) && (snakePositions[0].y == *randomFoodY) && (*isRandomFoodEaten == 0)){
        snakeLength++;
        if(snakeLength > 5){
            clearMax7219();
            gameStatus = WIN_GAME;
            return;
        }       
        max7219b_set(snakePositions[0].x, snakePositions[0].y);
        max7219b_out();                  
        *isRandomFoodEaten = 1; 
    }


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  SET END OF SNAKE'S TAIL  --------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    max7219b_clr(snakePositions[snakeLength].x, snakePositions[snakeLength].y);
    max7219b_out();     

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  CHECK FOR HEAD NOT CROSSING BODY  -----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/    

    for(unsigned char snakeBodyIndex = 2; snakeBodyIndex < snakeLength; snakeBodyIndex++){
        if((snakePositions[snakeBodyIndex].x == snakePositions[0].x) && (snakePositions[snakeBodyIndex].y == snakePositions[0].y)){
            clearMax7219();
            gameStatus = END_GAME;
            return;
        }
    }
}

/*
=====================================================================================================================================================
========================  SNAKE FOOD GRAPHICS  ======================================================================================================
=====================================================================================================================================================
*/

void snakesRandomFood(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten){
    if(*isRandomFoodEaten){   
        bool isFoodIsAtSnake = true;
        unsigned char cnt;
        while(true){
            *randomFoodX = rand() % 16;
            *randomFoodY = rand() % 8;
            for(cnt = 0; cnt < snakeLength; cnt++){
                if((snakePositions[cnt].x == *randomFoodX) && (snakePositions[cnt].y == *randomFoodY)) 
                break;
            }
            if(cnt == snakeLength) break;
        }
        max7219b_set(*randomFoodX, *randomFoodY);
        max7219b_out();
        _delay_ms(70);
        max7219b_clr(*randomFoodX, *randomFoodY);
        max7219b_out();        
        _delay_ms(70);
        max7219b_set(*randomFoodX, *randomFoodY);
        max7219b_out();     
        *isRandomFoodEaten = 0;
    }
}

/*
=====================================================================================================================================================
========================  INIT PORT  ================================================================================================================
=====================================================================================================================================================
*/

void initPorts(void)
{
    DDRB = 0xFF;  
    DDRD = 0x01;
    PORTD |= 0x08;         
}

/*
=====================================================================================================================================================
========================  READ JOYSTICK  ============================================================================================================
=====================================================================================================================================================
*/

void readJoystick(void)
{
    unsigned char horisontalLocal, vericalLocal;
    ADCSRA = 0xE7;                                      // ADEN = 1, ADC ENABLED// ADSC = 1, ADC START CONVESION// ADATE = 1, ADC AUTO TRIGGER ENABLED// ADIF = 0, ADC INTERRUPT FLAG//
                                                        // ADIE = 0, ADC INTERRUPT DISABLED// ADPS0:2 = 1, ADC PRESCALER DIVISION FACTOR = 128  
    ADMUX = 0x00;
    ADMUX |= 1 << 5;                                    // ADC OUTPUT REGISTER LEFT ADJUSTED
    ADMUX |= 1 << 6;                                    // ADC HIGHREF VCC
  
    while(true){ 
        if((ADCSRA & 0x10) == 0x10){
            horisontalLocal = ADCH; // RETURN RESULT WHEN INTERUPT FLAG
            break;
        }
    }                                                    
    ADCSRA = 0xE7;                                      // ADEN = 1, ADC ENABLED// ADSC = 1, ADC START CONVESION// ADATE = 1, ADC AUTO TRIGGER ENABLED// ADIF = 0, ADC INTERRUPT FLAG//
               
    _delay_ms(1);                                       // ADIE = 0, ADC INTERRUPT DISABLED// ADPS0:2 = 1, ADC PRESCALER DIVISION FACTOR = 128  
    ADMUX = 0x01;
    ADMUX |= 1 << 5;                                    // ADC OUTPUT REGISTER LEFT ADJUSTED
    ADMUX |= 1 << 6;                                    // ADC HIGHREF VCC

    while(true){ 
        if((ADCSRA & 0x10) == 0x10){
            vericalLocal = ADCH;   // RETURN RESULT WHEN INTERUPT FLAG
            break;
        }
    }
    //if(horisontalLocal >= 64 && horisontal <= 192){} 
    //else if(vericalLocal >= 64 && vertical <= 192){} 
    if(horisontalLocal < 64 && (vericalLocal >= 64 && vericalLocal <= 192)){
        horisontal = 0;
        vertical = 128;
    } 
    else if(horisontalLocal > 192 && (vericalLocal >= 64 && vericalLocal <= 192)){
        horisontal = 255;
        vertical = 128;
    }
    else if(vericalLocal < 64 && (horisontalLocal >= 64 && horisontalLocal <= 192)){
        horisontal = 128;
        vertical = 0;
    }
    else if(vericalLocal > 192 && (horisontalLocal >= 64 && horisontalLocal <= 192)){
        horisontal = 128;
        vertical = 255;
    }
}

/*
=====================================================================================================================================================
========================  INTERRUPT ROTINE  =========================================================================================================
=====================================================================================================================================================
*/

ISR(INT1_vect){
    clearMax7219();
    if(gameStatus == PRE_GAME) isBreakMessageFlag = true;
    gameStatus = RUN_GAME;
    snakeLength = 2;
    EIFR &= 0x01;
}