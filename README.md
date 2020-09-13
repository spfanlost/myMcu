The 'Blinky' project is a simple demo program for
ST 'STM32F407Z' microcontroller using ST 'STM3240G-EVAL' Evaluation Board.
Compliant to Cortex Microcontroller Software Interface Standard (CMSIS v2.0).

Example functionality:
 - Clock Settings:
   - XTAL    =           8.00 MHz
   - SYSCLK  =          168.00 MHz

 - Sys Timer is used in interrupt mode
 - LED blink with speed depending on potentiometer position
 - colour LCD display shows init message and bargraph depending on potentiometer position
 - AD settings: 12 bit resolution
 - AD value is output onto serial port UART4 
   UART4 settings: 115200 baud, 8 data bits, no parity, 1 stop bit
 

The Blinky program is available in different targets:

  STM32F407 RAM:    runs from Internal RAM located on chip
                    (may be used for target debugging)

  STM32F407 Flash:  runs from Internal Flash located on chip
                    (used for production or target debugging)
