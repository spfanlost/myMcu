/**
 * @file    bus_lcd.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-10-23
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _BUS_LCD_H_
#define _BUS_LCD_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
struct bus_lcd
{
    enum bus_lcd_type bus_type;
    dword_t (*write_cmd     )(word_t);
    dword_t (*write_data    )(byte_t *, dword_t);
    dword_t (*read_data     )(byte_t *, dword_t);
};

//LCD地址结构体
typedef struct
{
    vu16 LCD_REG;
    vu16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE        MYF407_LCD_BASE
#define LCD             ((LCD_TypeDef *) LCD_BASE)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/


#endif /* _BUS_LCD_H_ */


