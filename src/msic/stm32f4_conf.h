/**
 * @file    stm32f4_conf.h
 * @author  meng_yu
 * @brief   stm32f4_configure function header file
 * @version 0.0.1
 * @date    2020-09-08
 * 
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _STM32F4_CONF_H_
#define _STM32F4_CONF_H_
#include "stm32f407xx.h"

//#define USE_LCD_EN

#define STM32F40_41xxx

#ifdef STM32F40_41xxx
#include "stm32f407xx.h"
#define DEVICE_STR "STM32F407XX"
#endif


#endif /* _STM32F4_CONF_H_ */


