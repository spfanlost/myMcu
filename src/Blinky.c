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
#include "bsp_lcd.h"
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
    GLCD_SetBackgroundColor (GLCD_COLOR_BLACK);
    GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
    GLCD_ClearScreen        ();
    GLCD_SetFont            (&GLCD_Font_12x24);
    GLCD_DrawString         (0, 0*24, "  STM32407ZG  ");
    GLCD_DrawString         (0, 1*24, "    Blinky    ");
    GLCD_DrawString         (0, 2*24, " www.keil.com ");
#endif

    while(1)
    {
        ADC_StartCnv();
        while(!ADC_DoneCnv());
        if(ticks % 100 == 0)
        {
            AD_value = ADC_GetCnv(); /* Read AD_last value                 */

#ifdef USE_LCD_EN
            GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
            sprintf(text, "Value=%#X", AD_value);
            GLCD_DrawString(0, 3*24, text);
            GLCD_SetForegroundColor (GLCD_COLOR_RED);
            GLCD_DrawBargraph(0, 4*24, 14*12, 24, AD_value*200/0xFFF);
#endif
            if(flag)
            {
                flag = 0;
                GLCD_DrawBitmap(24, 6*24 , 107, 107, (unsigned char*)mylogo);
            }
        }
    }
}


