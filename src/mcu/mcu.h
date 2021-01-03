/**
 * @file    mcu.h
 * @author  meng_yu
 * @brief   stm32 config function header file
 * @version 0.0.1
 * @date    2020-09-13
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_H_
#define _MCU_H_

//#include "stm32f1xx.h"

#if defined(STM32F407xx)||defined(STM32F429xx)||defined(STM32F401xE)
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#endif


/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/


#if defined(STM32F407xx)
#define USE_LCD_EN
#define DEVICE_STR "STM32F407xx"
#elif defined(STM32F429xx)
#define DEVICE_STR "STM32F429xx"
#elif defined(STM32F401xE)
#define DEVICE_STR "STM32F401xE"
#elif defined(STM32H750xx)
#include "stm32h7xx.h"
#define DEVICE_STR "STM32H750xx"
#endif


#endif /* _MCU_H_ */

