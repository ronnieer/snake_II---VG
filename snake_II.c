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

/*
=====================================================================================================================================================
========================  FUNCTIONS DECLARATION  ====================================================================================================
=====================================================================================================================================================
*/

unsigned char snakeLength = 2;
unsigned char horisontal = 128;
unsigned char vertical = 128;

void initPorts(void);
void snakesRandomFood(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten);
void readJoystick(void);
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
    sei();  
	init_serial();
	max7219_init();
    millis_init();
    

    //unsigned char horisontal = 128, vertical = 128;

    int cnt = 0;
    for(int cnt = 0; cnt < 128; cnt++){
        snakePositions[cnt].x = 200;
        snakePositions[cnt].y = 200;
    }
    
    snakePositions[0].x = 0;
    snakePositions[0].y = 0;
    unsigned char *isJoyStickUpdate = 1;
    unsigned char randomFoodX = 16;
    unsigned char randomFoodY = 8;
    int8_t *isRandomFoodEaten = 1;
	while(1){
        snakeGraphics(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);          
        snakesRandomFood(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);
        max7219b_out();      
        _delay_ms(1000);
        readJoystick();
        enterpretJoystick(snakePositions);
	}

	return 0;
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
        max7219b_set(snakePositions[0].x, snakePositions[0].y);
        max7219b_out();                  
        *isRandomFoodEaten = 1; 
    }
    for(unsigned char snakeBodyIndex = 2; snakeBodyIndex < snakeLength; snakeBodyIndex++){
        if((snakePositions[snakeBodyIndex].x == snakePositions[0].x) && (snakePositions[snakeBodyIndex].y == snakePositions[0].y)){
            for(unsigned char repeatIndex = 0; repeatIndex < 3; repeatIndex++)
            {
                for(unsigned char iEatMySelfIndex = 0; iEatMySelfIndex < snakeLength; iEatMySelfIndex++){
                    if(iEatMySelfIndex == 0){
                        for(uint8_t iEatMySelfIndexHead = 0; iEatMySelfIndexHead < 5; iEatMySelfIndexHead++){
                            max7219b_clr((snakePositions[iEatMySelfIndex].x), (snakePositions[iEatMySelfIndex].y));
                            max7219b_out();
                            _delay_ms(250);
                            max7219b_set((snakePositions[iEatMySelfIndex].x), (snakePositions[iEatMySelfIndex].y));
                            max7219b_out();
                            _delay_ms(250);
                        }
                    }
                    max7219b_clr((snakePositions[iEatMySelfIndex].x), (snakePositions[iEatMySelfIndex].y));
                    max7219b_out();
                    _delay_ms(50);
                    max7219b_set((snakePositions[iEatMySelfIndex].x), (snakePositions[iEatMySelfIndex].y));
                    max7219b_out();
                    _delay_ms(50);
                }
            }
            while(true){}
        }
    }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  SET END OF SNAKE'S TAIL  --------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    max7219b_clr(snakePositions[snakeLength].x, snakePositions[snakeLength].y);
    max7219b_out(); 
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