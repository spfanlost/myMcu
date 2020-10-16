

/**
 * @file    drv_lcd.c
 * @author  meng_yu
 * @brief   low level Graphic LCD (320x240 pixels) with 16-bit parallel interface realize
 * @version 0.0.1
 * @date    2020-09-14
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_io.h"
#include "drv_lcd.h"
#include "drv_ili9325.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

static uint16_t   foreground_color = GLCD_COLOR_BLACK;
static uint16_t   background_color = GLCD_COLOR_WHITE;
static GLCD_FONT *active_font      = NULL;
static uint8_t    Himax;
#if (GLCD_SWAP_XY == 0)
static uint32_t   Scroll;
#endif

struct lcd_drv *drv_lcd;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

static void delay_ms (int ms) {
  ms *= (SystemCoreClock/10000);
  while (ms--) { __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); }
}

static __inline void wr_dat_start(void)
{
    /* only used for SPI interface */
}

static __inline void wr_dat_stop(void)
{
    /* only used for SPI interface */
}

/*-----------------------------------------------------------------------------------
  Initialize the Graphic LCD controller functions definition
-----------------------------------------------------------------------------------*/
void GLCD_Init(void)
{
    word_t driverCode;
    drv_lcd = &drv_ili9325;
    drv_lcd->bus_init();
    drv_lcd->bl_init();
    delay_ms(50); /* delay_ms 50 ms                        */
    driverCode = drv_lcd->bus_rd_reg(0x00);
    LOG_INFO("lcd_code:%x\r\n", driverCode);
    Himax = 0; /* This is not Himax LCD controller   */
    /* Start Initial Sequence ------------------------------------------------*/
#if (GLCD_MIRROR_X)
    drv_lcd->bus_wr_reg(0x01, 0x0000);               /* Clear SS bit                       */
#else
    drv_lcd->bus_wr_reg(0x01, 0x0100);               /* Set SS bit                         */
#endif
    drv_lcd->bus_wr_reg(0x02, 0x0700);               /* Set 1 line inversion               */
    drv_lcd->bus_wr_reg(0x04, 0x0000);               /* Resize register                    */
    drv_lcd->bus_wr_reg(0x08, 0x0207);               /* 2 lines front, 7 back porch        */
    drv_lcd->bus_wr_reg(0x09, 0x0000);               /* Set non-disp area refresh cyc ISC  */
    drv_lcd->bus_wr_reg(0x0A, 0x0000);               /* FMARK function                     */
    drv_lcd->bus_wr_reg(0x0C, 0x0000);               /* RGB interface setting              */
    drv_lcd->bus_wr_reg(0x0D, 0x0000);               /* Frame marker Position              */
    drv_lcd->bus_wr_reg(0x0F, 0x0000);               /* RGB interface polarity             */

    /* Power On sequence -----------------------------------------------------*/
    drv_lcd->bus_wr_reg(0x10, 0x0000);               /* Reset Power Control 1              */
    drv_lcd->bus_wr_reg(0x11, 0x0000);               /* Reset Power Control 2              */
    drv_lcd->bus_wr_reg(0x12, 0x0000);               /* Reset Power Control 3              */
    drv_lcd->bus_wr_reg(0x13, 0x0000);               /* Reset Power Control 4              */
    delay_ms(200);                      /* Discharge cap power voltage (200ms)*/
    drv_lcd->bus_wr_reg(0x10, 0x12B0);               /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
    drv_lcd->bus_wr_reg(0x11, 0x0007);               /* DC1[2:0], DC0[2:0], VC[2:0]        */
    delay_ms(50);                       /* Delay 50 ms                        */
    drv_lcd->bus_wr_reg(0x12, 0x01BD);               /* VREG1OUT voltage                   */
    delay_ms(50);                       /* Delay 50 ms                        */
    drv_lcd->bus_wr_reg(0x13, 0x1400);               /* VDV[4:0] for VCOM amplitude        */
    drv_lcd->bus_wr_reg(0x29, 0x000E);               /* VCM[4:0] for VCOMH                 */
    delay_ms(50);                       /* Delay 50 ms                        */
    drv_lcd->bus_wr_reg(0x20, 0x0000);               /* GRAM horizontal Address            */
    drv_lcd->bus_wr_reg(0x21, 0x0000);               /* GRAM Vertical Address              */
    /* Adjust the Gamma Curve ------------------------------------------------*/
    switch(driverCode)
    {
        case 0x5408: /* LCD with SPFD5408 LCD Controller   */
            drv_lcd->bus_wr_reg(0x30, 0x0B0D);
            drv_lcd->bus_wr_reg(0x31, 0x1923);
            drv_lcd->bus_wr_reg(0x32, 0x1C26);
            drv_lcd->bus_wr_reg(0x33, 0x261C);
            drv_lcd->bus_wr_reg(0x34, 0x2419);
            drv_lcd->bus_wr_reg(0x35, 0x0D0B);
            drv_lcd->bus_wr_reg(0x36, 0x1006);
            drv_lcd->bus_wr_reg(0x37, 0x0610);
            drv_lcd->bus_wr_reg(0x38, 0x0706);
            drv_lcd->bus_wr_reg(0x39, 0x0304);
            drv_lcd->bus_wr_reg(0x3A, 0x0E05);
            drv_lcd->bus_wr_reg(0x3B, 0x0E01);
            drv_lcd->bus_wr_reg(0x3C, 0x010E);
            drv_lcd->bus_wr_reg(0x3D, 0x050E);
            drv_lcd->bus_wr_reg(0x3E, 0x0403);
            drv_lcd->bus_wr_reg(0x3F, 0x0607);
            break;

        case 0x9325: /* LCD with RM68050 LCD Controller    */
            drv_lcd->bus_wr_reg(0x0030,0x0000);
            drv_lcd->bus_wr_reg(0x0031,0x0607);
            drv_lcd->bus_wr_reg(0x0032,0x0305);
            drv_lcd->bus_wr_reg(0x0035,0x0000);
            drv_lcd->bus_wr_reg(0x0036,0x1604);
            drv_lcd->bus_wr_reg(0x0037,0x0204);
            drv_lcd->bus_wr_reg(0x0038,0x0001);
            drv_lcd->bus_wr_reg(0x0039,0x0707);
            drv_lcd->bus_wr_reg(0x003C,0x0000);
            drv_lcd->bus_wr_reg(0x003D,0x000F);
            break;

        case 0x9320: /* LCD with ILI9320 LCD Controller    */
        default: /* LCD with other LCD Controller      */
            drv_lcd->bus_wr_reg(0x30, 0x0006);
            drv_lcd->bus_wr_reg(0x31, 0x0101);
            drv_lcd->bus_wr_reg(0x32, 0x0003);
            drv_lcd->bus_wr_reg(0x35, 0x0106);
            drv_lcd->bus_wr_reg(0x36, 0x0B02);
            drv_lcd->bus_wr_reg(0x37, 0x0302);
            drv_lcd->bus_wr_reg(0x38, 0x0707);
            drv_lcd->bus_wr_reg(0x39, 0x0007);
            drv_lcd->bus_wr_reg(0x3C, 0x0600);
            drv_lcd->bus_wr_reg(0x3D, 0x020B);
            break;
    }

    /* Set GRAM area ---------------------------------------------------------*/
    drv_lcd->bus_wr_reg(0x50, 0x0000); /* Horizontal GRAM Start Address      */
    drv_lcd->bus_wr_reg(0x51, (GLCD_HEIGHT-1)); /* Horizontal GRAM End   Address      */
    drv_lcd->bus_wr_reg(0x52, 0x0000); /* Vertical   GRAM Start Address      */
    drv_lcd->bus_wr_reg(0x53, (GLCD_WIDTH-1)); /* Vertical   GRAM End   Address      */

    /* Set Gate Scan Line ----------------------------------------------------*/
    switch(driverCode)
    {
        case 0x5408: /* LCD with SPFD5408 LCD Controller   */
        case 0x9325: /* LCD with RM68050 LCD Controller    */
#if (GLCD_MIRROR_Y)
        drv_lcd->bus_wr_reg(0x60, 0x2700);
#else
        drv_lcd->bus_wr_reg(0x60, 0xA700);
#endif
        break;
        case 0x9320: /* LCD with ILI9320 LCD Controller    */
        default: /* LCD with other LCD Controller      */
#if (GLCD_MIRROR_Y)
        drv_lcd->bus_wr_reg(0x60, 0xA700);
#else
        drv_lcd->bus_wr_reg(0x60, 0x2700);
#endif
        break;
    }

    drv_lcd->bus_wr_reg(0x61, 0x0001); /* NDL,VLE, REV                       */
    drv_lcd->bus_wr_reg(0x6A, 0x0000); /* Set scrolling line                 */

    /* Partial Display Control -----------------------------------------------*/
    drv_lcd->bus_wr_reg(0x80, 0x0000);
    drv_lcd->bus_wr_reg(0x81, 0x0000);
    drv_lcd->bus_wr_reg(0x82, 0x0000);
    drv_lcd->bus_wr_reg(0x83, 0x0000);
    drv_lcd->bus_wr_reg(0x84, 0x0000);
    drv_lcd->bus_wr_reg(0x85, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    drv_lcd->bus_wr_reg(0x90, 0x0010);
    drv_lcd->bus_wr_reg(0x92, 0x0000);
    drv_lcd->bus_wr_reg(0x93, 0x0003);
    drv_lcd->bus_wr_reg(0x95, 0x0110);
    drv_lcd->bus_wr_reg(0x97, 0x0000);
    drv_lcd->bus_wr_reg(0x98, 0x0000);

    /* Set GRAM write direction
    I/D=11 (Horizontal : increment, Vertical : increment)                  */
#if ( GLCD_SWAP_XY == 1)
    /* AM=1   (address is updated in vertical writing direction)              */
    drv_lcd->bus_wr_reg(0x03, 0x1038);
#else
    /* AM=0   (address is updated in horizontal writing direction)            */
    drv_lcd->bus_wr_reg(0x03, 0x1030);
#endif
    drv_lcd->bus_wr_reg(0x07, 0x0137); /* 262K color and display ON          */
    drv_lcd->bl_sts(TRUE);
}

/*******************************************************************************
* Set draw window region                                                       *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        window width in pixel                            *
*                   h:        window height in pixels                          *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_SetWindow(dword_t x, dword_t y, dword_t w, dword_t h)
{
    dword_t xe, ye;
    if(Himax)
    {
        xe = x+w-1;
        ye = y+h-1;
        drv_lcd->bus_wr_reg(0x02, x >> 8); /* Column address start MSB           */
        drv_lcd->bus_wr_reg(0x03, x&0xFF); /* Column address start LSB           */
        drv_lcd->bus_wr_reg(0x04, xe >> 8); /* Column address end MSB             */
        drv_lcd->bus_wr_reg(0x05, xe&0xFF); /* Column address end LSB             */
        drv_lcd->bus_wr_reg(0x06, y >> 8); /* Row address start MSB              */
        drv_lcd->bus_wr_reg(0x07, y&0xFF); /* Row address start LSB              */
        drv_lcd->bus_wr_reg(0x08, ye >> 8); /* Row address end MSB                */
        drv_lcd->bus_wr_reg(0x09, ye&0xFF); /* Row address end LSB                */
    }
    else
    {
#if ( GLCD_SWAP_XY == 1)
        drv_lcd->bus_wr_reg(0x50, y); /* Vertical   GRAM Start Address      */
        drv_lcd->bus_wr_reg(0x51, y+h-1); /* Vertical   GRAM End   Address (-1) */
        drv_lcd->bus_wr_reg(0x52, x); /* Horizontal GRAM Start Address      */
        drv_lcd->bus_wr_reg(0x53, x+w-1); /* Horizontal GRAM End   Address (-1) */
        drv_lcd->bus_wr_reg(0x20, y);
        drv_lcd->bus_wr_reg(0x21, x);
#else
        drv_lcd->bus_wr_reg(0x50, x); /* Horizontal GRAM Start Address      */
        drv_lcd->bus_wr_reg(0x51, x+w-1); /* Horizontal GRAM End   Address (-1) */
        drv_lcd->bus_wr_reg(0x52, y); /* Vertical   GRAM Start Address      */
        drv_lcd->bus_wr_reg(0x53, y+h-1); /* Vertical   GRAM End   Address (-1) */
        drv_lcd->bus_wr_reg(0x20, x);
        drv_lcd->bus_wr_reg(0x21, y);
#endif
    }
}

