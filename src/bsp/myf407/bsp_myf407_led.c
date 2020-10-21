/**
 * @file    bsp_myf407_led.c
 * @author  meng_yu
 * @brief   myf407 board led function realize
 * @version 0.0.1
 * @date    2020-10-17
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
dword_t bsp_myf407_led_init(void);
dword_t bsp_myf407_led_on(dword_t pin);
dword_t bsp_myf407_led_off(dword_t pin);
dword_t bsp_myf407_led_toggle(dword_t pin);
dword_t bsp_myf407_led_is_on(dword_t pin);

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
struct bsp_led bsp_myf407_led =
{
    bsp_myf407_led_init,
    bsp_myf407_led_on,
    bsp_myf407_led_off,
    bsp_myf407_led_toggle,
    bsp_myf407_led_is_on,
};

/**
 * @brief  This function Initialize LEDs
 * @param  None
 * @note:  call
 */
dword_t bsp_myf407_led_init(void)
{
    mcu_io_clk_enable(GPIOF_CLK);
    /* Configure LED (PF9,PF10) pins as push-pull outputs */
    mcu_io_config(GPIOF, LED1_PIN | LED2_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    bsp_myf407_led_off(LED1_PIN);
    return SUCCESS;
}

dword_t bsp_myf407_led_on( dword_t pin)
{
    mcu_io_set(GPIOF, pin);
    return SUCCESS;
}

dword_t bsp_myf407_led_off(dword_t pin)
{
    mcu_io_reset(GPIOF, pin);
    return SUCCESS;
}

dword_t bsp_myf407_led_toggle(dword_t pin)
{
    mcu_io_toggle(GPIOF, pin);
    return SUCCESS;
}

dword_t bsp_myf407_led_is_on(dword_t pin)
{
    if(mcu_io_pin_val(GPIOF, pin))
    {
        return TRUE;
    }
    return FALSE;
}


