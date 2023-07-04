# Door_Locker_Security_System
C Project - Based on Atmega32 Microcontroller
- Developed a system that takes a password & confirms it using keypad, password is hidden into (*) on an LCD , previous features are implemented in HMI_ECU, Password is sent to control_ECU using UART to be stored in external EEPROM to compare the input with it for future trials of opening the door, a buzzer is set in case of 3 false trials.
and if the password is entered wrong for 3 times a buzzer will start for 1 minute.
- Drivers: GPIO, Keypad, LCD, Timer, UART, I2C, EEPROM, Buzzer and DC-Motor
