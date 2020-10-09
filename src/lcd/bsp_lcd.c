

/**
 * @file    lcd.c
 * @author  meng_yu
 * @brief   low level Graphic LCD (320x240 pixels) with 16-bit parallel interface realize
 * @version 0.0.1
 * @date    2020-09-14
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "stm32_config.h"
#include "gpio.h"
#include "bsp_lcd.h"

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

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/
//const LCD_UTILS_Drv_t LCD_Driver =
//{
//  BSP_LCD_DrawBitmap,
//  BSP_LCD_FillRGBRect,
//  BSP_LCD_DrawHLine,
//  BSP_LCD_DrawVLine,
//  BSP_LCD_FillRect,
//  BSP_LCD_ReadPixel,
//  BSP_LCD_WritePixel,
//  BSP_LCD_GetXSize,
//  BSP_LCD_GetYSize,
//  BSP_LCD_SetActiveLayer,
//  BSP_LCD_GetPixelFormat
//};

/*-----------------------------------------------------------------------------------
  LCD Backlight functions definition
-----------------------------------------------------------------------------------*/
static void glcd_bl_on(void)
{
    GPIOB->BSRR |= PIN15;
}


static void glcd_bl_off(void)
{
    GPIOB->BSRR |= ((uint32_t)PIN15 << 16);
}


