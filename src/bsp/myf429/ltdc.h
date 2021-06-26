#ifndef __LTDC_H
#define __LTDC_H
#include "common.h"
#include "mcu.h"
#include "sdram.h"

#define LCD_PIXFORMAT_ARGB8888 (0x00000000)
#define LCD_PIXFORMAT_RGB888 (0x00000001)
#define LCD_PIXFORMAT_RGB565 (0x00000002)
#define LCD_PIXFORMAT_ARGB1555 (0x00000003)
#define LCD_PIXFORMAT_ARGB4444 (0x00000004)
#define LCD_PIXFORMAT_L8 (0x00000005)
#define LCD_PIXFORMAT_AL44 (0x00000006)
#define LCD_PIXFORMAT_AL88 (0x00000007)


#define LTDC_BlendingFactor1_CA ((uint32_t)0x00000400)
#define LTDC_BlendingFactor1_PAxCA ((uint32_t)0x00000600)
#define LTDC_BlendingFactor2_CA ((uint32_t)0x00000005)
#define LTDC_BlendingFactor2_PAxCA ((uint32_t)0x00000007)

#define RCC_AHB1Periph_DMA2D ((uint32_t)0x00800000)
#define DMA2D_R2M ((uint32_t)0x00030000)
#define DMA2D_ARGB8888 ((uint32_t)0x00000000)
#define DMA2D_RGB888 ((uint32_t)0x00000001)
#define DMA2D_RGB565 ((uint32_t)0x00000002)
#define DMA2D_ARGB1555 ((uint32_t)0x00000003)
#define DMA2D_ARGB4444 ((uint32_t)0x00000004)

#define DMA2D_FLAG_TC DMA2D_ISR_TCIF
#define LTDC_IMReload LTDC_SRCR_IMR /*!< Immediately Reload. */
#define LTDC_VBReload LTDC_SRCR_VBR /*!< Vertical Blanking Reload. */

/*
	LCD 颜色代码，CL_是Color的简写
	16Bit由高位至低位， RRRR RGGG GGGB BBBB

	下面的RGB 宏将24位的RGB值转换为16位格式。
	启动windows的画笔程序，点击编辑颜色，选择自定义颜色，可以获得的RGB值。

	推荐使用迷你取色器软件获得你看到的界面颜色。
*/
#define RGB(R, G, B) (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3)) /* 将8位R,G,B转化为 16位RGB565格式 */
#define ARGB(A, R, G, B) ((A << 24) | (R << 16) | (G << 8) | (B << 0))

/* 解码出 R=8bit G=8bit B=8bit */
#define RGB565_R(x) ((x >> 8) & 0xF8)
#define RGB565_G(x) ((x >> 3) & 0xFC)
#define RGB565_B(x) ((x << 3) & 0xF8)

/* 解码出 R=5bit G=6bit B=5bit */
#define RGB565_R2(x) ((x >> 11) & 0x1F)
#define RGB565_G2(x) ((x >> 5) & 0x3F)
#define RGB565_B2(x) ((x >> 0) & 0x1F)

enum
{
    CL_WHITE = RGB(255, 255, 255), /* 白色 */
    CL_BLACK = RGB(0, 0, 0),       /* 黑色 */
    CL_RED = RGB(255, 0, 0),       /* 红色 */
    CL_GREEN = RGB(0, 255, 0),     /* 绿色 */
    CL_BLUE = RGB(0, 0, 255),      /* 蓝色 */
    CL_YELLOW = RGB(255, 255, 0),  /* 黄色 */

    ARGB_WHITE = ARGB(0, 255, 255, 255), /* 白色 */
    ARGB_BLACK = ARGB(0, 0, 0, 0),       /* 黑色 */
    ARGB_RED = ARGB(0, 255, 0, 0),       /* 红色 */
    ARGB_GREEN = ARGB(0, 0, 255, 0),     /* 绿色 */
    ARGB_BLUE = ARGB(0, 0, 0, 255),      /* 蓝色 */
    ARGB_YELLOW = ARGB(0, 255, 255, 0),  /* 黄色 */

    
    CL_GREY = RGB(98, 98, 98),     /* 深灰色 */
    CL_GREY1 = RGB(150, 150, 150), /* 浅灰色 */
    CL_GREY2 = RGB(180, 180, 180), /* 浅灰色 */
    CL_GREY3 = RGB(200, 200, 200), /* 最浅灰色 */
    CL_GREY4 = RGB(230, 230, 230), /* 最浅灰色 */

