/**
 * @file    main.c
 * @author  meng_yu
 * @brief   Blinky for my MCU EVAL test
 * @version 0.0.1
 * @date    2020-10-01
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_sys.h"
#include "mcu_isr.h"
#include "mcu_uart.h"
#include "mcu_adc.h"
#include "drv_led.h"

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

extern dword_t SystemCoreClock;


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
    drv_led_init();
    mcu_uart_init(84, 115200);
    mcu_adc_init();

    printf("Device: %s, SystemCoreClock:%dMHz pllp:%d pllm:%d plln:%d,%d\r",
    DEVICE_STR, SystemCoreClock/1000000,
    (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2,
    RCC->PLLCFGR & RCC_PLLCFGR_PLLM,
    ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6),
    AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)]);

    while(1)
    {
        mcu_adc_start_conv();
        if(ticks % 100 == 0)
        {
            AD_value = mcu_adc_get_conv(); /* Read AD  value                 */
//            LOG_INFO ("Value=%#X\r\n", AD_value);
        }
    }
}


