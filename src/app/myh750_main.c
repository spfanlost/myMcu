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
#include "app_param.h"
#include "app_paint.h"
#include "SEGGER_RTT.h"

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
    // static char r;
    task_init();
    SCB_EnableICache();
    SCB_EnableDCache();
    NVIC_SetPriorityGrouping(NVIC_GROUP4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); //Generate interrupt each 1 ms
    drv_led_init();
    mcu_uart_init(84, 115200);

    // SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n");
    // SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    // r = SEGGER_RTT_WaitKey();
    // SEGGER_RTT_Write(0, &r, 1);
    // r++;

    while (1)
    {
        task_now = task_now->pnxt;
        if (task_now->flag)
        {
            task_now->func();
        }
    }
}


