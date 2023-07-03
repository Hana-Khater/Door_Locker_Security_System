/******************************************************************************
 *
 * Module: 	BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for BUZZER DRIVER
 *
 * Author: Hana Khater
 *
 *******************************************************************************/

#include "buzzer.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/* Description :
 * 1. Setup the direction of the pin that will trigger the buzzer using GPIO driver
 * 2. turn off the buzzer at the beginning
 */

void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID , BUZZER_PIN_ID , PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_LOW);
}

/* Description :
 * Turn on the buzzer by setting the output pin
 */
void Buzzer_On(void)
{
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_HIGH);
}

/* Description :
 * Turn off the buzzer by clearing the output pin
 */
void Buzzer_Off(void)
{
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_LOW);

}
