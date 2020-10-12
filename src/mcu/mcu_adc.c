
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
#include "mcu.h"
#include "mcu_io.h"
#include "mcu_adc.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/
static volatile word_t ad_last; /* Last converted value               */
static volatile byte_t ad_done; /* AD conversion done flag            */

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
#define ADCx ADC1

void mcu_adc_clk_enable(dword_t adc_clk)
{
    RCC->APB2ENR |= adc_clk;
}

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void mcu_adc_init(void)
{
    /* Setup and initialize ADC converter                                 */
    mcu_io_clk_enable(GPIOC_CLK);
    mcu_adc_clk_enable(ADC1_CLK);
    GPIOC->MODER |= (3UL << 2*2); /* PC2 is in Analog mode              */
    ADCx->SQR1 = 0;
    ADCx->SQR2 = 0;
    ADCx->SQR3 = (12UL << 0); /* SQ1 = channel 12                   */
    ADCx->SMPR1 = (7UL << 6); /* Channel 12 sample time is 480 cyc. */
    ADCx->SMPR2 = 0; /* Clear register                     */
    ADCx->CR1 = (1UL << 8); /* Scan mode on                       */
    ADCx->CR2 |= (1UL << 3); /* Initialize calibration registers   */
    while(ADCx->CR2&(1UL << 3)); /* Wait for initialization to finish  */
    ADCx->CR2 |= (1UL << 2); /* Start calibration                  */
    while(ADCx->CR2&(1UL << 2)); /* Wait for calibration to finish     */
    ADCx->CR1 |= (1UL << 5); /* enable EOC interrupt               */
    NVIC_EnableIRQ(ADC_IRQn); /* enable ADC Interrupt               */
    ADCx->CR2 |= (1UL << 0); /* ADC enable                         */
}


/*-----------------------------------------------------------------------------
  Start AD Conversion
 *----------------------------------------------------------------------------*/
void mcu_adc_start_conv(void)
{
    ad_done = 0;
    ADCx->CR2 |= 1 << 30; /* Start conversion                   */
    while(!ad_done);
}

/*-----------------------------------------------------------------------------
  Get converted AD value
 *----------------------------------------------------------------------------*/
word_t mcu_adc_get_conv(void)
{
    return (ad_last);
}

/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void)
{
    if(ADCx->SR&(1 << 1)) /* ADC EOC interrupt?                 */
    {
        ad_last = (ADCx->DR&ADC_VALUE_MAX);
        ad_done = 1;
        ADCx->SR &=~(1 << 1); /* Clear EOC interrupt                */
    }
}


