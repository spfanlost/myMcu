

/**
* @file    drv_led.c
* @author  meng_yu
* @brief   Low level LED functions function realize
* @version 0.0.1
* @date    2020-09-10
*
* @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
*/
#include "common.h"
#include "stm32_config.h"
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

/**
 * @brief  This function Initialize LEDs
 * @param  None
 * @note:  call
 */
void led_init(void)
{
    RCC->AHB1ENR |= (1UL << 5); //Enable clock for GPIOF

    /* Configure LED (PF9,PF10) pins as push-pull outputs */
    GPIO_Set(GPIOF, LED1_PIN | LED2_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
}


/**
* @brief  This function Turns on requested LED
* @param  pin: led's pin
* @note:  call
*/
void led_on(dword_t pin)
{
    GPIOF->BSRR |= pin;
}


/**
* @brief  This function Turns off requested LED
* @param  pin: led's pin
* @note:  call
*/
void led_off(dword_t pin)
{
    GPIOF->BSRR |= (pin << 16);
}


