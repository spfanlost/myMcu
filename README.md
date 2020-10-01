### myMcu
The 'Blinky' project is a simple demo program for my STM32 Evaluation Board.

>Evaluation Board:
```
f401re 
f407zg has a LCD
f429ig 
h750ig 
```
- Sys Timer is used in interrupt mode
- LED blink with speed depending on potentiometer position
- colour LCD display shows init message and bargraph depending on potentiometer position
- AD settings: 12 bit resolution
- AD value is output onto serial port UART1 
  UART1 settings: 115200 baud, 8 data bits, no parity, 1 stop bit

in this program, usart is designed as a commond interface:
type: "help" for more info

