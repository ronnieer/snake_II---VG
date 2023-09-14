#include "general.h"



unsigned char setSnakeLength = 10;      //SET SNAKE LENGTH TO ADJUST LEVEL OF DIDDICULTY. A LENGTH BETWEEN 5 AND 15 WILL GIVE A PLESSANT GAME



long currentValue = 0;
unsigned char snakeLength = 2;
bool isBreakMessageFlag = false;
GameStatus statusOfGame;
JoyStick joyStickPos;

void clearMax7219(void);

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
            for(unsigned char i = 0; i < 2; i++) printText("WIN");
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
            for(unsigned char i = 0; i < 2; i++) printText("END");
            statusOfGame = PRE_GAME;
        }        
	}

	return 0;
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
------------------------  CLEAR END OF SNAKE'S TAIL  ------------------------------------------------------------------------------------------------
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
========================  INTERRUPT ROTINE  =========================================================================================================
=====================================================================================================================================================
*/

ISR(INT0_vect){
    if(statusOfGame == PRE_GAME){
        clearMax7219();
        isBreakMessageFlag = true;
    } 
    statusOfGame = RUN_GAME;
    snakeLength = 2;
    EIFR &= 0x01;
}
