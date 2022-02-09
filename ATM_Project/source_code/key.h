/*
 * key.h
 *
 *  Created on: Dec 19, 2021
 *      Author: user
 */

#ifndef KEY_H_
#define KEY_H_



#include "GPIO.h"

#define R0  0
#define R1  1
#define R2  2
#define R3  3

// Defining four columns of the Keypad
#define C0  4
#define C1  5
#define C2  6
#define C3  7

// Connect Keypad Data to PortD
#define KEYPAD_PORT_DATA_R        PORTA

// Connect Keypad DDR to DDRD
#define KEYPAD_PORT_DDR_R          DDRA

// Connect Keypad PIN TO PIND
#define KEYPAD_PORT_PIN_R          PINA


/* =============== Functionos Prototypes ============ */
void keypad_init(void);
char keypad_getchar(void);




#endif /* KEY_H_ */
