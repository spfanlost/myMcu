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
#include "task.h"
#include "mcu.h"
#include "mcu_sys.h"
#include "mcu_isr.h"
#include "mcu_uart.h"
#include "mcu_adc.h"
#include "drv_led.h"
#include "drv_lcd.h"
#include "touch.h"
#include "mylogo.h"
#include "app_param.h"
#include "app_paint.h"
/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/
extern dword_t SystemCoreClock;

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
void debug_msg(void)
{
    LOG_INFO ("touch_adj_done? %d\r\n", g_param.touch_adj_done);
}

/*----------------------------------------------------------------------------
      MAIN function
*----------------------------------------------------------------------------*/
int main(void)
{
    task_init();
    NVIC_SetPriorityGrouping(NVIC_GROUP4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); //Generate interrupt each 1 ms
    drv_led_init();
    mcu_uart_init(84, 115200);
    mcu_adc_init();

#ifdef USE_LCD_EN
    LCD_Init();
    touch_init();
    app_paint_init();
#endif
    while (1)
    {
        task_now = task_now->pnxt;
        if (task_now->flag)
        {
            task_now->func();
        }
    }
}


