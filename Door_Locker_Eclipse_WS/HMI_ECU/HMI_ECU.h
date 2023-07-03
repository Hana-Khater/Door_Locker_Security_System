/******************************************************************************
 *
 * Module: 	Micro-controller 1
 *
 * File Name: MC1.h
 *
 * Description: Header file for the MC1 application code
 *
 * Author: Hana Khater
 *
 *******************************************************************************/

#ifndef MC1_H_
#define MC1_H_
#include "std_types.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer0.h"
#include <util/delay.h>

/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/

/* Numbers of digits in the password */
#define NUM_DIGITS 5
/* 'P' is a flag that we send to MC2 so it starts storing the new password */
#define NEW_PASS   'P'
/* 'C' is a flag that we send to MC2 so it checks that the password entered is correct */
#define CHECK      'C'
/* 'O' is a flag that we send to MC2 so it opens the gate */
#define OPEN_GATE  'O'
/* 'B' is a flag that we send to MC2 so it generate the buzzer */
#define BUZZER     'B'


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*  Description :
 *  Ask the user to enter his new password
 */

void MC1_askForPass (void);

/* Description :
 *  Check if there is a mismatch between the password and confirmation password
 */

uint8 MC1_matchingCheck(void);

/* Description :
 * Sends the new password MC2 so it would be stored in EEPROM
 */

void MC1_sendNewPassword(void);

/* Description :
 * Display main options : either to open the door or to change the password
 */

void MC1_mainOptions(void);

/* Description :
 * 1. We store the address of this function in ISR of Timer0 so it increments a global counter (g_counter)
 *    each time there is an interrupt
 * 2. this function does'nt stop incrementing until we turn off Timer0 interrupts
 * 3. we turn off Timer0 interrupts when we reach the time needed and that depends on the function
 *    that opens Timer0 interrupts
 */

void MC1_timer0Counter(void);

#endif /* MC1_H_ */
