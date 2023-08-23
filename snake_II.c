#include "general.h"

long currentValue = 0;
unsigned char setSnakeLength = 10;      //SET SNAKE LENGTH TO WIN
unsigned char snakeLength = 2;
bool isBreakMessageFlag = false;
GameStatus statusOfGame;
JoyStick joyStickPos;

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
    statusOfGame = PRE_GAME;
	while(true){


/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  PRE GAME MESSAGE, WAITING FOR GAME START  ---------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        while(statusOfGame == PRE_GAME){
            printText(" GAME");
        }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  RUN GAME  -----------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        for(int cnt = 0; cnt < 128; cnt++){     // SET SNAKE ARRAY FOR START (SNAKE BODY PARTS)
        snakePositions[cnt].x = 200;            // SET OUTSIDE SCREEN
            snakePositions[cnt].y = 200;
        }    

        snakePositions[0].x = rand() % 14 + 1;
        snakePositions[0].y = rand() % 6 + 1;
        joyStickPos = rand() % 4;
        unsigned char *isJoyStickUpdate = 1;
        unsigned char randomFoodX = 16;
        unsigned char randomFoodY = 8;
        int8_t *isRandomFoodEaten = 1;        

        while(true){
            snakeGraphics(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);
            if(statusOfGame == END_GAME || statusOfGame == WIN_GAME) break;
            snakesRandomFood(snakePositions, &randomFoodX, &randomFoodY, &isRandomFoodEaten);
            max7219b_out();      
            _delay_ms(350);
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

        while(statusOfGame == WIN_GAME){
            for(unsigned char i = 0; i < 2; i++) printText(" WIN");
            statusOfGame = PRE_GAME;
        }   

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  GAME LOST, SCROLL END ONES  ------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

        isBreakMessageFlag = false;
        counter = 0;
        while(statusOfGame == END_GAME){
            for(unsigned char i = 0; i < 2; i++) printText(" END");
            statusOfGame = PRE_GAME;
        }        
	}

	return 0;
}

/*
=====================================================================================================================================================
========================  JOYSTICK MOVEMENTS  =======================================================================================================
=====================================================================================================================================================
*/

void enterpretJoystick(snakePosition snakePositions[128]){

    if(joyStickPos == JOYSTICK_RIGHT){
        snakePositions[0].x++;
    } 

    if(joyStickPos == JOYSTICK_LEFT){
        snakePositions[0].x--;
    }

    if(joyStickPos == JOYSTICK_DOWN){
        snakePositions[0].y++;
    }

    if(joyStickPos == JOYSTICK_UP){
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
------------------------  SET SNAKE HEAD COPY TO BODY SEGMENTS ITERATION  ---------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    unsigned char i;
    max7219b_set(snakePositions[0].x, snakePositions[0].y); 
    for(i = 0; i < snakeLength; i++){
        snakePositions[snakeLength - i] = snakePositions[snakeLength - i - 1];       
    }
    max7219b_out(); 

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  ADD SEGMENTS WHEN EATING  -------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

    if((snakePositions[0].x == *randomFoodX) && (snakePositions[0].y == *randomFoodY) && (*isRandomFoodEaten == 0)){
        snakeLength++;
        if(snakeLength > setSnakeLength){
            clearMax7219();
            statusOfGame = WIN_GAME;
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
            statusOfGame = END_GAME;
            return;
        }
        else if((snakePositions[0].x == snakePositions[3].x) && (snakePositions[0].y == snakePositions[3].y)){
            clearMax7219();
            statusOfGame = END_GAME;
            return;            
        }
    }

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
------------------------  CHECK FOR HEAD NOT LEAVING SCREEN  ----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/  

    if(!(snakePositions[0].x >= 0 && snakePositions[0].x <= 15) || !(snakePositions[0].y >= 0 && snakePositions[0].y <= 7)){
        clearMax7219();
        statusOfGame = END_GAME;
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
            horisontalLocal = ADCH;                     // RETURN RESULT WHEN INTERUPT FLAG
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
            vericalLocal = ADCH;                        // RETURN RESULT WHEN INTERUPT FLAG
            break;
        }
    }

    if(horisontalLocal < 43 && (vericalLocal >= 43 && vericalLocal <= 213)) joyStickPos = JOYSTICK_LEFT;
    else if(horisontalLocal > 213 && (vericalLocal >= 43 && vericalLocal <= 213)) joyStickPos = JOYSTICK_RIGHT;
    else if(vericalLocal < 43 && (horisontalLocal >= 43 && horisontalLocal <= 213)) joyStickPos = JOYSTICK_UP;
    else if(vericalLocal > 213 && (horisontalLocal >= 43 && horisontalLocal <= 213)) joyStickPos = JOYSTICK_DOWN;
}

/*
=====================================================================================================================================================
========================  TEXT SCROLLERS  ============================================================================================================
=====================================================================================================================================================
*/

void printText(char *string[40]){
    char textGame[40];
    char textScrollbox[40][8];

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

    while(true){
        
        for(uint8_t charPos = 0; charPos < strlen(textGame); charPos++){
            for(uint8_t charLineElement = 0; charLineElement < 8; charLineElement++){
                for(uint8_t charLinePos = 0; charLinePos < 8; charLinePos++){
                    if(((textScrollbox[charPos][charLineElement] >> charLinePos) & 0x01) == 0x01){
                        max7219b_set(charLinePos + charPos * 8 - posOffset + 8, charLineElement);
                        //max7219b_out();
                    } 
                    else{ 
                        max7219b_clr(charLinePos + charPos * 8 - posOffset + 8, charLineElement);
                        //max7219b_out();
                    } 
                }
            }
        }
        max7219b_out();
        _delay_ms(100);
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
            //max7219b_out();
        }
    }
    max7219b_out();
}

/*
=====================================================================================================================================================
========================  INTERRUPT ROTINE  =========================================================================================================
=====================================================================================================================================================
*/

ISR(INT1_vect){
    if(statusOfGame == PRE_GAME){
        clearMax7219();
        isBreakMessageFlag = true;
    } 
    statusOfGame = RUN_GAME;
    snakeLength = 2;
    EIFR &= 0x01;
}