/**
 * @file    bsp_myh750_led.c
 * @author  meng_yu
 * @brief   myf401 board led function realize
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#include "common.h"
#include "mcu.h"
#include "drv_led.h"
#include "bsp_myh750_led.h"

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
dword_t bsp_myh750_led_init(void);
dword_t bsp_myh750_led_on(dword_t pin);
dword_t bsp_myh750_led_off(dword_t pin);
dword_t bsp_myh750_led_toggle(dword_t pin);
dword_t bsp_myh750_led_is_on(dword_t pin);

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
struct bsp_led bsp_myh750_led =
{
    bsp_myh750_led_init,
    bsp_myh750_led_on,
    bsp_myh750_led_off,
    bsp_myh750_led_toggle,
    bsp_myh750_led_is_on,
};

/**
 * @brief  This function Initialize LEDs
 * @param  None
 * @note:  call
 */
dword_t bsp_myh750_led_init(void)
{
    mcu_io_clk_enable(LEDR_PORT|LEDG_PORT|LEDB_PORT);
    bsp_myh750_led_off(LEDR_PIN|LEDG_PIN|LEDB_PIN);
    mcu_io_config(GPIOB, LEDR_PIN|LEDG_PIN|LEDB_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_RES);
    return SUCCESS;
}

dword_t bsp_myh750_led_on( dword_t pin)
{
    mcu_io_reset(GPIOB, pin);
    return SUCCESS;
}

dword_t bsp_myh750_led_off(dword_t pin)
{
    mcu_io_set(GPIOB, pin);
    return SUCCESS;
}

dword_t bsp_myh750_led_toggle(dword_t pin)
{
    mcu_io_toggle(GPIOB, pin);
    return SUCCESS;
}

dword_t bsp_myh750_led_is_on(dword_t pin)
{
    if(mcu_io_pin_val(GPIOB, pin))
    {
        return TRUE;
    }
    return FALSE;
}

