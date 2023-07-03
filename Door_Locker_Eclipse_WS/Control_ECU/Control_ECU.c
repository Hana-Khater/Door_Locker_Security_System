/******************************************************************************
 *
 * Module: 	Micro-controller 2
 *
 * File Name: MC2.c
 *
 * Description: Source file for the MC2 application code
 *
 * Author: Hana Khater
 *
 *******************************************************************************/
#include "MC2.h"

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/

uint16 g_counter = 0               ; // Global counter used identify time needed for each instruction
uint8 Password[NUM_DIGITS]         ; // Array in which we store new password
uint8 EEPROM_Password[NUM_DIGITS]  ; // Array used to read data stored in EEPROM
uint8 Test_Password[NUM_DIGITS]    ; // Array to store the password that we need to check if it is correct
uint8 Flag = 0                     ; // Global flag used in main to receive flags from MC1

/* 1. Use Overflow mode
 * 2. Use PRESCALER = 1024
 * 3. Initial value = 0
 */
Timer0_ConfigType Timer0_config = { Normal , 0 , FCPU_1024 , 0} ;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*Description :
 * 1. To receive the new password using UART driver
 * 2. Store the password in the EEPROM using I2C
 */

void MC2_recieveStorePass(void)
{
	/* Receive password in an array */
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		Password[index] = UART_recieveByte() ;
	}
	/* Store the new password in EEPROM */
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		 EEPROM_writeByte(EEPROM_ADDRESS + index, Password[index])  ;
		 /* a delay of 100 milliseconds to make sure that the data is stored in EEPROM successfully */
		 _delay_ms(100);
	}

}

/*Description :
 * 1. Check that the entered password in case of choosing (open gate) or (change pass) is correct
 * 2. If the password is correct it sends a flag using UART to open the gate / change password
 * 3. If the password is wrong it sends a flag (FALSE) to MC1
 */
void MC2_checkTheRecievedPass(void)
{
	/* local variable that increments each time we detect a mismatch between password */
	uint8 Failure = 0 ;
	/* Store the password that we want to check on in Test_Password array */
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		Test_Password[index] = UART_recieveByte();
	}
	/* Read data stored in EEPROM & store it in EEPROM_Password array */
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		EEPROM_readByte(EEPROM_ADDRESS + index, EEPROM_Password + index);
		_delay_ms(100);
	}
	/* Check if there is a mismatch between the received password & the password stored in EEPROM */
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		if(Test_Password[index] != EEPROM_Password[index])
			{
			  Failure++;
			}
	}
	if(Failure ==0)
	{
		/* if failure = 0 this means that there is no mismatch detected & the password entered is correct
		 * therefore we send a flag of TRUE to MC1
		 */
		UART_sendByte(TRUE);
	}
	else
	{
		/* if failure > 0 this means that there is a mismatch detected & the password entered is incorrect
		 * therefore we send a flag of FALSE to MC1
		 */
		UART_sendByte(FALSE);
	}
}

/*Description :
 * 1. It is called when MC1 sends a flag = (OPEN_GATE) using UART to MC2
 * 2. Once it is called it rotates the DC motor CLOCKWISE for 15 seconds
 *    Stop it for 3 seconds and finally rotates it ANTICLOCKWISE for 15 seconds
 *    using both DC_motor & Timer0 drivers
 */

void MC2_openAndCloseGate(void)
{
	DcMotor_init();
	/* Open the gate */
	DcMotor_rotate(CLOCKWISE);
	Timer0_init(&Timer0_config);
	Timer0_setCallBack(MC2_timer0Counter);
	/* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
	 * T(timer) = 1/F(timer) = 0.128 millisecond
	 * T(overflow) = 256*0.128 = 32.768 millisecond
	 * To count 15 seconds we need 459 interrupt (15/0.0327)
	 */
	while(g_counter != 459){}

	/* once we reach time needed we turn off Timer0 & reset the global counter g_counter
	 * in case we need it in another instruction
	 */
	Timer0_deinit();
	g_counter = 0 ;
	/* stop the gate */
	DcMotor_rotate(STOP);
	Timer0_init(&Timer0_config);
	Timer0_setCallBack(MC2_timer0Counter);
	/* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
	 * T(timer) = 1/F(timer) = 0.128 millisecond
	 * T(overflow) = 256*0.128 = 32.768 millisecond
	 * To count 3 seconds we need 92 interrupt (3/0.0327)
	 */
	while(g_counter != 92){}
	Timer0_deinit();
	g_counter = 0 ;
	/*Close the gate */
	DcMotor_rotate(ANTI_CLOCKWISE);
	Timer0_init(&Timer0_config);
	Timer0_setCallBack(MC2_timer0Counter);
	while(g_counter != 459){}
	Timer0_deinit();
	g_counter = 0 ;
	DcMotor_rotate(STOP);
}

/*Description :
 * 1. It is called when MC1 sends a flag = (BUZZER) using UART to MC2
 * 2. Once it is called it opens the buzzer for 1 minute using both buzzer & Timer0 drivers
 */

void MC2_TriggerBuzzer(void)
{
	Buzzer_On();
	Timer0_init(&Timer0_config);
	Timer0_setCallBack(MC2_timer0Counter);

	/* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
	 * T(timer) = 1/F(timer) = 0.128 millisecond
	 * T(overflow) = 256*0.128 = 32.768 millisecond
	 * To count 1 minute we need 1835 interrupt (60/0.0327)
	 */
	while(g_counter != 1835){}
	Buzzer_Off();
	Timer0_deinit();
	g_counter = 0 ;
}

/* Description :
 * 1. We store the address of this function in ISR of Timer0 so it increments a global counter (g_counter)
 *    each time there is an interrupt
 * 2. this function does'nt stop incrementing until we turn off Timer0 interrupts
 * 3. we turn off Timer0 interrupts when we reach the time needed and that depends on the function
 *    that opens Timer0 interrupts
 */

void MC2_timer0Counter(void)
{
	g_counter++;
}

int main()
{
	/* 1. Size of the character = 8 bits
	 * 2. Disable parity check
	 * 3. Use one stop bit
	 * 4. choose 9600 bps baud rate
	 */
	UART_ConfigType UART_Configuration = { EIGHT_BIT , DISABLED , ONE_BIT , 9600 } ;
	UART_init(&UART_Configuration);
	/* 1. SCL frequency = 400KHz
	 * 2. Address = 0x01
	 */
	TWI_ConfigType TWI_Configuration   = { 400 , 0x01 }  ;
    TWI_init(&TWI_Configuration);
    Buzzer_init();
	while(1)
	{
      Flag = UART_recieveByte();
      /* switch cases based on what flag of the four flags we defined is sent by MC1
       * for each flag there is a function that is called to execute specific instruction
       */
      switch (Flag)
      {
      case NEW_PASS :
    	  MC2_recieveStorePass();
    	  break ;
      case CHECK :
    	  MC2_checkTheRecievedPass();
    	  break ;
      case OPEN_GATE :
    	  MC2_openAndCloseGate();
    	  break ;
      case BUZZER :
    	  MC2_TriggerBuzzer();
    	  break ;
      }
	}
}

