
/**
 * @file    mcu_io.c
 * @author  meng_yu
 * @brief   &#& function realize
 * @version 0.0.1
 * @date    2020-09-07
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_io.h"

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
 * @brief  This function mcu_io_config
 * @param  GPIOx: GPIOA~GPIOI.
 * @param  PINx: PINx:0X0000~0XFFFF, each bit means a pin
 * @param  MODE: 0~3;mode sel,0,input(sys reset default);1,normal output;2,af function;3,anlog input.
 * @param  OTYPE: 0/1;output type sel,0, pop output;1, open output.
 * @param  OSPEED: 0~3;output speed config,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
 * @param  PUPD: pull up/down,0,none;1,pull up;2,pull down;
 * @note:  in input mode(normal/anlog),OTYPE&OSPEED invld!!
 */
void mcu_io_config(GPIO_TypeDef*GPIOx,
    dword_t PINx,
    dword_t MODE,
    dword_t OTYPE,
    dword_t OSPEED,
    dword_t PUPD)
{
    dword_t pinpos = 0, pos = 0, curpin = 0;

    for(pinpos = 0; pinpos<16; pinpos++)
    {
        pos = 1 << pinpos;
        curpin = PINx&pos;
        if(curpin==pos)
        {
            GPIOx->MODER &=~(3 << (pinpos*2));
            GPIOx->MODER |= MODE << (pinpos*2);
            if((MODE==0X01)||(MODE==0X02))
            {
                GPIOx->OSPEEDR &=~(3 << (pinpos*2));
                GPIOx->OSPEEDR |= (OSPEED << (pinpos*2));
                GPIOx->OTYPER &=~(1 << pinpos);
                GPIOx->OTYPER |= OTYPE << pinpos;
            }
            GPIOx->PUPDR &=~(3 << (pinpos*2));
            GPIOx->PUPDR |= PUPD << (pinpos*2);
        }
    }
}


/**
 * @brief  This function mcu_io_af_config
 * @param  GPIOx: GPIOA~GPIOI.
 * @param  PINx: 0~15,means IO pin index.
 * @param  AFx: AF0~AF15.AF0~15config (normal use,refer to STM32F40x-DataSheet,Table 9):
 *      AF0:MCO/SWD/SWCLK/RTC      AF1:TIM1/TIM2;                AF2:TIM3~5;                    AF3:TIM8~11
 *      AF4:I2C1~I2C4;             AF5:SPI1~SPI6;                AF6:SPI3/SAI1;                 AF7:SPI2/3/USART1~3/
    UART5/SPDIFRX;
 *      AF8:USART4~8/SPDIFRX/SAI2; AF9;CAN1~2/TIM12~14/LCD/QSPI; AF10:USB_OTG/USB_HS/SAI2/QSPI; AF11:ETH
 *      AF12:FMC/SDIO/OTG/HS       AF13:DCIM                     AF14:LCD;                      AF15:EVENTOUT
 * @note:  call
 */
void mcu_io_af_config(GPIO_TypeDef*GPIOx, dword_t pin, dword_t AFx)
{
    GPIOx->AFR[pin >> 3] &=~(0X0F << ((pin&0X07)*4));
    GPIOx->AFR[pin >> 3] |= ((dword_t)AFx) << ((pin&0X07)*4);
}

void mcu_io_clk_enable(dword_t gpio_clk)
{
    RCC->AHB1ENR |= gpio_clk;
}

void mcu_io_set(GPIO_TypeDef*GPIOx, dword_t pin)
{
    GPIOx->BSRR |= pin;
}

void mcu_io_reset(GPIO_TypeDef*GPIOx, dword_t pin)
{
    GPIOx->BSRR |= (pin << 16);
}

void mcu_io_toggle(GPIO_TypeDef*GPIOx, dword_t pin)
{
    GPIOx->ODR ^= pin;
}

byte_t mcu_io_pin_val(GPIO_TypeDef*GPIOx, dword_t pin)
{
    if ((GPIOx->IDR & pin) == 0)
    {
        return 0;
    }
    return 1;
}


