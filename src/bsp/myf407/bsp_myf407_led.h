/**
 * @file    bsp_myf407_led.h
 * @author  meng_yu
 * @brief   myf407 board led function header file
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _BSP_MYF407_LED_H_
#define _BSP_MYF407_LED_H_

#define LED1_PIN PIN9 //(PF9)
#define LED1_PORT GPIOF //

#define LED2_PIN PIN10 //(PF10)
#define LED2_PORT GPIOF //


extern struct bsp_led bsp_myf407_led;

#endif /* _BSP_MYF407_LED_H_ */

