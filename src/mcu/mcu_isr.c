

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
#include "mcu_uart.h"
#include "mcu_isr.h"

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

volatile qword_t ticks = 0;
volatile static qword_t us_ticks = 0;
volatile static qword_t ms_ticks = 0;
static qword_t curTicks = 0;

void delay_us(qword_t us)
{
    us_ticks = 0;
    curTicks = us_ticks;
    while((us_ticks - curTicks) < us);
}
void delay_ms(qword_t ms)
{
    us_ticks = 0;
    ms_ticks = 0;
    curTicks = ms_ticks;
    while((ms_ticks - curTicks) < ms);
}

void SysTick_Handler(void)
{
    ticks++;
    us_ticks++;
    if(us_ticks%1000 == 0)
    {
        ms_ticks ++;
    }
}

qword_t get_sys_ticks(void)
{
    return ticks;
}

extern void button_callback(void);
/**
  * @brief  This function handles external lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    static dword_t key_temp = 0;
    /* Manage Flags */
    if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
    {
        if((LL_GPIO_ReadInputPort(GPIOC)&LL_GPIO_PIN_13) == 0)
        {
            key_temp++;
        }
        else
        {
            key_temp = 0;
        }
        if(key_temp > 100)
        {
            key_temp = 0;
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
            /* Handle User push-button press in dedicated function */
            button_callback(); 
        }
    }
}

void USART1_IRQHandler(void)
{
    common_cmd_isr();
}


