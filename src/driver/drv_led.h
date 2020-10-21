

/**
 * @file    drv_led.h
 * @author  meng_yu
 * @brief   Low level LED definitions function header file
 * @version 0.0.1
 * @date    2020-09-10
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _DRV_LED_H_
#define _DRV_LED_H_

#include "mcu_io.h"
#if defined(STM32F407xx)
#include "bsp_myf407_led.h"
#elif defined(STM32F401xE)
#include "bsp_myf401_led.h"
#else

#endif



/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/
struct bsp_led
{
    dword_t (*led_init      )(void);
    dword_t (*led_on        )(dword_t);
    dword_t (*led_off       )(dword_t);
    dword_t (*led_toggle    )(dword_t);
    dword_t (*led_is_on     )(dword_t);
};

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void drv_led_init(void);
extern void drv_led_on(dword_t pin);
extern void drv_led_off(dword_t pin);
extern void drv_led_toggle(dword_t pin);
extern void drv_led_is_on(dword_t pin);

#endif /* _DRV_LED_H_ */

