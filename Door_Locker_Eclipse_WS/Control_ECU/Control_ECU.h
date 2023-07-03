/******************************************************************************
 *
 * Module: 	Micro-controller 2
 *
 * File Name: MC2.h
 *
 * Description: Header file for the MC2 application code
 *
 * Author: Hana Khater
 *
 *******************************************************************************/


#ifndef MC2_H_
#define MC2_H_
#include "std_types.h"
#include "dc_motor.h"
#include "uart.h"
#include "timer0.h"
#include "twi.h"
#include "external_eeprom.h"
#include "buzzer.h"
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
/* Address of the first byte in EEPROM */
#define EEPROM_ADDRESS   0x0311


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*Description :
 * 1. To receive the new password using UART driver
 * 2. Store the password in the EEPROM using I2C
 */

void MC2_recieveStorePass(void);

/*Description :
 * 1. Check that the entered password in case of choosing (open gate) or (change pass) is correct
 * 2. If the password is correct it sends a flag using UART to open the gate / change password
 * 3. If the password is wrong it sends a flag (FALSE) to MC1
 */
void MC2_checkTheRecievedPass(void);

/*Description :
 * 1. It is called when MC1 sends a flag = (OPEN_GATE) using UART to MC2
 * 2. Once it is called it rotates the DC motor CLOCKWISE for 15 seconds
 *    Stop it for 3 seconds and finally rotates it ANTICLOCKWISE for 15 seconds
 *    using both DC_motor & Timer0 drivers
 */

void MC2_openAndCloseGate(void);

/*Description :
 * 1. It is called when MC1 sends a flag = (BUZZER) using UART to MC2
 * 2. Once it is called it opens the buzzer for 1 minute using both buzzer & Timer0 drivers
 */

void MC2_TriggerBuzzer(void);

/* Description :
 * 1. We store the address of this function in ISR of Timer0 so it increments a global counter (g_counter)
 *    each time there is an interrupt
 * 2. this function does'nt stop incrementing until we turn off Timer0 interrupts
 * 3. we turn off Timer0 interrupts when we reach the time needed and that depends on the function
 *    that opens Timer0 interrupts
 */

void MC2_timer0Counter(void);


#endif /* MC2_H_ */
