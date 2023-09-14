#include "general.h"

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
========================  INT0 INTERRUPT  ===========================================================================================================
=====================================================================================================================================================
*/

void int1Interrupt(void){
    SREG &= ~0x80;
    EICRA |= 0x02;  //INTERRUPT 0, FALLING EDGE
    EIMSK |= 0x01;  //EXTERNAL PIN INTERRUPT ENABLED IN0 (PORTD 2)
    EIFR &= 0x02;   //CLEAR INT0 FLAG
    SREG |= 0x80;   //Global Interrupt ENA
}

void int1InterruptOff(void){
    EIFR &= 0x01;
    EIMSK &= ~0x02;
}

void int1InterruptOn(void){
    EIMSK |= 0x02;
    EIFR &= 0x01;
}

/*
=====================================================================================================================================================
========================  INIT PORT  ================================================================================================================
=====================================================================================================================================================
*/

void initPorts(void){
    DDRB = 0xFF;  
    DDRD = 0x01;
    PORTD |= 0x04;         
}

