#ifndef __BSP_LCD9325_H
#define __BSP_LCD9325_H

#include "stm32f4xx.h"
#include "stdlib.h"
      
#define Use_Horizontal  0//定义是否使用横屏         0,不使用.1,使用.
#if Use_Horizontal==1
    //uCGUI
    #ifndef LCD_XSIZE
    #define LCD_XSIZE (320) 
    #endif
    #ifndef LCD_YSIZE
    #define LCD_YSIZE (240) 
    #endif
    //emWin
    #ifndef XSIZE_PHYS
    #define XSIZE_PHYS (320)    
    #endif
    #ifndef YSIZE_PHYS
    #define YSIZE_PHYS (240)    
    #endif

    #define MAX_CHAR_POSX 312
    #define MAX_CHAR_POSY 232 

    #define L2R_U2D  (0<<5)|(1<<4)|(1<<3);  //从左到右,从上到下
    #define L2R_D2U  (0<<5)|(0<<4)|(1<<3); //从左到右,从下到上
    #define R2L_U2D  (0<<5)|(0<<4)|(0<<3);//从右到左,从上到下
    #define R2L_D2U  (1<<5)|(0<<4)|(1<<3); //从右到左,从下到上
    #define U2D_L2R  (0<<5)|(1<<4)|(0<<3); //从上到下,从左到右
    #define U2D_R2L  (1<<5)|(1<<4)|(0<<3);//从上到下,从右到左
    #define D2U_L2R  (0<<5)|(0<<4)|(0<<3); //从下到上,从左到右
    #define D2U_R2L  (1<<5)|(0<<4)|(0<<3); //从下到上,从右到左
#else     
    //uCGUI
    #ifndef LCD_XSIZE
    #define LCD_XSIZE (240) 
    #endif
    #ifndef LCD_YSIZE
    #define LCD_YSIZE (320) 
    #endif
    //emWin
    #ifndef XSIZE_PHYS
    #define XSIZE_PHYS (240)    
    #endif
    #ifndef YSIZE_PHYS
    #define YSIZE_PHYS (320)    
    #endif
    
    #define MAX_CHAR_POSX 232
    #define MAX_CHAR_POSY 312
    
    #define L2R_U2D  (1<<5)|(1<<4)|(0<<3);  //从左到右,从上到下
    #define L2R_D2U  (0<<5)|(1<<4)|(0<<3); //从左到右,从下到上
    #define R2L_U2D  (1<<5)|(0<<4)|(0<<3);//从右到左,从上到下
    #define R2L_D2U  (0<<5)|(0<<4)|(0<<3); //从右到左,从下到上
    #define U2D_L2R  (0<<5)|(0<<4)|(0<<3); //从上到下,从左到右
    #define U2D_R2L  (1<<5)|(0<<4)|(1<<3);//从上到下,从右到左
    #define D2U_L2R  (0<<5)|(1<<4)|(1<<3); //从下到上,从左到右
    #define D2U_R2L  (0<<5)|(0<<4)|(1<<3); //从下到上,从右到左
#endif

//LCD地址结构体
typedef struct
{
    __IO uint16_t LCD_REG;
    __IO uint16_t LCD_RAM;
} LCD_TypeDef;


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111 1110=0X7E                
#define LCD_BASE        ((uint32_t)(0x6C000000 | 0x0000007E))
#define LCDYM             ((LCD_TypeDef *) LCD_BASE)

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //棕色
#define BRRED 0XFC07 //棕红色
#define GRAY 0X8430 //灰色
#define DARKBLUE 0X01CF //深蓝色
#define LIGHTBLUE 0X7D7C    //浅蓝色  
#define GRAYBLUE 0X5458 //灰蓝色
#define LIGHTGREEN 0X841F //浅绿色
#define LGRAY 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_WR_REG(uint16_t regval);
void LCD_WR_DATA(uint16_t data);
uint16_t LCD_RD_DATA(void);
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t Color);
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_Display(uint8_t off_on);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Scan_Dir(void);
void LCDx_Init(void);
void LCD_Clear(uint16_t color);

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t Color);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t chr,uint16_t Back_Color,uint16_t Color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *pstr,uint16_t Back_Color,uint16_t Color);

#endif


