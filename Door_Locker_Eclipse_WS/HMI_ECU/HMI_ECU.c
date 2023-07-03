/******************************************************************************
 *
 * Module: 	Micro-controller 1
 *
 * File Name: MC1.c
 *
 * Description: Source file for the MC1 application code
 *
 * Author: Hana Khater
 *
 *******************************************************************************/

#include "MC1.h"

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/

uint16 g_counter = 0                    ; // Global counter used identify time needed for each instruction
uint8 Password[NUM_DIGITS]              ; // Array in which we store new password
uint8 Confirmation_Password[NUM_DIGITS] ; // Array in which we store the confirmation of new password
/* 1. Use Overflow mode
 * 2. Use PRESCALER = 1024
 * 3. Initial value = 0
 */
Timer0_ConfigType Timer0_config = { Normal , 0 , FCPU_1024 , 0} ;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description :
 * Displays a message to enter new password to the user & store it in the array "Password"
 */
void MC1_askForPass(void)
{
	LCD_clearScreen(); //in case their is something displayed before this message
	LCD_displayString("Enter new pass");
	LCD_moveCursor(1,0);
	for(uint8 index =0 ; index<NUM_DIGITS ; index++)
	{
		Password[index] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(2000);
	}
	LCD_clearScreen();
	LCD_displayString("Re-Enter pass");
	LCD_moveCursor(1,0);

	/* we store the 2nd trial of entering the new password in confirmation_password array
	 * to check later that there is no mismatch
	 */
	for(uint8 index =0 ; index <NUM_DIGITS ; index ++)
	{
		Confirmation_Password[index] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(2000);

	}
}

/*Description :
 *  Check if there is a mismatch between the password and confirmation password
 */
uint8 MC1_matchingCheck(void)
{
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
		if( Password[index] != Confirmation_Password[index] )
			return 0;
	}
	return 1;
}

/* Description :
 * Sends the new password MC2 so it would be stored in EEPROM
 */

void MC1_sendNewPassword()
{
	/* Send NEW_PASS flag to MC2 so it generates the function that stores the password in EEPROM */
	UART_sendByte(NEW_PASS);
	for(uint8 index =0 ; index <NUM_DIGITS ; index++)
	{
      UART_sendByte(Password[index]);
	}
}

/* Description :
 * Display main options : either to open the door or to change the password
 */

