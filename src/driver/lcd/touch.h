#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "common.h"
#include "mcu.h"
#include "mcu_io.h"

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define CT_MAX_TOUCH  5    //电容屏支持的点数,固定为5点


//按键状态	 
#define Key_Down 0x80
#define TP_Key_Down 0x40
#define Key_Up   0x00 


/* 液晶坐标结构体 */
typedef	struct POINT 
{
   uint16_t x;		
   uint16_t y;
}Coordinate;

//笔杆结构体
typedef struct 
{
	u16 X0;//原始坐标
	u16 Y0;
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态			  
}Pen_Parameters;	

//触摸屏校准参数
typedef struct 
{
float xfac;
float yfac;
short xoff;
short yoff;

}Touch_Adjust_Parameters;


extern Touch_Adjust_Parameters Touch_date;
extern Pen_Parameters Pen_data;


//与触摸屏芯片连接引脚	   
#define TS_DRAW_RANGE 50 
#define PEN mcu_io_pin_val(GPIOB,PIN1)   //PC1  INT
#define DOUT mcu_io_pin_val(GPIOB,PIN2)   //PC2  MISO 
#define TDIN_Low mcu_io_reset(GPIOF,PIN11)  //PC3  MOSI
#define TDIN_High mcu_io_set(GPIOF,PIN11)  //PC3  MOSI
#define TCLK_Low mcu_io_reset(GPIOB,PIN0)  //PC0  SCLK
#define TCLK_High mcu_io_set(GPIOB,PIN0)  //PC0  SCLK   
#define TCS_Low mcu_io_reset(GPIOC,PIN13) //PC13 CS
#define TCS_High mcu_io_set(GPIOC,PIN13) //PC13 CS
 
//电阻屏函数
void TP_Write_Byte(u8 num);						//向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);							//读取AD转换值
u16 TP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//双方向读取(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标校准点
void TP_Save_Adjdata(void);						//保存校准参数
u8 TP_Get_Adjdata(void);						//读取校准参数
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//显示校准信息
//电阻屏/电容屏 共用函数
u8 Read_TP(u8 tp);							//扫描
 void TP_Init(void);
 void Draw_Big_Circle(u8 x,u16 y,u16 Color);
#endif

















