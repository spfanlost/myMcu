

/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM3240G-EVAL
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32_config.h"
#include "common.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "lcd.h"
//VECT_TAB_SRAM
#define __FI 1 /* Font index 16x24                  */

#if ( __FI == 1)

/* Font index  6x8*/
#define __FONT_WIDTH 16
#define __FONT_HEIGHT 24

#else

/* Font index 16x24*/
#define __FONT_WIDTH 6
#define __FONT_HEIGHT 8
#endif

char text[40];

/* Import external variables from IRQ.c file                                  */
extern volatile unsigned char clock_1s;
extern uint32_t SystemCoreClock;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void)
{
    uint16_t AD_value = 0;
    uint16_t AD_print = 0;
    NVIC_SetPriorityGrouping(4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000); //Generate interrupt each 1 ms
    led_init();
    usart_init(84, 115200);

    //ADC_Init();                           /* ADC Initialization                 */
#ifdef USE_LCD_EN
    GLCD_Init();
    GLCD_Clear(White);
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(0, 0, __FI, " STM3240G-EVAL Demo ");
    GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
    GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);
#endif

    while(1)
    {
        //ADC_StartCnv ();
        //while (!ADC_DoneCnv());
        //AD_value = ADC_GetCnv();            /* Read AD_last value                 */
        if(AD_value!=AD_print)
        {
#ifdef USE_LCD_EN
            GLCD_SetTextColor(Red);
            GLCD_Bargraph(9*__FONT_WIDTH, 6*__FONT_HEIGHT, 10*__FONT_WIDTH, __FONT_HEIGHT-2, (AD_value >> 2));
            GLCD_SetTextColor(White);
#endif

            AD_print = AD_value; /* Get unscaled value for printout    */
        }
        if(clock_1s)
        {
            clock_1s = 0;
            sprintf(text, "AD value = 0x%04X", AD_print);

#ifdef USE_LCD_EN
            GLCD_SetTextColor(Blue);
            GLCD_DisplayString(5, 0, __FI, (unsigned char*) text);
#endif
        }
    }
}