/**
  \fn          int32_t GLCD_SetForegroundColor (uint32_t color)
  \brief       Set foreground color
  \param[in]   color  Color value
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetForegroundColor (uint32_t color) {
  foreground_color = color;
  return 0;
}

/**
  \fn          int32_t GLCD_SetBackgroundColor (uint32_t color)
  \brief       Set background color
  \param[in]   color  Color value
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetBackgroundColor (uint32_t color) {
  background_color = color;
  return 0;
}

/**
  \fn          int32_t GLCD_ClearScreen (void)
  \brief       Clear screen (with active background color)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_ClearScreen (void) {

  GLCD_SetWindow(0, 0, GLCD_WIDTH, GLCD_HEIGHT);
  drv_lcd->bus_wr_n_reg(0x22,GLCD_WIDTH * GLCD_HEIGHT,background_color);
  return 0;
}

/**
  \fn          int32_t GLCD_SetFont (GLCD_FONT *font)
  \brief       Set active font
  \param[in]   font   Pointer to font structure
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_SetFont (GLCD_FONT *font) {
  active_font = font;
  return 0;
}

/**
  \fn          int32_t GLCD_DrawPixel (uint32_t x, uint32_t y)
  \brief       Draw pixel (in active foreground color)
  \param[in]   x  x position in pixels (0 = left corner)
  \param[in]   y  y position in pixels (0 = upper corner)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawPixel (uint32_t x, uint32_t y) {

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif

  if (Himax) {
    drv_lcd->bus_wr_reg(0x02, x >>    8);            /* Column address start MSB           */
    drv_lcd->bus_wr_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    drv_lcd->bus_wr_reg(0x04, x >>    8);            /* Column address end MSB             */
    drv_lcd->bus_wr_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    drv_lcd->bus_wr_reg(0x06, y >>    8);            /* Row address start MSB              */
    drv_lcd->bus_wr_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    drv_lcd->bus_wr_reg(0x08, y >>    8);            /* Row address end MSB                */
    drv_lcd->bus_wr_reg(0x09, y &  0xFF);            /* Row address end LSB                */
  }
  else {
  #if (GLCD_SWAP_XY == 1)
    drv_lcd->bus_wr_reg(0x20, y);
    drv_lcd->bus_wr_reg(0x21, x);
  #else
    drv_lcd->bus_wr_reg(0x20, x);
    drv_lcd->bus_wr_reg(0x21, y);
  #endif
  }

  drv_lcd->bus_wr_reg(0x22,foreground_color);

  return 0;
}

