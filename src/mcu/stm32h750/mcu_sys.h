/**
 * @file    mcu_sys.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-09-29
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_SYS_H_
#define _MCU_SYS_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
//PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
//USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
//SYSCLK = PLL_VCO / PLL_P

#define PLL_Q (7U)
#define PLL_N (336U)

#if defined (STM32F407xx)
#define HSE_VALUE ((uint32_t)8000000)
#define PLL_M (8U)
#define PLL_P (2)
#elif defined (STM32F401xE)
#define HSE_VALUE ((uint32_t)16000000)
#define PLL_M (16U)
#define PLL_P (4)
#else
#endif

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void SystemClock_Config(void);


#endif /* _SYS_H_ */


