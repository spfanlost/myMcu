

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
#include "stm32_config.h"
#include "common_cmd.h"
#include "led.h"
#include "usart.h"
#include "isr.h"

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

/*!< Specifies the IRQ channel to be enabled or disabled.
      This parameter can be an enumerator of @ref IRQn_Type
      enumeration (For the complete STM32 Devices IRQ Channels
      list, please refer to stm32f4xx.h file) */

/*!< Specifies the pre-emption priority for the IRQ channel
      specified in NVIC_IRQChannel. This parameter can be a value
      between 0 and 15 as described in the table @ref MISC_NVIC_Priority_Table
      A lower priority value indicates a higher priority */

/*!< Specifies the subpriority level for the IRQ channel specified
      in NVIC_IRQChannel. This parameter can be a value
      between 0 and 15 as described in the table @ref MISC_NVIC_Priority_Table
      A lower priority value indicates a higher priority */

/*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
      will be enabled or disabled.
      This parameter can be set either to ENABLE or DISABLE */
void NVIC_Config(IRQn_Type IRQn, uint8_t PreemptionPrio, uint8_t SubPrio, uint8_t Enable)
{
    uint32_t temp;

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
            led_on(LED1_PIN);
            led_on(LED2_PIN);
        }
        else
        {
            led_off(LED1_PIN);
            led_off(LED2_PIN);
        }
        leds =!leds;
    }
}


void USART1_IRQHandler(void)
{
    common_cmd_isr();
}


