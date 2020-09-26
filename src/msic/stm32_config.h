/**
 * @file    stm32_config.h
 * @author  meng_yu
 * @brief   stm32 config function header file
 * @version 0.0.1
 * @date    2020-09-13
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _STM32_CONFIG_H_
#define _STM32_CONFIG_H_

//#include "stm32f1xx.h"
#include "stm32f4xx.h"
//#include "stm32h7xx.h"

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
#define DEVICE_STR "STM32H750xx"
#endif

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void SystemClock_Config(void);

#endif /* _STM32_CONFIG_H_ */

