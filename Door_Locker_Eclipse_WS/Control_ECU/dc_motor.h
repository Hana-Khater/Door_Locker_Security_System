/******************************************************************************
 *
 * Module: 	DC MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for DC MOTOR DRIVER
 *
 * Author: Hana Khater
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_PORT     PORTD_ID
#define DC_PIN_A    PIN2_ID
#define DC_PIN_B    PIN3_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	STOP , CLOCKWISE , ANTI_CLOCKWISE
} DcMotor_State ;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * Responsible to setup the direction for the two motor pins through GPIO driver
 * Stop DC motor at the beginning
 */

void DcMotor_init (void);

/* Description :
 * 1. Responsible to rotate the motor clock-wise or anti clock-wise or stop the motor
 * based on the state input state value
 * 2. Send the required duty cycle to the PWM driver based on the required speed value
 */

void DcMotor_rotate(DcMotor_State state);

#endif /* DC_MOTOR_H_ */
