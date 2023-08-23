#ifndef __GENERAL_H
#define __GENERAL_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "uart.h"
#include "max72xx.h"
#include "millis.h"


#define	SCL 5
#define	SDA 3
#define	CS 2

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
========================  SNAKE'S GAME STATUS CYCLE ENUMERATION  ====================================================================================
=====================================================================================================================================================
*/

typedef enum{
    PRE_GAME,
    END_GAME,
    RUN_GAME,
    WIN_GAME
}GameStatus;

extern GameStatus statusOfGame;

/*
=====================================================================================================================================================
========================  SNAKE'S JOYSTICK POSITIONS ENUMERATION  ===================================================================================
=====================================================================================================================================================
*/

typedef enum{
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT
}JoyStick;

extern JoyStick joyStickPos;

/*
=====================================================================================================================================================
========================  FUNCTIONS DECLARATION  ====================================================================================================
=====================================================================================================================================================
*/

extern bool isBreakMessageFlag;
extern unsigned char snakeLength;

extern const unsigned char myCharacters[128][8];
void initPorts(void);
void int1Interrupt(void);
void int1InterruptOff(void);
void int1InterruptOn(void);
void snakesRandomFood(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten);
void readJoystick(void);
uint16_t randNumber(unsigned char range, unsigned char offset);
void printText(char *string[40]);
void clearMax7219(void);
void enterpretJoystick(snakePosition snakePositions[128]);
void snakeGraphics(snakePosition snakePositions[128], unsigned char *randomFoodX, unsigned char *randomFoodY, unsigned char *isRandomFoodEaten);
void setupADC(void);

void initPorts(void);

#endif