void MC1_mainOptions(void)
{

	uint8 counter = 0 ; // to count the number of times of mismatch
	uint8 option  = 0 ; // a variable in which we store the instruction that the user entered (+ or -)
	LCD_clearScreen(); //in case their is something displayed before this message
	LCD_displayString("+ : Open door");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass");
	option = KEYPAD_getPressedKey();
	if( option == '+')
	{
      LCD_clearScreen();
      LCD_displayString("Enter Password");
      LCD_moveCursor(1,0);
      /* Send CHECK flag to MC2 so it generates the function that detects if the password entered is correct */
      UART_sendByte(CHECK);
      for(uint8 index =0; index<NUM_DIGITS ; index++)
      {
    	  _delay_ms(2000);
    	  UART_sendByte(KEYPAD_getPressedKey());
    	  LCD_displayCharacter('*');

      }
      /* a variable that receives a flag from MC2 that identifies either
       * the password entered is correct (TRUE) or not correct (FALSE)
       */
      uint8 condition = UART_recieveByte() ;
      while( condition == FALSE )
      {
    	  /* checking if the user reached the maximum number of mismatches that equals 3 */
    	  if(counter == 3)
    	  {
    		  LCD_clearScreen();
    		  LCD_displayString("THIEF !");
    		  /* Send BUZZER flag to MC2 so it generates the function that opens the buzzer */
    		  UART_sendByte(BUZZER);
    		  Timer0_init(&Timer0_config);
    		  Timer0_setCallBack(MC1_timer0Counter);

    		  /* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
    		   * T(timer) = 1/F(timer) = 0.128 millisecond
    		   * T(overflow) = 256*0.128 = 32.768 millisecond
    		   * To count 1 minute we need 1835 interrupt (60/0.0327)
    		   */
    		  while(g_counter != 1835){}
    		  /* once we reach time needed we turn off Timer0 & reset the global counter g_counter
    		   * in case we need it in another instruction
    		   */
    		  Timer0_deinit();
    		  g_counter = 0 ;
    		  /* Display again the main options */
    		  MC1_mainOptions();
    	  }
    	  LCD_clearScreen();
    	  LCD_displayString("Enter Password");
    	  LCD_moveCursor(1,0);
    	  UART_sendByte(CHECK);
    	  for(uint8 index =0; index<NUM_DIGITS ; index++)
    	  {
    		  _delay_ms(2000);
    		  UART_sendByte(KEYPAD_getPressedKey());
    		  LCD_displayCharacter('*');

    	  }
    	  condition = UART_recieveByte();
    	  counter++; // increment the counter each time there is a mismatch detected
      }
      if(condition == TRUE)
      {
    	  /* Send OPEN_GATE flag to MC2 so it generates the function that opens the gate */
    	  UART_sendByte(OPEN_GATE);
    	  LCD_clearScreen();
    	  LCD_displayString("Gate Opening");
    	  Timer0_init(&Timer0_config);
    	  Timer0_setCallBack(MC1_timer0Counter);
    	  /* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
    	   * T(timer) = 1/F(timer) = 0.128 millisecond
    	   * T(overflow) = 256*0.128 = 32.768 millisecond
    	   * To count 15 seconds we need 459 interrupt (15/0.0327)
    	   */
    	  while(g_counter != 459){}
    	  Timer0_deinit();
    	  g_counter = 0 ;
    	  LCD_clearScreen();
    	  LCD_displayString("Gate Opened");
    	  Timer0_init(&Timer0_config);
    	  Timer0_setCallBack(MC1_timer0Counter);
    	  /* F(timer) = 8 MHZ / 1024 = 7.8125 KHz
    	   * T(timer) = 1/F(timer) = 0.128 millisecond
    	   * T(overflow) = 256*0.128 = 32.768 millisecond
    	   * To count 3 seconds we need 92 interrupt (3/0.0327)
    	   */
    	  while(g_counter != 92){}
    	  Timer0_deinit();
    	  g_counter = 0 ;
    	  LCD_clearScreen();
    	  LCD_displayString("Gate Closing");
    	  Timer0_init(&Timer0_config);
    	  Timer0_setCallBack(MC1_timer0Counter);
    	  while(g_counter != 459){}
    	  Timer0_deinit();
    	  g_counter = 0 ;
    	  MC1_mainOptions();
      }

	}
	else if ( option == '-')
	{
	    uint8 condition = 0 ;
		UART_sendByte(CHECK);
		LCD_clearScreen();
		LCD_displayString("Enter Password");
		LCD_moveCursor(1,0);

		for(uint8 index =0; index<NUM_DIGITS ; index++)
		{
			_delay_ms(2000);
			UART_sendByte(KEYPAD_getPressedKey());
			LCD_displayCharacter('*');
		}
		condition = UART_recieveByte();
		if(condition == FALSE)
		{
			LCD_clearScreen();
			LCD_displayString("Wrong Pass");
			_delay_ms(2000);
			MC1_mainOptions();
		}

		else if(condition == TRUE)
		_delay_ms(2000);
		do
		{
			MC1_askForPass();
		}
		while(!( MC1_matchingCheck() ) ) ;

		MC1_sendNewPassword();
		MC1_mainOptions();
	}
	/* in case the user pressed on wrong keys */
	else
	{
		LCD_clearScreen();
	    LCD_displayString("Wrong Input");
	    _delay_ms(2000);
		MC1_mainOptions();
	}

}


/* Description :
 * 1. We store the address of this function in ISR of Timer0 so it increments a global counter (g_counter)
 *    each time there is an interrupt
 * 2. this function does'nt stop incrementing until we turn off Timer0 interrupts
 * 3. we turn off Timer0 interrupts when we reach the time needed and that depends on the function
 *    that opens Timer0 interrupts
 */

void MC1_timer0Counter(void)
{
	g_counter++;
}

int main(void)
{
  LCD_init();

  /* 1. Size of the character = 8 bits
   * 2. Disable parity check
   * 3. Use one stop bit
   * 4. choose 9600 bps baud rate
   */
  UART_ConfigType UART_Configuration = { EIGHT_BIT , DISABLED , ONE_BIT , 9600 } ;
  UART_init(&UART_Configuration);
  while(1)
  {

	  do
	  {
        MC1_askForPass();
	  }
	  while(!( MC1_matchingCheck() ) ) ; // In case there is a mismatch between 1st & 2nd time we repeat the function
      /* Store new password */
	  MC1_sendNewPassword();
	  /* Print on LCD the main options (open gate / change password) */
	  MC1_mainOptions();

  }
}