static void glcd_bl_init(void)
{
    glcd_bl_off();
    RCC->AHB1ENR |= (1UL << 1); //Enable clock for GPIOB
    GPIO_Set(GPIOB, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
}


/*-----------------------------------------------------------------------------------
  LCD FSMC functions definition
-----------------------------------------------------------------------------------*/
static void glcd_fsmc_init(void)
{

    RCC->AHB3ENR |= (1UL << 0); /* Enable FSMC clock                  */

#ifdef __STM_EVAL /* STM3220G-EVAL and STM3240G-EVAL    */

    FSMC_Bank1->BTCR[(3-1)*2+1] = /* Bank3 NOR/SRAM timing register     */
    /* configuration                      */
#else /* MCBSTM32F200 and MCBSTMF400 board  */


    FSMC_Bank1->BTCR[(4-1)*2+1] = /* Bank4 NOR/SRAM timing register     */
    /* configuration                      */
#endif

    (0 << 28) | /* FSMC AccessMode A                  */
    (2 << 24) | /* Data Latency                       */
    (2 << 20) | /* CLK Division                       */
    (0 << 16) | /* Bus Turnaround Duration            */
    (9 << 8) | /* Data SetUp Time                    */
    (1 << 4) | /* Address Hold Time                  */
    (5 << 0); /* Address SetUp Time                 */

#ifdef __STM_EVAL /* STM3220G-EVAL and STM3240G-EVAL    */
    FSMC_Bank1->BTCR[(3-1)*2+0] =

#else /* MCBSTM32F200 and MCBSTMF400 board  */

    FSMC_Bank1->BTCR[(4-1)*2+0] = /* Control register                   */
#endif

    (0 << 19) | /* Write burst disabled               */
    (0 << 15) | /* Async wait disabled                */
    (0 << 14) | /* Extended mode disabled             */
    (0 << 13) | /* NWAIT signal is disabled           */
    (1 << 12) | /* Write operation enabled            */

    (0 << 11) | /* NWAIT signal is active one data    */
    /* cycle before wait state            */
    (0 << 10) | /* Wrapped burst mode disabled        */
    (0 << 9) | /* Wait signal polarity active low    */
    (0 << 8) | /* Burst access mode disabled         */
    (1 << 4) | /* Memory data  bus width is 16 bits  */
    (0 << 2) | /* Memory type is SRAM                */
    (0 << 1) | /* Address/Data Multiplexing disable  */
    (1 << 0); /* Memory Bank enable                 */
}

static void glcd_fsmc_gpio_init(void)
{
    static dword_t fsmc_inited = 0;
    if(fsmc_inited)
        return;
    fsmc_inited = 1;
    RCC->AHB1ENR |= ((1UL << 0) | /* Enable GPIOA clock                 */
                    (1UL << 3) | /* Enable GPIOD clock                 */
                    (1UL << 4) | /* Enable GPIOE clock                 */
                    (1UL << 5) | /* Enable GPIOF clock                 */
                    (1UL << 6)); /* Enable GPIOG clock                 */

    /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE)                           */
    /* PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1)                 */
    GPIO_Set(GPIOD, PIN0 | PIN1 | PIN4 | PIN5 | PIN8 | PIN9 | PIN10 | PIN14 | PIN15,
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    GPIO_AF_Set(GPIOD, 0, 12); //PD.00(D2),AF12
    GPIO_AF_Set(GPIOD, 1, 12); //PD.01(D3),AF12
    GPIO_AF_Set(GPIOD, 4, 12); //PD.04(NOE),AF12
    GPIO_AF_Set(GPIOD, 5, 12); //PD.05(NWE),AF12
    GPIO_AF_Set(GPIOD, 8, 12); //PD.08(D13),AF12
    GPIO_AF_Set(GPIOD, 9, 12); //PD.09(D14),AF12
    GPIO_AF_Set(GPIOD, 10, 12); //PD.10(D15),AF12
    GPIO_AF_Set(GPIOD, 14, 12); //PD.14(D0),AF12
    GPIO_AF_Set(GPIOD, 15, 12); //PD.15(D1),AF12

    /* PE.07(D4), PE.08(D5),  PE.09(D6),  PE.10(D7), PE.11(D8)                  */
    /* PE.12(D9), PE.13(D10), PE.14(D11), PE.15(D12)                            */
    GPIO_Set(GPIOE, PIN7 | PIN8 | PIN9 | PIN10 | PIN11 | PIN12 | PIN13 | PIN14 | PIN15,
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    GPIO_AF_Set(GPIOE, 7, 12); //PE.07(D4),AF12
    GPIO_AF_Set(GPIOE, 8, 12); //PE.08(D5),AF12
    GPIO_AF_Set(GPIOE, 9, 12); //PE.09(D6),AF12
    GPIO_AF_Set(GPIOE, 10, 12); //PE.10(D7),AF12
    GPIO_AF_Set(GPIOE, 11, 12); //PE.11(D8),AF12
    GPIO_AF_Set(GPIOE, 12, 12); //PE.12(D9),AF12
    GPIO_AF_Set(GPIOE, 13, 12); //PE.13(D10),AF12
    GPIO_AF_Set(GPIOE, 14, 12); //PE.14(D11),AF12
    GPIO_AF_Set(GPIOE, 15, 12); //PE.15(D12),AF12

    /* PF.00(A0 (RS)) */
    GPIO_Set(GPIOF, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    GPIO_AF_Set(GPIOF, 12, 12); //PE.12(A6),AF12

    /* PG.12(NE4 (LCD/CS)) - CE1(LCD /CS)                                       */
    GPIO_Set(GPIOG, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    GPIO_AF_Set(GPIOG, 12, 12); //PE.12(NE4),AF12
}


//static void glcd_delay(int cnt)
//{
////    while(cnt--);
//    ms *= (SystemCoreClock/10000);
//    while (ms--) { __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); }
//}

static void delay_ms (int ms) {
  ms *= (SystemCoreClock/10000);
  while (ms--) { __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); }
}


static __inline void wr_cmd(byte_t cmd)
{
    LCD_REG16 = cmd;
}

static __inline void wr_dat(word_t dat)
{
    LCD_DAT16 = dat;
}

static __inline word_t rd_dat(void)
{
    return (LCD_DAT16);
}

static __inline void wr_reg(byte_t reg, word_t val)
{
    wr_cmd(reg);
    wr_dat(val);
}

static word_t rd_reg(byte_t reg)
{
    wr_cmd(reg);
    return (rd_dat());
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
    glcd_fsmc_init();
    glcd_fsmc_gpio_init();
    glcd_bl_init();
    delay_ms(50); /* delay_ms 50 ms                        */
    driverCode = rd_reg(0x00);
    LOG_INFO("lcd_code:%x\r\n", driverCode);
    Himax = 0; /* This is not Himax LCD controller   */
    /* Start Initial Sequence ------------------------------------------------*/
#if (GLCD_MIRROR_X)
    wr_reg(0x01, 0x0000);               /* Clear SS bit                       */
#else
    wr_reg(0x01, 0x0100);               /* Set SS bit                         */
#endif
    wr_reg(0x02, 0x0700);               /* Set 1 line inversion               */
    wr_reg(0x04, 0x0000);               /* Resize register                    */
    wr_reg(0x08, 0x0207);               /* 2 lines front, 7 back porch        */
    wr_reg(0x09, 0x0000);               /* Set non-disp area refresh cyc ISC  */
    wr_reg(0x0A, 0x0000);               /* FMARK function                     */
    wr_reg(0x0C, 0x0000);               /* RGB interface setting              */
    wr_reg(0x0D, 0x0000);               /* Frame marker Position              */
    wr_reg(0x0F, 0x0000);               /* RGB interface polarity             */

    /* Power On sequence -----------------------------------------------------*/
    wr_reg(0x10, 0x0000);               /* Reset Power Control 1              */
    wr_reg(0x11, 0x0000);               /* Reset Power Control 2              */
    wr_reg(0x12, 0x0000);               /* Reset Power Control 3              */
    wr_reg(0x13, 0x0000);               /* Reset Power Control 4              */
    delay_ms(200);                      /* Discharge cap power voltage (200ms)*/
    wr_reg(0x10, 0x12B0);               /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
    wr_reg(0x11, 0x0007);               /* DC1[2:0], DC0[2:0], VC[2:0]        */
    delay_ms(50);                       /* Delay 50 ms                        */
    wr_reg(0x12, 0x01BD);               /* VREG1OUT voltage                   */
    delay_ms(50);                       /* Delay 50 ms                        */
    wr_reg(0x13, 0x1400);               /* VDV[4:0] for VCOM amplitude        */
    wr_reg(0x29, 0x000E);               /* VCM[4:0] for VCOMH                 */
    delay_ms(50);                       /* Delay 50 ms                        */
    wr_reg(0x20, 0x0000);               /* GRAM horizontal Address            */
    wr_reg(0x21, 0x0000);               /* GRAM Vertical Address              */
    /* Adjust the Gamma Curve ------------------------------------------------*/
    switch(driverCode)
    {
        case 0x5408: /* LCD with SPFD5408 LCD Controller   */
            wr_reg(0x30, 0x0B0D);
            wr_reg(0x31, 0x1923);
            wr_reg(0x32, 0x1C26);
            wr_reg(0x33, 0x261C);
            wr_reg(0x34, 0x2419);
            wr_reg(0x35, 0x0D0B);
            wr_reg(0x36, 0x1006);
            wr_reg(0x37, 0x0610);
            wr_reg(0x38, 0x0706);
            wr_reg(0x39, 0x0304);
            wr_reg(0x3A, 0x0E05);
            wr_reg(0x3B, 0x0E01);
            wr_reg(0x3C, 0x010E);
            wr_reg(0x3D, 0x050E);
            wr_reg(0x3E, 0x0403);
            wr_reg(0x3F, 0x0607);
            break;

        case 0x9325: /* LCD with RM68050 LCD Controller    */
            wr_reg(0x0030,0x0000);
            wr_reg(0x0031,0x0607);
            wr_reg(0x0032,0x0305);
            wr_reg(0x0035,0x0000);
            wr_reg(0x0036,0x1604);
            wr_reg(0x0037,0x0204);
            wr_reg(0x0038,0x0001);
            wr_reg(0x0039,0x0707);
            wr_reg(0x003C,0x0000);
            wr_reg(0x003D,0x000F);
            break;

        case 0x9320: /* LCD with ILI9320 LCD Controller    */
        default: /* LCD with other LCD Controller      */
            wr_reg(0x30, 0x0006);
            wr_reg(0x31, 0x0101);
            wr_reg(0x32, 0x0003);
            wr_reg(0x35, 0x0106);
            wr_reg(0x36, 0x0B02);
            wr_reg(0x37, 0x0302);
            wr_reg(0x38, 0x0707);
            wr_reg(0x39, 0x0007);
            wr_reg(0x3C, 0x0600);
            wr_reg(0x3D, 0x020B);
            break;
    }

    /* Set GRAM area ---------------------------------------------------------*/
    wr_reg(0x50, 0x0000); /* Horizontal GRAM Start Address      */
    wr_reg(0x51, (GLCD_HEIGHT-1)); /* Horizontal GRAM End   Address      */
    wr_reg(0x52, 0x0000); /* Vertical   GRAM Start Address      */
    wr_reg(0x53, (GLCD_WIDTH-1)); /* Vertical   GRAM End   Address      */

    /* Set Gate Scan Line ----------------------------------------------------*/
    switch(driverCode)
    {
        case 0x5408: /* LCD with SPFD5408 LCD Controller   */
        case 0x9325: /* LCD with RM68050 LCD Controller    */
#if (GLCD_MIRROR_Y)
        wr_reg(0x60, 0x2700);
#else
        wr_reg(0x60, 0xA700);
#endif
        break;
        case 0x9320: /* LCD with ILI9320 LCD Controller    */
        default: /* LCD with other LCD Controller      */
#if (GLCD_MIRROR_Y)
        wr_reg(0x60, 0xA700);
#else
        wr_reg(0x60, 0x2700);
#endif
        break;
    }

    wr_reg(0x61, 0x0001); /* NDL,VLE, REV                       */
    wr_reg(0x6A, 0x0000); /* Set scrolling line                 */

    /* Partial Display Control -----------------------------------------------*/
    wr_reg(0x80, 0x0000);
    wr_reg(0x81, 0x0000);
    wr_reg(0x82, 0x0000);
    wr_reg(0x83, 0x0000);
    wr_reg(0x84, 0x0000);
    wr_reg(0x85, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    wr_reg(0x90, 0x0010);
    wr_reg(0x92, 0x0000);
    wr_reg(0x93, 0x0003);
    wr_reg(0x95, 0x0110);
    wr_reg(0x97, 0x0000);
    wr_reg(0x98, 0x0000);

    /* Set GRAM write direction
    I/D=11 (Horizontal : increment, Vertical : increment)                  */
#if ( GLCD_SWAP_XY == 1)
    /* AM=1   (address is updated in vertical writing direction)              */
    wr_reg(0x03, 0x1038);
#else
    /* AM=0   (address is updated in horizontal writing direction)            */
    wr_reg(0x03, 0x1030);
#endif
    wr_reg(0x07, 0x0137); /* 262K color and display ON          */
    glcd_bl_on();
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
        wr_reg(0x02, x >> 8); /* Column address start MSB           */
        wr_reg(0x03, x&0xFF); /* Column address start LSB           */
        wr_reg(0x04, xe >> 8); /* Column address end MSB             */
        wr_reg(0x05, xe&0xFF); /* Column address end LSB             */
        wr_reg(0x06, y >> 8); /* Row address start MSB              */
        wr_reg(0x07, y&0xFF); /* Row address start LSB              */
        wr_reg(0x08, ye >> 8); /* Row address end MSB                */
        wr_reg(0x09, ye&0xFF); /* Row address end LSB                */
    }
    else
    {
#if ( GLCD_SWAP_XY == 1)
        wr_reg(0x50, y); /* Vertical   GRAM Start Address      */
        wr_reg(0x51, y+h-1); /* Vertical   GRAM End   Address (-1) */
        wr_reg(0x52, x); /* Horizontal GRAM Start Address      */
        wr_reg(0x53, x+w-1); /* Horizontal GRAM End   Address (-1) */
        wr_reg(0x20, y);
        wr_reg(0x21, x);
#else
        wr_reg(0x50, x); /* Horizontal GRAM Start Address      */
        wr_reg(0x51, x+w-1); /* Horizontal GRAM End   Address (-1) */
        wr_reg(0x52, y); /* Vertical   GRAM Start Address      */
        wr_reg(0x53, y+h-1); /* Vertical   GRAM End   Address (-1) */
        wr_reg(0x20, x);
        wr_reg(0x21, y);
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
  uint32_t i;

  GLCD_SetWindow(0, 0, GLCD_WIDTH, GLCD_HEIGHT);

  wr_cmd(0x22);
  wr_dat_start();
  for (i = 0; i < (GLCD_WIDTH * GLCD_HEIGHT); i++) {
    wr_dat(background_color);
  }
  wr_dat_stop();

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
    wr_reg(0x02, x >>    8);            /* Column address start MSB           */
    wr_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    wr_reg(0x04, x >>    8);            /* Column address end MSB             */
    wr_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    wr_reg(0x06, y >>    8);            /* Row address start MSB              */
    wr_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    wr_reg(0x08, y >>    8);            /* Row address end MSB                */
    wr_reg(0x09, y &  0xFF);            /* Row address end LSB                */
  }
  else {
  #if (GLCD_SWAP_XY == 1)
    wr_reg(0x20, y);
    wr_reg(0x21, x);
  #else
    wr_reg(0x20, x);
    wr_reg(0x21, y);
  #endif
  }

  wr_cmd(0x22);
  wr_dat(foreground_color);

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
  wr_cmd(0x22);
  wr_dat_start();

  while (length--) {
    wr_dat(foreground_color);
  }
  wr_dat_stop();

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
  wr_cmd(0x22);
  wr_dat_start();

  while (length--) {
    wr_dat(foreground_color);
  }
  wr_dat_stop();

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

  wr_cmd(0x22);
  wr_dat_start();

  for (i = 0; i < active_font->height; i++) {
    for (j = 0; j < active_font->width; j++) {
      wr_dat(((*ptr_ch_bmp >> (j & 7)) & 1) ? foreground_color : background_color);
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

  wr_cmd(0x22);
  wr_dat_start();
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if(j >= val) {
        wr_dat(background_color);
      } else {
        wr_dat(foreground_color);
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

  wr_cmd(0x22);
  wr_dat_start();


  //for (i = (height-1)*width; i > -1; i -= width)
  for(i = 0; i<(height-1)*width; i += width)
  {
    for (j = 0; j < width; j++) {
      wr_dat(ptr_bmp[i+j]);
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
  uint32_t i;

  Scroll = (Scroll + dy) % GLCD_HEIGHT;

  if (Himax) {
    wr_reg(0x01, 0x08);
    wr_reg(0x14, Scroll >> 8);          /* VSP MSB                            */
    wr_reg(0x15, Scroll & 0xFF);        /* VSP LSB                            */
  }
  else {
    wr_reg(0x6A, Scroll);
    wr_reg(0x61,  3);
  }

  /* Clear new space */
  GLCD_SetWindow(0, Scroll, GLCD_WIDTH, dy);

  wr_cmd(0x22);
  wr_dat_start();
  for(i = 0; i < (GLCD_WIDTH * dy); i++) {
    wr_dat(background_color);
  }
  wr_dat_stop();

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

  wr_reg (0x20, 0x0000);
  wr_reg (0x21, 0x0000);
  wr_cmd (0x22);
  return 0;
}

/**
  \fn          uint32_t GLCD_FrameBufferAddress (void)
  \brief       Retrieve FrameBuffer address
  \returns     FrameBuffer address
*/
uint32_t GLCD_FrameBufferAddress (void) {
  return (uint32_t)&LCD_DAT16;
}

