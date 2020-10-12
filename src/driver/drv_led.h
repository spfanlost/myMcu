

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

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
#define LED1_PIN PIN9 //(PF9)
#define LED2_PIN PIN10 //(PF10)

extern void drv_led_init(void);
extern void drv_led_on(dword_t pin);
extern void drv_led_off(dword_t pin);

#endif /* _DRV_LED_H_ */