/**
  \fn          int32_t GLCD_DrawHLine (uint32_t x, uint32_t y, uint32_t length)
  \brief       Draw horizontal line (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   length Line length
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawHLine (uint32_t x, uint32_t y, uint32_t length) {

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif
  GLCD_SetWindow(x, y, length, 1);
  drv_lcd->bus_wr_n_reg(0x22,length,foreground_color);
  return 0;
}

/**
  \fn          int32_t GLCD_DrawVLine (uint32_t x, uint32_t y, uint32_t length)
  \brief       Draw vertical line (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   length Line length in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawVLine (uint32_t x, uint32_t y, uint32_t length) {

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif

  GLCD_SetWindow(x, y, 1, length);
  drv_lcd->bus_wr_n_reg(0x22,length,foreground_color);
  return 0;
}

/**
  \fn          int32_t GLCD_DrawRectangle (uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  \brief       Draw rectangle (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Rectangle width in pixels
  \param[in]   height Rectangle height in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawRectangle (uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

  GLCD_DrawHLine (x,         y,          width);
  GLCD_DrawHLine (x,         y + height, width);
  GLCD_DrawVLine (x,         y,          height);
  GLCD_DrawVLine (x + width, y,          height);

  return 0;
}

/**
  \fn          int32_t GLCD_DrawChar (uint32_t x, uint32_t y, int32_t ch)
  \brief       Draw character (in active foreground color)
  \param[in]   x      x position in pixels (0 = left corner)
  \param[in]   y      y position in pixels (0 = upper corner)
  \param[in]   ch     Character
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawChar (uint32_t x, uint32_t y, int32_t ch) {
  uint32_t i, j;
  uint32_t wb;
  uint8_t *ptr_ch_bmp;

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif

  if (active_font == NULL) return -1;

  ch        -= active_font->offset;
  wb         = (active_font->width + 7)/8;
  ptr_ch_bmp = (uint8_t *)active_font->bitmap + (ch * wb * active_font->height);

  GLCD_SetWindow(x, y, active_font->width, active_font->height);

  drv_lcd->bus_wr_cmd(0x22);
  wr_dat_start();

  for (i = 0; i < active_font->height; i++) {
    for (j = 0; j < active_font->width; j++) {
      drv_lcd->bus_wr_dat(((*ptr_ch_bmp >> (j & 7)) & 1) ? foreground_color : background_color);
      if (((j & 7) == 7) && (j != (active_font->width - 1))) ptr_ch_bmp++;
    }
    ptr_ch_bmp++;
  }
  wr_dat_stop();

  return 0;
}

/**
  \fn          int32_t GLCD_DrawString (uint32_t x, uint32_t y, const char *str)
  \brief       Draw string (in active foreground color)
  \param[in]   x      x position in pixels (0 = left corner)
  \param[in]   y      y position in pixels (0 = upper corner)
  \param[in]   str    Null-terminated String
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawString (uint32_t x, uint32_t y, const char *str) {

  while (*str) { GLCD_DrawChar(x, y, *str++); x += active_font->width; }

  return 0;
}

/**
  \fn          int32_t GLCD_DrawBargraph (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t val)
  \brief       Draw bargraph (in active foreground color)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Full bargraph width in pixels
  \param[in]   height Full bargraph height in pixels
  \param[in]   val    Active bargraph value (maximum value is 100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawBargraph (uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t val) {
  uint32_t i, j;

  val = (val * width) / 100;            /* Scale value                        */

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif

  GLCD_SetWindow(x, y, width, height);

  drv_lcd->bus_wr_cmd(0x22);
  wr_dat_start();
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(j >= val) {
        drv_lcd->bus_wr_dat(background_color);
      } else {
        drv_lcd->bus_wr_dat(foreground_color);
      }
    }
  }
  wr_dat_stop();

  return 0;
}

