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

#if defined (STM32F407xx)
#define HSE_VALUE ((uint32_t)8000000)
#define PLL_Q (7U)
#define PLL_N (336U)
#define PLL_M (8U)
#define PLL_P (2)
#elif defined (STM32F401xE)
#define HSE_VALUE ((uint32_t)16000000)
#define PLL_Q (7U)
#define PLL_N (336U)
#define PLL_M (16U)
#define PLL_P (4)
#elif defined (STM32H750xx)
#define HSE_VALUE ((uint32_t)16000000)
#define PLL_Q (2)
#define PLL_N (120)
#define PLL_M (1)
#define PLL_P (2)
#define PLL_R (2)
#else

#endif


#if defined (STM32H750xx)
#define PWR_FLAG_STOP       ((uint8_t)0x01U)
#define PWR_FLAG_SB_D1      ((uint8_t)0x02U)
#define PWR_FLAG_SB_D2      ((uint8_t)0x03U)
#define PWR_FLAG_SB         ((uint8_t)0x04U)
#define PWR_FLAG_PVDO       ((uint8_t)0x0BU)
#define PWR_FLAG_AVDO       ((uint8_t)0x0CU)
#define PWR_FLAG_ACTVOSRDY  ((uint8_t)0x0DU)
#define PWR_FLAG_ACTVOS     ((uint8_t)0x0EU)
#define PWR_FLAG_BRR        ((uint8_t)0x0FU)
#define PWR_FLAG_VOSRDY     ((uint8_t)0x10U)
#define PWR_FLAG_SCUEN      ((uint8_t)0x11U)
#define PWR_SUPPLY_CONFIG_MASK               (PWR_CR3_SCUEN | PWR_CR3_LDOEN | PWR_CR3_BYPASS)

#define __HAL_PWR_GET_FLAG(__FLAG__) ( \
((__FLAG__) == PWR_FLAG_PVDO)?((PWR->CSR1 & PWR_CSR1_PVDO) == PWR_CSR1_PVDO) : \
((__FLAG__) == PWR_FLAG_AVDO)?((PWR->CSR1 & PWR_CSR1_AVDO) == PWR_CSR1_AVDO) : \
((__FLAG__) == PWR_FLAG_ACTVOSRDY)?((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == PWR_CSR1_ACTVOSRDY) : \
((__FLAG__) == PWR_FLAG_VOSRDY)?((PWR->D3CR & PWR_D3CR_VOSRDY) == PWR_D3CR_VOSRDY) : \
((__FLAG__) == PWR_FLAG_SCUEN)?((PWR->CR3 & PWR_CR3_SCUEN) == PWR_CR3_SCUEN) : \
((__FLAG__) == PWR_FLAG_BRR)?((PWR->CR2 & PWR_CR2_BRRDY) == PWR_CR2_BRRDY) : \
((__FLAG__) == PWR_FLAG_SB)?((PWR->CPUCR & PWR_CPUCR_SBF) == PWR_CPUCR_SBF) : \
((__FLAG__) == PWR_FLAG_STOP)?((PWR->CPUCR & PWR_CPUCR_STOPF) == PWR_CPUCR_STOPF) : \
((__FLAG__) == PWR_FLAG_SB_D1)?((PWR->CPUCR & PWR_CPUCR_SBF_D1) == PWR_CPUCR_SBF_D1) : \
((PWR->CPUCR & PWR_CPUCR_SBF_D2) == PWR_CPUCR_SBF_D2))

#define PWR_REGULATOR_VOLTAGE_SCALE0  ((uint32_t)0x00000000)
#define PWR_REGULATOR_VOLTAGE_SCALE1  (PWR_D3CR_VOS_1 | PWR_D3CR_VOS_0)
#define PWR_REGULATOR_VOLTAGE_SCALE2  (PWR_D3CR_VOS_1)
#define PWR_REGULATOR_VOLTAGE_SCALE3  (PWR_D3CR_VOS_0)

