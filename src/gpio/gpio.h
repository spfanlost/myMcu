

/**
 * @file    gpio.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-09-07
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _GPIO_H_
#define _GPIO_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define FTIR (1)     //下降沿触发
#define RTIR (2)     //上升沿触发

//GPIO设置专用宏定义
#define GPIO_MODE_IN (0)     //普通输入模式
#define GPIO_MODE_OUT (1)     //普通输出模式
#define GPIO_MODE_AF (2)     //AF功能模式
#define GPIO_MODE_AIN (3)     //模拟输入模式
#define GPIO_SPEED_2M (0)     //GPIO速度2Mhz
#define GPIO_SPEED_25M (1)     //GPIO速度25Mhz
#define GPIO_SPEED_50M (2)     //GPIO速度50Mhz
#define GPIO_SPEED_100M (3)     //GPIO速度100Mhz
#define GPIO_PUPD_NONE (0)     //不带上下拉
#define GPIO_PUPD_PU (1)     //上拉
#define GPIO_PUPD_PD (2)     //下拉
#define GPIO_PUPD_RES (3)     //保留
#define GPIO_OTYPE_PP (0)     //推挽输出
#define GPIO_OTYPE_OD (1)     //开漏输出

//GPIO引脚编号定义
#define PIN0 (1<<0)
#define PIN1 (1<<1)
#define PIN2 (1<<2)
#define PIN3 (1<<3)
#define PIN4 (1<<4)
#define PIN5 (1<<5)
#define PIN6 (1<<6)
#define PIN7 (1<<7)
#define PIN8 (1<<8)
#define PIN9 (1<<9)
#define PIN10 (1<<10)
#define PIN11 (1<<11)
#define PIN12 (1<<12)
#define PIN13 (1<<13)
#define PIN14 (1<<14)
#define PIN15 (1<<15)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void GPIO_Set(GPIO_TypeDef*GPIOx,
    uint32_t PINx,
    uint32_t MODE,
    uint32_t OTYPE,
    uint32_t OSPEED,
    uint32_t PUPD);
void GPIO_AF_Set(GPIO_TypeDef*GPIOx, uint8_t PINx, uint8_t AFx);

#endif /* _GPIO_H_ */

