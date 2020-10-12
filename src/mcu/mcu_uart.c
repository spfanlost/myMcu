/**
 * @file    mcu_uart.c
 * @author  meng_yu
 * @brief   mcu uart function realize
 * @version 0.0.1
 * @date    2020-10-11
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_io.h"
#include "mcu_isr.h"
#include "mcu_uart.h"


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

/**
 * @brief  This function is uart init
 * @param  pclk2: pclock (MHz)
 * @param  bound: bound
 * @note:  call
 */
void mcu_uart_init(dword_t pclk2, dword_t bound)
{
    dword_t mantissa;
    dword_t fraction;

    mantissa = (pclk2*1000000) / (bound*16); //USARTDIV@OVER8=0
    fraction = ((pclk2*100000000) / (bound*16))%100; //@OVER8=0
    mantissa <<= 4;
    mantissa |= fraction;
    mcu_io_clk_enable(GPIOA_CLK);
    RCC->APB2ENR |= 1 << 4; //enable uart 1 clock

    //TX = PA9,RX = PA10
    mcu_io_config(GPIOA, PIN9 | PIN10, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PU);
    mcu_io_af_config(GPIOA, 9, 7); //PA9,AF7
    mcu_io_af_config(GPIOA, 10, 7); //PA10,AF7
    USART1->BRR = mantissa; //bound
    USART1->CR1 &=~(1 << 15); //OVER8=0
    USART1->CR1 |= 1 << 3; //enable Tx
    USART1->CR1 |= 1 << 2; //enable Rx
    USART1->CR1 |= 1 << 5; //enable Rx irq
    NVIC_Config(USART1_IRQn, 8, 0, ENABLE);
    USART1->CR1 |= 1 << 13; //enable USART
}


/**
 * @brief  This function uart put_char
 * @param  ch: char send out
 * @return send out char
 * @retval param
 * @note:  call
 */
int mcu_uart_put_char(int32_t ch)
{
    while(! (USART1->SR&(0x1 << 7)));
    USART1->DR = (ch&0xFF);
    return (ch);
}

/**
 * @brief  This function is uart_get_char
 * @param  None
 * @return get char
 * @retval get char
 * @note:  call
 */
int mcu_uart_get_char(void)
{
    int ret =-1;

    if(USART1->SR&0x0020) ret = USART1->DR;
    return ret;
}


/**
 * @brief  This function uart_chk_char
 * @param  None
 * @return if a char is vld,return true
 * @retval a char is vld,return true
 * @note:  a char not vld,return false
 */
int mcu_uart_chk_char(void)
{
    if(USART1->SR&(0x1 << 5)) return TRUE;

    else return FALSE;
}

