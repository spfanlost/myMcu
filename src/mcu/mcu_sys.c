

/**
* @file    sys.c
* @author  meng_yu
* @brief   system function realize
* @version 0.0.1
* @date    2020-09-13
*
* @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
*/
#include "common.h"
#include "mcu.h"
#include "mcu_sys.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
//PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
#define PLL_M (8U)

//USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
#define PLL_Q (7U)
#define PLL_N (336U)

//SYSCLK = PLL_VCO / PLL_P
#define PLL_P (2)

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
  *         AHB/APBx prescalers and Flash settings
  * @Note   This function should be called only once the RCC clock configuration
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC->CR |= ((dword_t) RCC_CR_HSEON); //Enable HSE
    while(! (RCC->CR&RCC_CR_HSERDY)); //Wait till HSE ready

    //Select regulator voltage output Scale 1 mode
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //HCLK = SYSCLK / 1

#if defined (STM32F407xx ) || defined(STM32F429xx)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //PCLK1 = HCLK / 4
#elif defined (STM32F401xE )
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; //PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //PCLK1 = HCLK / 4

#else

#error "pls define mcu type!!"
#endif

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1)-1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    RCC->CR |= RCC_CR_PLLON; //Enable the main PLL
    while((RCC->CR&RCC_CR_PLLRDY)==0); //Wait till the main PLL is ready

#if defined (STM32F429xx )

    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR&PWR_CSR_ODRDY)==0);
    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR&PWR_CSR_ODSWRDY)==0);

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;
#elif defined (STM32F407xx )

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;
#elif defined (STM32F401xE )

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_2WS;

#else

#error "pls define mcu type!!"
#endif

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (dword_t) (~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while((RCC->CFGR&(dword_t) RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
}


