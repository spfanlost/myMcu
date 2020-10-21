

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
#if defined(STM32F407xx)
struct bsp_led *drv_led = &bsp_myf407_led;
#elif defined(STM32F401xE)
struct bsp_led *drv_led = &bsp_myf401_led;
#else

#endif

/**
 * @brief  This function Initialize LEDs
 * @param  None
 * @note:  call
 */
void drv_led_init(void)
{
    drv_led->led_init();
}


/**
* @brief  This function Turns on requested LED
* @param  pin: led's pin
* @note:  call
*/
void drv_led_on(dword_t pin)
{
    drv_led->led_on(pin);
}


/**
* @brief  This function Turns off requested LED
* @param  pin: led's pin
* @note:  call
*/
void drv_led_off(dword_t pin)
{
    drv_led->led_off(pin);
}

/**
* @brief  This function let LED toggle
* @param  pin: led's pin
* @note:  call
*/
void drv_led_toggle(dword_t pin)
{
    drv_led->led_toggle(pin);
}

/**
* @brief  This function check LED is on
* @param  pin: led's pin
* @note:  call
*/
void drv_led_is_on(dword_t pin)
{
    drv_led->led_is_on(pin);
}


