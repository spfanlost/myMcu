
/**
 * @file    mcu_adc.c
 * @author  meng_yu
 * @brief   low level ADC functions function realize
 * @version 0.0.1
 * @date    2020-09-10
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "stm32_config.h"
#include "mcu_adc.h"

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
word_t AD_last; /* Last converted value               */
byte_t AD_done; /* AD conversion done flag            */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
#define ADCx ADC1
void ADC_Init(void)
{
    /* Setup and initialize ADC converter                                 */
    RCC->APB2ENR |= (1UL << 8); /* Enable ADC1 clock                  */
    RCC->AHB1ENR |= (1UL << 2); /* Enable GPIOC clock                 */
    GPIOC->MODER |= (3UL << 2*2); /* PC2 is in Analog mode              */
    ADC1->SQR1 = 0;
    ADC1->SQR2 = 0;
    ADC1->SQR3 = (12UL << 0); /* SQ1 = channel 12                   */
    ADC1->SMPR1 = (7UL << 6); /* Channel 12 sample time is 480 cyc. */
    ADC1->SMPR2 = 0; /* Clear register                     */
    ADC1->CR1 = (1UL << 8); /* Scan mode on                       */
    ADC1->CR2 |= (1UL << 3); /* Initialize calibration registers   */
    while(ADC1->CR2&(1UL << 3)); /* Wait for initialization to finish  */
    ADC1->CR2 |= (1UL << 2); /* Start calibration                  */
    while(ADC1->CR2&(1UL << 2)); /* Wait for calibration to finish     */
    ADC1->CR1 |= (1UL << 5); /* enable EOC interrupt               */
    NVIC_EnableIRQ(ADC_IRQn); /* enable ADC Interrupt               */
    ADC1->CR2 |= (1UL << 0); /* ADC enable                         */
}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void)
{
    if(ADCx->SR&(1 << 1)) /* ADC EOC interrupt?                 */
    {
        AD_last = (ADCx->DR&ADC_VALUE_MAX);
        AD_done = 1;
        ADCx->SR &=~(1 << 1); /* Clear EOC interrupt                */
    }
}


/*-----------------------------------------------------------------------------
  Start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv(void)
{
    AD_done = 0;
    ADCx->CR2 |= 1 << 30; /* Start conversion                   */
}


/*-----------------------------------------------------------------------------
  Is AD conversion done
 *----------------------------------------------------------------------------*/
dword_t ADC_DoneCnv(void)
{
    return (AD_done);
}


/*-----------------------------------------------------------------------------
  Get converted AD value
 *----------------------------------------------------------------------------*/
word_t ADC_GetCnv(void)
{
    return (AD_last);
}


