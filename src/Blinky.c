/**
 * @file    Blinky.c
 * @author  meng_yu
 * @brief   Blinky for my MCU EVAL test
 * @version 0.0.1
 * @date    2020-10-01
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "stm32_config.h"
#include "sys.h"
#include "isr.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "lcd.h"
#include "mylogo.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/

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

extern dword_t SystemCoreClock;

byte_t flag = 1;


/*----------------------------------------------------------------------------
      MAIN function
*----------------------------------------------------------------------------*/
int main(void)
{
    word_t AD_value = 0;
    NVIC_SetPriorityGrouping(NVIC_GROUP4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000); //Generate interrupt each 1 ms
    led_init();
    usart_init(84, 115200);
    ADC_Init();

#ifdef USE_LCD_EN
    GLCD_Init();
    GLCD_Clear(White);
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(0, 0, __FI, (unsigned char*)" STM3240G-EVAL Demo ");
    GLCD_DisplayString(1, 0, __FI, (unsigned char*)"       Blinky       ");
    GLCD_DisplayString(2, 0, __FI, (unsigned char*)"    www.keil.com    ");
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);
#endif

    while(1)
    {
        if(flag)
        {
            flag = 0;
            GLCD_Bitmap(0, 0, 107, 107, (unsigned char*)mylogo);
        }
        ADC_StartCnv();
        while(!ADC_DoneCnv());
        if(ticks % 100 == 0)
        {
            AD_value = ADC_GetCnv(); /* Read AD_last value                 */

#ifdef USE_LCD_EN
            GLCD_SetTextColor(Red);
            GLCD_Bargraph(9*__FONT_WIDTH, 6*__FONT_HEIGHT, 10*__FONT_WIDTH, __FONT_HEIGHT-2, (AD_value >> 2));
            GLCD_SetTextColor(White);
            GLCD_SetTextColor(Blue);
            sprintf(text, "AD value = 0x%04X", AD_value);
            GLCD_DisplayString(5, 0, __FI, (unsigned char*) text);
#endif
        }
    }
}


