/******************************************************************************
 *
 * Module: 	DC MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for DC MOTOR DRIVER
 *
 * Author: Hana Khater
 *
 *******************************************************************************/
#include "dc_motor.h"
#include "gpio.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/* Description :
 * 1. Responsible to setup the direction for the two motor pins through GPIO driver
 * 2. Stop DC motor at the beginning through GPIO driver
 */

void DcMotor_init(void)
{
  GPIO_setupPinDirection(DC_PORT ,DC_PIN_A , PIN_OUTPUT);
  GPIO_setupPinDirection(DC_PORT ,DC_PIN_B , PIN_OUTPUT);
  GPIO_writePin(DC_PORT ,DC_PIN_A , LOGIC_LOW);
  GPIO_writePin(DC_PORT ,DC_PIN_B , LOGIC_LOW);
}

/* Description :
 * 1. Responsible to rotate the motor clock-wise or anti clock-wise or stop the motor
 * based on the state input state value
 * 2. Send the required duty cycle to the PWM driver based on the required speed value
 */

void DcMotor_rotate(DcMotor_State state)
{
  switch(state)
  {
  case STOP :
	  GPIO_writePin(DC_PORT ,DC_PIN_A , LOGIC_LOW);
	  GPIO_writePin(DC_PORT ,DC_PIN_B , LOGIC_LOW);
	  break;
  case CLOCKWISE :
  	  GPIO_writePin(DC_PORT ,DC_PIN_A , LOGIC_HIGH);
  	  GPIO_writePin(DC_PORT ,DC_PIN_B , LOGIC_LOW);
  	  break;
  case ANTI_CLOCKWISE :
  	  GPIO_writePin(DC_PORT ,DC_PIN_A , LOGIC_LOW);
  	  GPIO_writePin(DC_PORT ,DC_PIN_B , LOGIC_HIGH);
  	  break;
  }

}
