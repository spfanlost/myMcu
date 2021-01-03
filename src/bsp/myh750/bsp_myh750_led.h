
/**
 * @file    bsp_myh750_led.h
 * @author  meng_yu
 * @brief   myf401 board led function header file
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _BSP_MYH750_LED_H_
#define _BSP_MYH750_LED_H_

#define LEDR_PIN PIN3   //
#define LEDR_PORT GPIOB //

#define LEDG_PIN PIN3   //
#define LEDG_PORT GPIOB //

#define LEDB_PIN PIN3   //
#define LEDB_PORT GPIOB //

extern struct bsp_led bsp_myh750_led;

#endif /* _BSP_MYH750_LED_H_ */

