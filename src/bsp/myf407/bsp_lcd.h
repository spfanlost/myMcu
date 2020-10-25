/**
 * @file    bsp_myf407_lcd.h
 * @author  meng_yu
 * @brief   myf407 board lcd function header file
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _BSP_MYF407_LCD_H_
#define _BSP_MYF407_LCD_H_

enum bus_lcd_type
{
    BUS_LCD_SPI = 0,
    BUS_LCD_I2C ,
    BUS_LCD_16B ,
};

struct bsp_lcd
{
    enum bus_lcd_type bus_type;
    void    (*lcd_bus_init  )(void);
    dword_t (*lcd_bl        )(byte_t);
    dword_t (*write_cmd     )(word_t);
    dword_t (*write_data    )(byte_t *, dword_t);
    dword_t (*read_data     )(byte_t *, dword_t);
};

#endif /* _BSP_MYF407_LCD_H_ */

