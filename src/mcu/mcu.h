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

#if defined(STM32F407xx)||defined(STM32F429xx)||defined(STM32F401xE)
    #include "stm32f4xx.h"
    #include "system_stm32f4xx.h"
    
    #if defined(STM32F407xx)
    #define USE_LCD_EN
    #define DEVICE_STR "STM32F407xx"
    #elif defined(STM32F429xx)
    #define DEVICE_STR "STM32F429xx"
    #elif defined(STM32F401xE)
    #define DEVICE_STR "STM32F401xE"
    #endif
    
    #include "stm32f4xx_ll_rcc.h"
    #include "stm32f4xx_ll_bus.h"
    #include "stm32f4xx_ll_system.h"
    #include "stm32f4xx_ll_exti.h"
    #include "stm32f4xx_ll_cortex.h"
    #include "stm32f4xx_ll_utils.h"
    #include "stm32f4xx_ll_pwr.h"
    #include "stm32f4xx_ll_dma.h"
    #include "stm32f4xx_ll_usart.h"
    #include "stm32f4xx_ll_gpio.h"
    #include "stm32f4xx_ll_dma2d.h"


#elif defined(STM32H750xx)
    #include "stm32h7xx.h"
    #include "stm32h7xx_ll_rcc.h"
    #include "stm32h7xx_ll_crs.h"
    #include "stm32h7xx_ll_bus.h"
    #include "stm32h7xx_ll_system.h"
    #include "stm32h7xx_ll_exti.h"
    #include "stm32h7xx_ll_cortex.h"
    #include "stm32h7xx_ll_utils.h"
    #include "stm32h7xx_ll_pwr.h"
    #include "stm32h7xx_ll_dma.h"
    #include "stm32h7xx_ll_usart.h"
    #include "stm32h7xx_ll_gpio.h"
    
    #define DEVICE_STR "STM32H750xx"
    
#endif


/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority
                                                      4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority
                                                      3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority
                                                      2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority
                                                      1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
                                                      0 bits for subpriority */

#endif /* _MCU_H_ */

