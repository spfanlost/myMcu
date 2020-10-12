

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
#include "mcu.h"
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
void drv_led_init(void)
{
    mcu_io_clk_enable(GPIOF_CLK);
    /* Configure LED (PF9,PF10) pins as push-pull outputs */
    mcu_io_config(GPIOF, LED1_PIN | LED2_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
}


/**
* @brief  This function Turns on requested LED
* @param  pin: led's pin
* @note:  call
*/
void drv_led_on(dword_t pin)
{
    mcu_io_set(GPIOF, pin);
}


/**
* @brief  This function Turns off requested LED
* @param  pin: led's pin
* @note:  call
*/
void drv_led_off(dword_t pin)
{
    mcu_io_reset(GPIOF, pin);
}