/**
  \fn          int32_t GLCD_DrawBitmap (uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t *bitmap)
  \brief       Draw bitmap (bitmap from BMP file without header)
  \param[in]   x      Start x position in pixels (0 = left corner)
  \param[in]   y      Start y position in pixels (0 = upper corner)
  \param[in]   width  Bitmap width in pixels
  \param[in]   height Bitmap height in pixels
  \param[in]   bitmap Bitmap data
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_DrawBitmap (uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t *bitmap) {
  int32_t   i, j;
  uint16_t *ptr_bmp;

  ptr_bmp = (uint16_t *)bitmap;

#if (GLCD_SWAP_XY == 0)
  y = (y + Scroll) % GLCD_HEIGHT;
#endif

  GLCD_SetWindow(x, y, width, height);

  drv_lcd->bus_wr_cmd(0x22);
  wr_dat_start();

  //for (i = (height-1)*width; i > -1; i -= width)
  for(i = 0; i<(height-1)*width; i += width)
  {
    for (j = 0; j < width; j++) {
      drv_lcd->bus_wr_dat(ptr_bmp[i+j]);
    }
  }
  wr_dat_stop();

  return 0;
}

/**
  \fn          int32_t GLCD_VScroll (uint32_t dy)
  \brief       Scroll picture on display vertically
  \param[in]   dy  Scroll size in pixels
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t GLCD_VScroll (uint32_t dy) {
#if (GLCD_SWAP_XY == 0)

  Scroll = (Scroll + dy) % GLCD_HEIGHT;

  if (Himax) {
    drv_lcd->bus_wr_reg(0x01, 0x08);
    drv_lcd->bus_wr_reg(0x14, Scroll >> 8);          /* VSP MSB                            */
    drv_lcd->bus_wr_reg(0x15, Scroll & 0xFF);        /* VSP LSB                            */
  }
  else {
    drv_lcd->bus_wr_reg(0x6A, Scroll);
    drv_lcd->bus_wr_reg(0x61,  3);
  }

  /* Clear new space */
  GLCD_SetWindow(0, Scroll, GLCD_WIDTH, dy);

  drv_lcd->bus_wr_n_reg(0x22,GLCD_WIDTH * dy,background_color);

  return 0;
#else
  return -1;
#endif
}

/**
  \fn          int32_t GLCD_FrameBufferAccess (bool enable)
  \brief       Enable or disable direct access to FrameBuffer
  \param[in]   enable Enable or disable request
                - true (1): enable
                - false(0): disable
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t  GLCD_FrameBufferAccess (bool enable) {
  GLCD_SetWindow(0, 0, GLCD_WIDTH, GLCD_HEIGHT);

  drv_lcd->bus_wr_reg (0x20, 0x0000);
  drv_lcd->bus_wr_reg (0x21, 0x0000);
  drv_lcd->bus_wr_cmd (0x22);
  return 0;
}