#define __HAL_PWR_VOLTAGESCALING_CONFIG(__REGULATOR__)  \
do { \
    __IO uint32_t tmpreg = 0x00; \
    if((__REGULATOR__) == PWR_REGULATOR_VOLTAGE_SCALE0) \
    { \
      MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, PWR_REGULATOR_VOLTAGE_SCALE1); \
      /* Delay after setting the voltage scaling */  \
      tmpreg = READ_BIT(PWR->D3CR, PWR_D3CR_VOS); \
      MODIFY_REG(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN, SYSCFG_PWRCR_ODEN); \
      /* Delay after setting the syscfg boost setting */  \
      tmpreg = READ_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN); \
    } \
    else \
    { \
      CLEAR_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN); \
      /* Delay after setting the syscfg boost setting */  \
      tmpreg = READ_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN); \
      MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, (__REGULATOR__)); \
      tmpreg = READ_BIT(PWR->D3CR, PWR_D3CR_VOS); \
    } \
    UNUSED(tmpreg); \
} while(0)

/** @brief  Check RCC flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *            @arg RCC_FLAG_HSIDIV: HSI divider flag
  *            @arg RCC_FLAG_CSIRDY: CSI oscillator clock ready
  *            @arg RCC_FLAG_HSI48RDY: HSI48 oscillator clock ready
  *            @arg RCC_FLAG_HSERDY:  HSE oscillator clock ready
  *            @arg RCC_FLAG_D1CKRDY:  Domain1 clock ready
  *            @arg RCC_FLAG_D2CKRDY:  Domain2 clock ready
  *            @arg RCC_FLAG_PLLRDY:  PLL1 clock ready
  *            @arg RCC_FLAG_PLL2RDY: PLL2 clock ready
  *            @arg RCC_FLAG_PLL3RDY: PLL3 clock ready
  *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *            @arg RCC_FLAG_CPURST: CPU reset flag
  *            @arg RCC_FLAG_D1RST:  D1 domain power switch reset flag
  *            @arg RCC_FLAG_D2RST:  D2 domain power switch reset flag
  *            @arg RCC_FLAG_BORRST: BOR reset flag
  *            @arg RCC_FLAG_PINRST: Pin reset
  *            @arg RCC_FLAG_PORRST: POR/PDR  reset
  *            @arg RCC_FLAG_SFTRST: System reset from CPU reset flag
  *            @arg RCC_FLAG_BORRST:   D2 domain power switch reset flag
  *            @arg RCC_FLAG_IWDG1RST: CPU Independent Watchdog reset
  *            @arg RCC_FLAG_WWDG1RST: Window Watchdog1 reset
  *            @arg RCC_FLAG_LPWR1RST: Reset due to illegal D1 DSTANDBY or CPU CSTOP flag
  *            @arg RCC_FLAG_LPWR2RST: Reset due to illegal D2 DSTANDBY flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define RCC_FLAG_MASK  ((uint8_t)0x1F)
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((((__FLAG__) >> 5U) == 1U)? RCC->CR :((((__FLAG__) >> 5U) == 2U) ? RCC->BDCR : \
((((__FLAG__) >> 5U) == 3U)? RCC->CSR : ((((__FLAG__) >> 5U) == 4U)? RCC->RSR :RCC->CIFR))))  & (1UL << ((__FLAG__) & RCC_FLAG_MASK)))!= 0U)? 1U : 0U)

#define __HAL_RCC_PLL_ENABLE()         SET_BIT(RCC->CR, RCC_CR_PLL1ON)
#define __HAL_RCC_PLL_DISABLE()        CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON)

#define RCC_FLAG_PLLRDY                ((uint8_t)0x39)
#define RCC_PLLSOURCE_HSI              (0x00000000U)
#define RCC_PLLSOURCE_CSI              (0x00000001U)
#define RCC_PLLSOURCE_HSE              (0x00000002U)
#define RCC_PLLSOURCE_NONE             (0x00000003U)

#define RCC_PLL1VCIRANGE_0                RCC_PLLCFGR_PLL1RGE_0
#define RCC_PLL1VCIRANGE_1                RCC_PLLCFGR_PLL1RGE_1
#define RCC_PLL1VCIRANGE_2                RCC_PLLCFGR_PLL1RGE_2
#define RCC_PLL1VCIRANGE_3                RCC_PLLCFGR_PLL1RGE_3
#define RCC_PLL1VCOWIDE                 (0x00000000U)

#define RCC_PLL1_DIVP                RCC_PLLCFGR_DIVP1EN
#define RCC_PLL1_DIVQ                RCC_PLLCFGR_DIVQ1EN
#define RCC_PLL1_DIVR                RCC_PLLCFGR_DIVR1EN

#endif

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void SystemClock_Config(void);


#endif /* _SYS_H_ */


