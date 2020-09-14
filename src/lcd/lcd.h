

/**
 * @file    lcd.h
 * @author  meng_yu
 * @brief   Graphic LCD function prototypes and defines function header file
 * @version 0.0.1
 * @date    2020-09-14
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _LCD_H_
#define _LCD_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  Color coding
  GLCD is coded:   15..11 red, 10..5 green, 4..0 blue  (unsigned short)  GLCD_R5, GLCD_G6, GLCD_B5
  original coding: 17..12 red, 11..6 green, 5..0 blue                    ORG_R6,  ORG_G6,  ORG_B6

  ORG_R1..5 = GLCD_R0..4,  ORG_R0 = GLCD_R4
  ORG_G0..5 = GLCD_G0..5,
  ORG_B1..5 = GLCD_B0..4,  ORG_B0 = GLCD_B4
 *----------------------------------------------------------------------------*/

/* GLCD RGB color definitions                                                 */
#define Black 0x0000     /*   0,   0,   0 */
#define Navy 0x000F      /*   0,   0, 128 */
#define DarkGreen 0x03E0 /*   0, 128,   0 */
#define DarkCyan 0x03EF  /*   0, 128, 128 */
#define Maroon 0x7800    /* 128,   0,   0 */
#define Purple 0x780F    /* 128,   0, 128 */
#define Olive 0x7BE0     /* 128, 128,   0 */
#define LightGrey 0xC618 /* 192, 192, 192 */
#define DarkGrey 0x7BEF  /* 128, 128, 128 */
#define Blue 0x001F      /*   0,   0, 255 */
#define Green 0x07E0     /*   0, 255,   0 */
#define Cyan 0x07FF      /*   0, 255, 255 */
#define Red 0xF800       /* 255,   0,   0 */
#define Magenta 0xF81F   /* 255,   0, 255 */
#define Yellow 0xFFE0    /* 255, 255, 0   */
#define White 0xFFFF     /* 255, 255, 255 */

//Orientation  configuration
#define LANDSCAPE 1 /* 1 for landscape, 0 for portrait    */
#define ROTATE180 0 /* 1 to rotate the screen for 180 deg */

//Graphic LCD size definitions
#if ( LANDSCAPE == 1)
#define WIDTH 320  /* Screen Width (in pixels)           */
#define HEIGHT 240 /* Screen Hight (in pixels)           */

#else

#define WIDTH 240  /* Screen Width (in pixels)           */
#define HEIGHT 320 /* Screen Hight (in pixels)           */
#endif

#define BPP 16               /* Bits per pixel                     */
#define BYPP ((BPP + 7) / 8) /* Bytes per pixel                    */

//Graphic LCD interface hardware definitions
struct lcd_t
{
__IO uint16_t cmd;
__IO uint16_t dat;
};


/* LCD /CS is NE4 - Bank 4 of NOR/SRAM Bank 1~4                               */
//#define LCD_BASE (0x60000000UL | 0x0C000000UL)
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线
//注意设置时STM32内部会右移一位对其! 111 1110=0X7E
#define LCD_BASE ((uint32_t)(0x6C000000 | 0x0000007E))
#define reg_lcd ((volatile struct lcd_t *) LCD_BASE)
#define BG_COLOR 0  /* Background color                   */
#define TXT_COLOR 1 /* Text color                         */

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void GLCD_Init(void);
extern void GLCD_WindowMax(void);
extern void GLCD_PutPixel(unsigned int x, unsigned int y);
extern void GLCD_SetTextColor(unsigned short color);
extern void GLCD_SetBackColor(unsigned short color);
extern void GLCD_Clear(unsigned short color);
extern void GLCD_DrawChar(unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char*c);
extern void GLCD_DisplayChar(unsigned int ln, unsigned int col, unsigned char fi, unsigned char c);
extern void GLCD_DisplayString(unsigned int ln, unsigned int col, unsigned char fi, unsigned char*s);
extern void GLCD_ClearLn(unsigned int ln, unsigned char fi);
extern void GLCD_Bargraph(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val);
extern void GLCD_Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char*bitmap);
extern void GLCD_ScrollVertical(unsigned int dy);
extern void GLCD_WrCmd(unsigned char cmd);
extern void GLCD_WrReg(unsigned char reg, unsigned short val);
#endif /* _LCD_H_ */

