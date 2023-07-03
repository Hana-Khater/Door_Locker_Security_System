/******************************************************************************
 *
 * Module: 	BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for BUZZER DRIVER
 *
 * Author: Hana Khater
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID PORTC_ID
#define BUZZER_PIN_ID  PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * 1. Setup the direction of the pin that will trigger the buzzer using GPIO driver
 * 2. turn off the buzzer at the beginning
 */

void Buzzer_init(void);

/* Description :
 * Turn on the buzzer by setting the output pin
 */
void Buzzer_On(void);

/* Description :
 * Turn off the buzzer by clearing the output pin
 */
void Buzzer_Off(void);

#endif /* BUZZER_H_ */