    CL_BUTTON_GREY = RGB(220, 220, 220), /* WINDOWS 按钮表面灰色 */

    CL_MAGENTA = 0xF81F, /* 红紫色，洋红色 */
    CL_CYAN = 0x7FFF,    /* 蓝绿色，青色 */
    ARGB_MAGENTA = ARGB(0, RGB565_R(CL_MAGENTA), RGB565_G(CL_MAGENTA), RGB565_B(CL_MAGENTA)), /* 红紫色，洋红色 */
    ARGB_CYAN = ARGB(0, RGB565_R(CL_CYAN), RGB565_G(CL_CYAN), RGB565_B(CL_CYAN)),    /* 蓝绿色，青色 */

    CL_BLUE1 = RGB(0, 0, 240),   /* 深蓝色 */
    CL_BLUE2 = RGB(0, 0, 128),   /* 深蓝色 */
    CL_BLUE3 = RGB(68, 68, 255), /* 浅蓝色1 */
    CL_BLUE4 = RGB(0, 64, 128),  /* 浅蓝色1 */

    /* UI 界面 Windows控件常用色 */
    CL_BTN_FACE = RGB(236, 233, 216), /* 按钮表面颜色(灰) */

    CL_BTN_FONT = CL_BLACK, /* 按钮字体颜色（黑） */

    CL_BOX_BORDER1 = RGB(172, 168, 153), /* 分组框主线颜色 */
    CL_BOX_BORDER2 = RGB(255, 255, 255), /* 分组框阴影线颜色 */

    CL_MASK = 0x9999 /* 颜色掩码，用于文字背景透明 */
};

#define LCD_LAYER_1 0x0000 /* 顶层 */
#define LCD_LAYER_2 0x0001 /* 第2层 */
#define LCD429_FRAME_BUFFER (SDRAM_BANK_ADDR)//

#define LCD_PIXFORMAT LCD_PIXFORMAT_ARGB8888//LCD_PIXFORMAT_RGB565//

/* Physical LCD Timings */
#define HBP 42
#define VBP 11
#define HSW 40
#define VSW 9
#define HFP 2
#define VFP 2

/* Number of multiple buffers to be used */
#define NUM_BUFFERS  1 
/* Number of virtual screens to be used */
#define NUM_VSCREENS 1

#define GUI_NUM_LAYERS            2    // Maximum number of available layers

#if(LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888)
#define BUFFER_OFFSET (uint32_t)(480 * 272 * sizeof(uint32_t) * NUM_VSCREENS)
#else
#define BUFFER_OFFSET (uint32_t)(480 * 272 * sizeof(uint16_t) * NUM_VSCREENS)
#endif

void LTDC_LayerInit(LTDC_Layer_TypeDef *LTDC_Layerx);
void LCD429_ConfigLTDC(void);
void LCD429_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint32_t _usColor);
void LCD429_SetLayer(uint8_t _ucLayer);
void LTDC_LayerPixelFormat(LTDC_Layer_TypeDef *LTDC_Layerx, uint32_t PixelFormat);
void LCD429_SetPixelFormat(uint32_t PixelFormat);
uint8_t Get_PixelFormat_byte(uint32_t PixelFormat);
void LCD429_ClrScr(uint32_t _usColor);
void LTDC_ReloadConfig(uint32_t LTDC_Reload);
void LTDC_LayerAddress(LTDC_Layer_TypeDef* LTDC_Layerx, uint32_t Address);
void LTDC_ITConfig(uint32_t LTDC_IT, FunctionalState NewState);
void LTDC_CLUTCmd(LTDC_Layer_TypeDef *LTDC_Layerx, FunctionalState NewState);
void LTDC_LayerCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState);
void LTDC_Cmd(FunctionalState NewState);
void LCD_Clear(u8 layer, u32 color);

void LCD429_InitHard(void); //LTDC初始化函数

#endif
