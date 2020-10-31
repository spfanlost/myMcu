#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "common.h"
#include "mcu.h"
#include "mcu_io.h"


//按键状态
#define Key_Down 0x80U
#define TP_Key_Down 0x40U
#define Key_Up   0x00U


/* 液晶坐标结构体 */
typedef struct POINT
{
   uint16_t x;
   uint16_t y;
}Coordinate;

//笔杆结构体
struct touch_pen_param
{
    uint16_t X0;//原始坐标
    uint16_t Y0;
    uint16_t X; //最终/暂存坐标
    uint16_t Y;
    uint8_t  Key_Sta;//笔的状态
};

//触摸屏校准参数
struct touch_dat_param
{
    float xfac;
    float yfac;
    short xoff;
    short yoff;
};

extern struct touch_dat_param touch_dat;
extern struct touch_pen_param pen_dat;


//与触摸屏芯片连接引脚
#define TS_DRAW_RANGE 50
#define PEN mcu_io_pin_val(GPIOB,PIN1)   //PB1  INT
#define DOUT mcu_io_pin_val(GPIOB,PIN2)   //PC2  MISO
#define TDIN_Low mcu_io_reset(GPIOF,PIN11)  //PC3  MOSI
#define TDIN_High mcu_io_set(GPIOF,PIN11)  //PC3  MOSI
#define TCLK_Low mcu_io_reset(GPIOB,PIN0)  //PC0  SCLK
#define TCLK_High mcu_io_set(GPIOB,PIN0)  //PC0  SCLK
#define TCS_Low mcu_io_reset(GPIOC,PIN13) //PC13 CS
#define TCS_High mcu_io_set(GPIOC,PIN13) //PC13 CS

void touch_write_byte(uint8_t dat);
uint16_t touch_read_ad(uint8_t cmd);
uint16_t touch_dead_xory(uint8_t xy);
uint8_t touch_read_xy(uint16_t *x,uint16_t *y);
uint8_t touch_read_xy2(uint16_t *x,uint16_t *y);
uint8_t touch_scan(uint8_t tp);
void touch_init(void);
void touch_draw_point(uint8_t x,uint16_t y,uint16_t Color);
void touch_load_draw_ui(void);

#endif

















