

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

#if defined (STM32H750xx)
void HAL_PWREx_ConfigSupply(uint32_t SupplySource)
{
    if(!__HAL_PWR_GET_FLAG(PWR_FLAG_SCUEN))
    {
        if((PWR->CR3 & PWR_SUPPLY_CONFIG_MASK) != SupplySource)
        {
            return;
        }
    }

    /* Set the power supply configuration */
    MODIFY_REG(PWR->CR3, PWR_SUPPLY_CONFIG_MASK, SupplySource);

    /* Wait till voltage level flag is set */
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ACTVOSRDY));
}
#endif

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

#if defined (STM32H750xx)
    //Supply configuration update enable
    HAL_PWREx_ConfigSupply(PWR_CR3_LDOEN);
    //Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
#endif

    SET_BIT(RCC->CR, RCC_CR_HSEON);//Enable HSE
    while(! (RCC->CR&RCC_CR_HSERDY)); //Wait till HSE ready

#if !defined (STM32H750xx )
    //Select regulator voltage output Scale 1 mode
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //HCLK = SYSCLK / 1
#endif

#if defined (STM32F407xx ) || defined(STM32F429xx)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //PCLK1 = HCLK / 4
#elif defined (STM32F401xE )
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; //PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //PCLK1 = HCLK / 4
#else
// #error "pls define mcu type!!"
#endif

    /* Configure the main PLL */
#if !defined (STM32H750xx )
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1)-1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    RCC->CR |= RCC_CR_PLLON; //Enable the main PLL
    while((RCC->CR&RCC_CR_PLLRDY)==0); //Wait till the main PLL is ready
#else
    CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != 0U);
    MODIFY_REG(RCC->PLLCKSELR, (RCC_PLLCKSELR_PLLSRC | RCC_PLLCKSELR_DIVM1) , ((RCC_PLLSOURCE_HSE) | ( PLL_M <<4U)));
    RCC->PLL1DIVR = ( ((PLL_N - 1U )& RCC_PLL1DIVR_N1) | (((PLL_P -1U ) << 9U) & RCC_PLL1DIVR_P1) | 
    (((PLL_Q -1U) << 16U)& RCC_PLL1DIVR_Q1) | (((PLL_R - 1U) << 24U)& RCC_PLL1DIVR_R1));
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN);
    MODIFY_REG(RCC->PLL1FRACR, RCC_PLL1FRACR_FRACN1, (uint32_t)(0) << RCC_PLL1FRACR_FRACN1_Pos);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1RGE, (RCC_PLL1VCIRANGE_3));
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1VCOSEL, (RCC_PLL1VCOWIDE));
    SET_BIT(RCC->PLLCFGR, (RCC_PLL1_DIVP));
    SET_BIT(RCC->PLLCFGR, (RCC_PLL1_DIVQ));
    SET_BIT(RCC->PLLCFGR, (RCC_PLL1_DIVR));
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN);
    SET_BIT(RCC->CR, RCC_CR_PLL1ON);
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == 0U);
#endif

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

#elif defined (STM32H750xx)

#else

#error "pls define mcu type!!"

#endif

#if !defined (STM32H750xx )
    /* Select the main PLL as system clock source */
    RCC->CFGR &= (dword_t) (~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while((RCC->CFGR&(dword_t) RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
#endif
}


