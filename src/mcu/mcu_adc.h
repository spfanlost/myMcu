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
#define ADC_VALUE_MAX (0xFFF)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void ADC_Init(void);
extern void ADC_StartCnv(void);
extern dword_t ADC_DoneCnv(void);
extern word_t ADC_GetCnv(void);


#endif /* _MCU_ADC_H_ */


