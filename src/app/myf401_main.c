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
#include "mcu_flash.h"
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

dword_t Save_DataTab[1024] = {0};


/*----------------------------------------------------------------------------
      MAIN function
*----------------------------------------------------------------------------*/
int main(void)
{
    word_t i = 0;
    word_t AD_value = 0;
    dword_t temp = 0x88888888;

    NVIC_SetPriorityGrouping(NVIC_GROUP4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); //Generate interrupt each 1 us
    drv_led_init();
    mcu_uart_init(84, 115200);
    mcu_adc_init();

    LOG_INFO("Device: %s, SystemCoreClock:%dMHz\r\n", DEVICE_STR, SystemCoreClock/1000000);

    for(i = 0; i < 1024; i++)
    {
        Save_DataTab[i] = i;
    }
    mcu_flash_write(FLASH_SAVE_ADDR, Save_DataTab, 1024);
    memset((void *)Save_DataTab,0,sizeof(Save_DataTab));
    mcu_flash_read(FLASH_SAVE_ADDR, Save_DataTab, 1024);
    for(i = 0; i < 1024; i++)
    {
        LOG_INFO("0x%08x=%08x\r\n", FLASH_SAVE_ADDR + (4 * i), Save_DataTab[i]);
    }


    while(1)
    {
        mcu_adc_start_conv();
        if(get_sys_ticks()%(500*1000) <= 30)
        {
            drv_led_toggle(LED1_PIN);
            AD_value = mcu_adc_get_conv(); /* Read AD  value                 */
//            LOG_INFO ("Value=%#X\r\n", AD_value);
        }
    }
}


