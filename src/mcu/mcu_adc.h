/**
 * @file    mcu_adc.h
 * @author  meng_yu
 * @brief   low level ADC definitions function header file
 * @version 0.0.1
 * @date    2020-10-11
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_ADC_H_
#define _MCU_ADC_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define ADC1_CLK (0x1U<<8)
#define ADC2_CLK (0x1U<<9)
#define ADC3_CLK (0x1U<<10)

#define ADC_VALUE_MAX (0xFFF)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void mcu_adc_clk_enable(dword_t adc_clk);
extern void mcu_adc_init(void);
extern void mcu_adc_start_conv(void);
extern dword_t ADC_DoneCnv(void);
extern word_t mcu_adc_get_conv(void);


#endif /* _MCU_ADC_H_ */


