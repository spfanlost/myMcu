/**
 * @file    uart.c
 * @author  meng_yu
 * @brief   Low level serial definitions function realize
 * @version 0.0.1
 * @date    2020-09-09
 * 
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "stm32f4_conf.h"
#include "uart.h"
#include "gpio.h"

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

extern void NVIC_Config(uint8_t Channel, uint8_t PreemptionPrio, uint8_t SubPrio, uint8_t Enable);

/**
 * @brief  This function is uart_init
 * @param  pclk2: pclock (MHz)
 * @param  bound: bound
 * @note:  call
 */
void uart_init(uint32_t pclk2,uint32_t bound)
{    
    float temp;
    uint16_t mantissa;
    uint16_t fraction;     
    temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
    mantissa=temp;               //得到整数部分
    fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
    mantissa|=fraction; 
    RCC->AHB1ENR|=1<<0;     //使能PORTA口时钟
    RCC->APB2ENR|=1<<4;     //使能串口1时钟
    //TX = PA9,RX = PA10,复用功能,上拉输出
    GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOA,9,7); //PA9,AF7
    GPIO_AF_Set(GPIOA,10,7);//PA10,AF7
    USART1->BRR=mantissa;   //bound
    USART1->CR1&=~(1<<15);  //OVER8=0
    USART1->CR1|=1<<3;      //enable Tx
    USART1->CR1|=1<<2;      //enable Rx
    USART1->CR1|=1<<5;      //enable Rx irq
    NVIC_Config(USART1_IRQn, 3, 3, ENABLE);
    USART1->CR1|=1<<13;     //enable USART
}

/**
 * @brief  This function uart_put_char
 * @param  ch: char send out
 * @return send out char
 * @retval param
 * @note:  call
 */
int uart_put_char(int32_t ch)
{
    while (!(USART1->SR & (0x1 << 7)));
    USART1->DR = (ch & 0xFF);
    return (ch);
}

/**
 * @brief  This function uart_get_char
 * @param  None
 * @return get char
 * @retval get char
 * @note:  call
 */
int uart_get_char(void)
{
    int ret = -1;
    if (USART1->SR & 0x0020)
        ret = USART1->DR;
    return ret;
}

/**
 * @brief  This function uart_chk_char
 * @param  None
 * @return if a char is vld,return true
 * @retval a char is vld,return true
 * @note:  a char not vld,return false
 */
int uart_chk_char(void) 
{
  if (USART1->SR & (0x1 << 5))
    return TRUE;
  else
    return FALSE;
}

