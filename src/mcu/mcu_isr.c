

/**
 * @file    isr.c
 * @author  meng_yu
 * @brief   IRQ Handler function realize
 * @version 0.0.1
 * @date    2020-09-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "cmdline.h"
#include "drv_led.h"
#include "mcu_uart.h"
#include "mcu_isr.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/
volatile qword_t ticks = 0;

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/

//PreemptionPrio:抢占优先级
//SubPrio       :响应优先级
//IRQn          :中断编号
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
void NVIC_Config(IRQn_Type IRQn, byte_t PreemptionPrio, byte_t SubPrio, byte_t Enable)
{
    dword_t temp;

    if(Enable)
    {
        temp = PreemptionPrio << (4-2);
        temp |= SubPrio&(0x0f >> 2);
        temp &= 0xf;
        NVIC_EnableIRQ(IRQn);
        NVIC->IP[IRQn] = temp << 4;
    }
    else
    {
        NVIC_DisableIRQ(IRQn);
    }
}

void SysTick_Handler(void)
{
    static byte_t leds = 0x1;
    ticks++;
    if(ticks%1000 == 0)
    {
        if(leds)
        {
            drv_led_on(LED1_PIN);
        }
        else
        {
            drv_led_off(LED1_PIN);
        }
        leds =!leds;
    }
}


void USART1_IRQHandler(void)
{
    common_cmd_isr();
}


