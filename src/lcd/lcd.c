

/**
 * @file    lcd.c
 * @author  meng_yu
 * @brief   low level Graphic LCD (320x240 pixels) with 16-bit parallel interface realize
 * @version 0.0.1
 * @date    2020-09-14
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "stm32_config.h"
#include "common.h"
#include "gpio.h"
#include "lcd.h"
#include "font_6x8.h"
#include "font_16x24.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/
static volatile unsigned short Color[2] =
{
    White, Black
};


static unsigned char Himax;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/
static void glcd_delay(int cnt)
{
    while(cnt--);
}


static __inline void glcd_wr_cmd(unsigned char cmd)
{
    reg_lcd->cmd = cmd;
}


static __inline void glcd_wr_dat(unsigned short dat)
{
    reg_lcd->dat = dat;
}


static __inline void glcd_wr_dat_only(unsigned short dat)
{
    reg_lcd->dat = dat;
}


static __inline unsigned short glcd_rd_dat(void)
{
    return (reg_lcd->dat);
}


static __inline void glcd_wr_reg(unsigned char reg, unsigned short val)
{
    glcd_wr_cmd(reg);
    glcd_wr_dat(val);
}


static unsigned short glcd_rd_reg(unsigned char reg)
{
    glcd_wr_cmd(reg);
    return (glcd_rd_dat());
}


static __inline void glcd_wr_dat_start(void)
{
    /* only used for SPI interface */
}


static __inline void glcd_wr_dat_stop(void)
{
    /* only used for SPI interface */
}


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


/*-----------------------------------------------------------------------------------
  Initialize the Graphic LCD controller functions definition
-----------------------------------------------------------------------------------*/
void GLCD_Init(void)
{
    unsigned short driverCode;

    glcd_bl_init();
    RCC->AHB1ENR |= ((1UL << 0) | /* Enable GPIOA clock                 */
    (1UL << 3) | /* Enable GPIOD clock                 */
    (1UL << 4) | /* Enable GPIOE clock                 */
    (1UL << 5) | /* Enable GPIOF clock                 */
    (1UL << 6)); /* Enable GPIOG clock                 */

    /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE)                           */
    /* PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1)                 */
    GPIO_Set(GPIOD, PIN0 | PIN1 | PIN4 | PIN5 | PIN8 | PIN9 | PIN10 | PIN14 | PIN15,
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
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
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
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
    GPIO_Set(GPIOF, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOF, 12, 12); //PE.12(A6),AF12

    /* PG.12(NE4 (LCD/CS)) - CE1(LCD /CS)                                       */
    GPIO_Set(GPIOG, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOG, 12, 12); //PE.12(NE4),AF12
    glcd_fsmc_init();
    glcd_delay(500); /* glcd_delay 50 ms                        */
    driverCode = glcd_rd_reg(0x00);
    LOG_INFO("lcd_code:%x\r\n", driverCode);
    if(driverCode==0x47)
    { /* LCD with HX8347-D LCD Controller   */

        Himax = 1; /* Set Himax LCD controller flag      */
        /* Driving ability settings ----------------------------------------------*/
        glcd_wr_reg(0xEA, 0x00); /* Power control internal used (1)    */
        glcd_wr_reg(0xEB, 0x20); /* Power control internal used (2)    */
        glcd_wr_reg(0xEC, 0x0C); /* Source control internal used (1)   */
        glcd_wr_reg(0xED, 0xC7); /* Source control internal used (2)   */
        glcd_wr_reg(0xE8, 0x38); /* Source output period Normal mode   */
        glcd_wr_reg(0xE9, 0x10); /* Source output period Idle mode     */
        glcd_wr_reg(0xF1, 0x01); /* RGB 18-bit interface ;0x0110       */
        glcd_wr_reg(0xF2, 0x10);

        /* Adjust the Gamma Curve ------------------------------------------------*/
        glcd_wr_reg(0x40, 0x01);
        glcd_wr_reg(0x41, 0x00);
        glcd_wr_reg(0x42, 0x00);
        glcd_wr_reg(0x43, 0x10);
        glcd_wr_reg(0x44, 0x0E);
        glcd_wr_reg(0x45, 0x24);
        glcd_wr_reg(0x46, 0x04);
        glcd_wr_reg(0x47, 0x50);
        glcd_wr_reg(0x48, 0x02);
        glcd_wr_reg(0x49, 0x13);
        glcd_wr_reg(0x4A, 0x19);
        glcd_wr_reg(0x4B, 0x19);
        glcd_wr_reg(0x4C, 0x16);
        glcd_wr_reg(0x50, 0x1B);
        glcd_wr_reg(0x51, 0x31);
        glcd_wr_reg(0x52, 0x2F);
        glcd_wr_reg(0x53, 0x3F);
        glcd_wr_reg(0x54, 0x3F);
        glcd_wr_reg(0x55, 0x3E);
        glcd_wr_reg(0x56, 0x2F);
        glcd_wr_reg(0x57, 0x7B);
        glcd_wr_reg(0x58, 0x09);
        glcd_wr_reg(0x59, 0x06);
        glcd_wr_reg(0x5A, 0x06);
        glcd_wr_reg(0x5B, 0x0C);
        glcd_wr_reg(0x5C, 0x1D);
        glcd_wr_reg(0x5D, 0xCC);

        /* Power voltage setting -------------------------------------------------*/
        glcd_wr_reg(0x1B, 0x1B);
        glcd_wr_reg(0x1A, 0x01);
        glcd_wr_reg(0x24, 0x2F);
        glcd_wr_reg(0x25, 0x57);
        glcd_wr_reg(0x23, 0x88);

        /* Power on setting ------------------------------------------------------*/
        glcd_wr_reg(0x18, 0x36); /* Internal oscillator frequency adj  */
        glcd_wr_reg(0x19, 0x01); /* Enable internal oscillator         */
        glcd_wr_reg(0x01, 0x00); /* Normal mode, no scrool             */
        glcd_wr_reg(0x1F, 0x88); /* Power control 6 - DDVDH Off        */
        glcd_delay(20);
        glcd_wr_reg(0x1F, 0x82); /* Power control 6 - Step-up: 3 x VCI */
        glcd_delay(5);
        glcd_wr_reg(0x1F, 0x92); /* Power control 6 - Step-up: On      */
        glcd_delay(5);
        glcd_wr_reg(0x1F, 0xD2); /* Power control 6 - VCOML active     */
        glcd_delay(5);

        /* Color selection -------------------------------------------------------*/
        glcd_wr_reg(0x17, 0x55); /* RGB, System interface: 16 Bit/Pixel*/
        glcd_wr_reg(0x00, 0x00); /* Scrolling off, no standby          */

        /* Interface config ------------------------------------------------------*/
        glcd_wr_reg(0x2F, 0x11); /* LCD Drive: 1-line inversion        */
        glcd_wr_reg(0x31, 0x00);
        glcd_wr_reg(0x32, 0x00); /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

        /* Display on setting ----------------------------------------------------*/
        glcd_wr_reg(0x28, 0x38); /* PT(0,0) active, VGL/VGL            */
        glcd_delay(20);
        glcd_wr_reg(0x28, 0x3C); /* Display active, VGL/VGL            */

#if ( LANDSCAPE == 1)

#if ( ROTATE180 == 0)
        glcd_wr_reg(0x16, 0xA8);

#else

        glcd_wr_reg(0x16, 0x68);
#endif

#else

#if ( ROTATE180 == 0)
        glcd_wr_reg(0x16, 0x08);

#else

        glcd_wr_reg(0x16, 0xC8);
#endif

#endif

        /* Display scrolling settings --------------------------------------------*/
        glcd_wr_reg(0x0E, 0x00); /* TFA MSB                            */
        glcd_wr_reg(0x0F, 0x00); /* TFA LSB                            */
        glcd_wr_reg(0x10, 320 >> 8); /* VSA MSB                            */
        glcd_wr_reg(0x11, 320&0xFF); /* VSA LSB                            */
        glcd_wr_reg(0x12, 0x00); /* BFA MSB                            */
        glcd_wr_reg(0x13, 0x00); /* BFA LSB                            */
    }
    else
    {

        Himax = 0; /* This is not Himax LCD controller   */
        /* Start Initial Sequence ------------------------------------------------*/

#if ( ROTATE180 == 1)
        glcd_wr_reg(0x01, 0x0000); /* Clear SS bit                       */

#else

        glcd_wr_reg(0x01, 0x0100); /* Set SS bit                         */
#endif

        glcd_wr_reg(0x02, 0x0700); /* Set 1 line inversion               */
        glcd_wr_reg(0x04, 0x0000); /* Resize register                    */
        glcd_wr_reg(0x08, 0x0207); /* 2 lines front, 7 back porch        */
        glcd_wr_reg(0x09, 0x0000); /* Set non-disp area refresh cyc ISC  */
        glcd_wr_reg(0x0A, 0x0000); /* FMARK function                     */
        glcd_wr_reg(0x0C, 0x0000); /* RGB interface setting              */
        glcd_wr_reg(0x0D, 0x0000); /* Frame marker Position              */
        glcd_wr_reg(0x0F, 0x0000); /* RGB interface polarity             */

        /* Power On sequence -----------------------------------------------------*/
        glcd_wr_reg(0x10, 0x0000); /* Reset Power Control 1              */
        glcd_wr_reg(0x11, 0x0000); /* Reset Power Control 2              */
        glcd_wr_reg(0x12, 0x0000); /* Reset Power Control 3              */
        glcd_wr_reg(0x13, 0x0000); /* Reset Power Control 4              */
        glcd_delay(20); /* Discharge cap power voltage (200ms)*/
        glcd_wr_reg(0x10, 0x12B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
        glcd_wr_reg(0x11, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0]        */
        glcd_delay(5); /* glcd_delay 50 ms                        */
        glcd_wr_reg(0x12, 0x01BD); /* VREG1OUT voltage                   */
        glcd_delay(5); /* glcd_delay 50 ms                        */
        glcd_wr_reg(0x13, 0x1400); /* VDV[4:0] for VCOM amplitude        */
        glcd_wr_reg(0x29, 0x000E); /* VCM[4:0] for VCOMH                 */
        glcd_delay(5); /* glcd_delay 50 ms                        */
        glcd_wr_reg(0x20, 0x0000); /* GRAM horizontal Address            */
        glcd_wr_reg(0x21, 0x0000); /* GRAM Vertical Address              */

        /* Adjust the Gamma Curve ------------------------------------------------*/
        switch(driverCode)
        {
            case 0x5408: /* LCD with SPFD5408 LCD Controller   */
                glcd_wr_reg(0x30, 0x0B0D);
                glcd_wr_reg(0x31, 0x1923);
                glcd_wr_reg(0x32, 0x1C26);
                glcd_wr_reg(0x33, 0x261C);
                glcd_wr_reg(0x34, 0x2419);
                glcd_wr_reg(0x35, 0x0D0B);
                glcd_wr_reg(0x36, 0x1006);
                glcd_wr_reg(0x37, 0x0610);
                glcd_wr_reg(0x38, 0x0706);
                glcd_wr_reg(0x39, 0x0304);
                glcd_wr_reg(0x3A, 0x0E05);
                glcd_wr_reg(0x3B, 0x0E01);
                glcd_wr_reg(0x3C, 0x010E);
                glcd_wr_reg(0x3D, 0x050E);
                glcd_wr_reg(0x3E, 0x0403);
                glcd_wr_reg(0x3F, 0x0607);
                break;

            case 0x9325: /* LCD with RM68050 LCD Controller    */
                glcd_wr_reg(0x0030, 0x0000);
                glcd_wr_reg(0x0031, 0x0607);
                glcd_wr_reg(0x0032, 0x0305);
                glcd_wr_reg(0x0035, 0x0000);
                glcd_wr_reg(0x0036, 0x1604);
                glcd_wr_reg(0x0037, 0x0204);
                glcd_wr_reg(0x0038, 0x0001);
                glcd_wr_reg(0x0039, 0x0707);
                glcd_wr_reg(0x003C, 0x0000);
                glcd_wr_reg(0x003D, 0x000F);
                break;

            case 0x9320: /* LCD with ILI9320 LCD Controller    */
            default: /* LCD with other LCD Controller      */
                glcd_wr_reg(0x30, 0x0006);
                glcd_wr_reg(0x31, 0x0101);
                glcd_wr_reg(0x32, 0x0003);
                glcd_wr_reg(0x35, 0x0106);
                glcd_wr_reg(0x36, 0x0B02);
                glcd_wr_reg(0x37, 0x0302);
                glcd_wr_reg(0x38, 0x0707);
                glcd_wr_reg(0x39, 0x0007);
                glcd_wr_reg(0x3C, 0x0600);
                glcd_wr_reg(0x3D, 0x020B);
                break;
        }

        /* Set GRAM area ---------------------------------------------------------*/
        glcd_wr_reg(0x50, 0x0000); /* Horizontal GRAM Start Address      */
        glcd_wr_reg(0x51, (HEIGHT-1)); /* Horizontal GRAM End   Address      */
        glcd_wr_reg(0x52, 0x0000); /* Vertical   GRAM Start Address      */
        glcd_wr_reg(0x53, (WIDTH-1)); /* Vertical   GRAM End   Address      */

        /* Set Gate Scan Line ----------------------------------------------------*/
        switch(driverCode)
        {
            case 0x5408: /* LCD with SPFD5408 LCD Controller   */
            case 0x9325: /* LCD with RM68050 LCD Controller    */

#if ( LANDSCAPE ^ ROTATE180)
                glcd_wr_reg(0x60, 0x2700);

#else

                glcd_wr_reg(0x60, 0xA700);
#endif

                break;

            case 0x9320: /* LCD with ILI9320 LCD Controller    */
            default: /* LCD with other LCD Controller      */

#if ( LANDSCAPE ^ ROTATE180)
                glcd_wr_reg(0x60, 0xA700);

#else

                glcd_wr_reg(0x60, 0x2700);
#endif

                break;
        }

        glcd_wr_reg(0x61, 0x0001); /* NDL,VLE, REV                       */
        glcd_wr_reg(0x6A, 0x0000); /* Set scrolling line                 */

        /* Partial Display Control -----------------------------------------------*/
        glcd_wr_reg(0x80, 0x0000);
        glcd_wr_reg(0x81, 0x0000);
        glcd_wr_reg(0x82, 0x0000);
        glcd_wr_reg(0x83, 0x0000);
        glcd_wr_reg(0x84, 0x0000);
        glcd_wr_reg(0x85, 0x0000);

        /* Panel Control ---------------------------------------------------------*/
        glcd_wr_reg(0x90, 0x0010);
        glcd_wr_reg(0x92, 0x0000);
        glcd_wr_reg(0x93, 0x0003);
        glcd_wr_reg(0x95, 0x0110);
        glcd_wr_reg(0x97, 0x0000);
        glcd_wr_reg(0x98, 0x0000);

        /* Set GRAM write direction
        I/D=11 (Horizontal : increment, Vertical : increment)                  */
#if ( LANDSCAPE == 1)

        /* AM=1   (address is updated in vertical writing direction)              */
        glcd_wr_reg(0x03, 0x1038);

#else

        /* AM=0   (address is updated in horizontal writing direction)            */
        glcd_wr_reg(0x03, 0x1030);
#endif

        glcd_wr_reg(0x07, 0x0137); /* 262K color and display ON          */
    }
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
void GLCD_SetWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    unsigned int xe, ye;
    if(Himax)
    {
        xe = x+w-1;
        ye = y+h-1;
        glcd_wr_reg(0x02, x >> 8); /* Column address start MSB           */
        glcd_wr_reg(0x03, x&0xFF); /* Column address start LSB           */
        glcd_wr_reg(0x04, xe >> 8); /* Column address end MSB             */
        glcd_wr_reg(0x05, xe&0xFF); /* Column address end LSB             */
        glcd_wr_reg(0x06, y >> 8); /* Row address start MSB              */
        glcd_wr_reg(0x07, y&0xFF); /* Row address start LSB              */
        glcd_wr_reg(0x08, ye >> 8); /* Row address end MSB                */
        glcd_wr_reg(0x09, ye&0xFF); /* Row address end LSB                */
    }
    else
    {
#if ( LANDSCAPE == 1)
        glcd_wr_reg(0x50, y); /* Vertical   GRAM Start Address      */
        glcd_wr_reg(0x51, y+h-1); /* Vertical   GRAM End   Address (-1) */
        glcd_wr_reg(0x52, x); /* Horizontal GRAM Start Address      */
        glcd_wr_reg(0x53, x+w-1); /* Horizontal GRAM End   Address (-1) */
        glcd_wr_reg(0x20, y);
        glcd_wr_reg(0x21, x);

#else

        glcd_wr_reg(0x50, x); /* Horizontal GRAM Start Address      */
        glcd_wr_reg(0x51, x+w-1); /* Horizontal GRAM End   Address (-1) */
        glcd_wr_reg(0x52, y); /* Vertical   GRAM Start Address      */
        glcd_wr_reg(0x53, y+h-1); /* Vertical   GRAM End   Address (-1) */
        glcd_wr_reg(0x20, x);
        glcd_wr_reg(0x21, y);
#endif
    }
}


/*******************************************************************************
* Set draw window region to whole screen                                       *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WindowMax(void)
{
    GLCD_SetWindow(0, 0, WIDTH, HEIGHT);
}


/*******************************************************************************
* Draw a pixel in foreground color                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_PutPixel(unsigned int x, unsigned int y)
{
    if(Himax)
    {
        glcd_wr_reg(0x02, x >> 8); /* Column address start MSB           */
        glcd_wr_reg(0x03, x&0xFF); /* Column address start LSB           */
        glcd_wr_reg(0x04, x >> 8); /* Column address end MSB             */
        glcd_wr_reg(0x05, x&0xFF); /* Column address end LSB             */
        glcd_wr_reg(0x06, y >> 8); /* Row address start MSB              */
        glcd_wr_reg(0x07, y&0xFF); /* Row address start LSB              */
        glcd_wr_reg(0x08, y >> 8); /* Row address end MSB                */
        glcd_wr_reg(0x09, y&0xFF); /* Row address end LSB                */
    }
    else
    {
#if ( LANDSCAPE == 1)
        glcd_wr_reg(0x20, y);
        glcd_wr_reg(0x21, x);

#else

        glcd_wr_reg(0x20, x);
        glcd_wr_reg(0x21, y);
#endif
    }
    glcd_wr_cmd(0x22);
    glcd_wr_dat(Color[TXT_COLOR]);
}


/*******************************************************************************
* Set foreground color                                                         *
*   Parameter:      color:    foreground color                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_SetTextColor(unsigned short color)
{
    Color[TXT_COLOR] = color;
}


/*******************************************************************************
* Set background color                                                         *
*   Parameter:      color:    background color                                 *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_SetBackColor(unsigned short color)
{
    Color[BG_COLOR] = color;
}


/*******************************************************************************
* Clear display                                                                *
*   Parameter:      color:    display clearing color                           *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Clear(unsigned short color)
{
    unsigned int i;
    GLCD_WindowMax();
    glcd_wr_cmd(0x22);
    glcd_wr_dat_start();
    for(i = 0; i<(WIDTH*HEIGHT); i++) glcd_wr_dat_only(color);
    glcd_wr_dat_stop();
}


/*******************************************************************************
* Draw character on given position                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   cw:       character width in pixel                         *
*                   ch:       character height in pixels                       *
*                   c:        pointer to character bitmap                      *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_DrawChar(unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char*c)
{
    unsigned int i, j, k, pixs;
    GLCD_SetWindow(x, y, cw, ch);
    glcd_wr_cmd(0x22);
    glcd_wr_dat_start();
    k = (cw+7) / 8;
    if(k==1)
    {
        for(j = 0; j<ch; j++)
        {
            pixs =* (unsigned char*)
            c;
            c += 1;
            for(i = 0; i<cw; i++)
            {
                glcd_wr_dat_only(Color[(pixs >> i)&1]);
            }
        }
    }
    else if(k==2)
    {
        for(j = 0; j<ch; j++)
        {
            pixs =* (unsigned short*)
            c;
            c += 2;
            for(i = 0; i<cw; i++)
            {
                glcd_wr_dat_only(Color[(pixs >> i)&1]);
            }
        }
    }
    glcd_wr_dat_stop();
}


/*******************************************************************************
* Disply character on given line                                               *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   c:        ascii character                                  *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_DisplayChar(unsigned int ln, unsigned int col, unsigned char fi, unsigned char c)
{
    c -= 32;
    switch(fi)
    {
        case 0: /* Font 6 x 8 */
            GLCD_DrawChar(col*6, ln*8, 6, 8, (unsigned char*)&Font_6x8_h[c*8]);
            break;

        case 1: /* Font 16 x 24 */
            GLCD_DrawChar(col*16, ln*24, 16, 24, (unsigned char*)&Font_16x24_h[c*24]);
            break;
    }
}


/*******************************************************************************
* Disply string on given line                                                  *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   s:        pointer to string                                *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_DisplayString(unsigned int ln, unsigned int col, unsigned char fi, unsigned char*s)
{
    while(*s)
    {
        GLCD_DisplayChar(ln, col++, fi, *s++);
    }
}


/*******************************************************************************
* Clear given line                                                             *
*   Parameter:      ln:       line number                                      *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_ClearLn(unsigned int ln, unsigned char fi)
{
    unsigned char i;
    unsigned char buf[60];
    GLCD_WindowMax();
    switch(fi)
    {
        case 0: /* Font 6 x 8 */
            for(i = 0; i<(WIDTH+5) / 6; i++) buf[i] = ' ';
            buf[i+1] = 0;
            break;

        case 1: /* Font 16 x 24 */
            for(i = 0; i<(WIDTH+15) / 16; i++) buf[i] = ' ';
            buf[i+1] = 0;
            break;
    }

    GLCD_DisplayString(ln, 0, fi, buf);
}


/*******************************************************************************
* Draw bargraph                                                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        maximum width of bargraph (in pixels)            *
*                   h:        bargraph height                                  *
*                   val:      value of active bargraph (in 1/1024)             *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Bargraph(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val)
{
    int i, j;
    val = (val*w) >> 10; /* Scale value                        */
    GLCD_SetWindow(x, y, w, h);
    glcd_wr_cmd(0x22);
    glcd_wr_dat_start();
    for(i = 0; i<h; i++)
    {
        for(j = 0; j<=w-1; j++)
        {
            if(j>=val)
            {
                glcd_wr_dat_only(Color[BG_COLOR]);
            }
            else
            {
                glcd_wr_dat_only(Color[TXT_COLOR]);
            }
        }
    }
    glcd_wr_dat_stop();
}


/*******************************************************************************
* Display graphical bitmap image at position x horizontally and y vertically   *
* (This function is optimized for 16 bits per pixel format, it has to be       *
*  adapted for any other bits per pixel format)                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        width of bitmap                                  *
*                   h:        height of bitmap                                 *
*                   bitmap:   address at which the bitmap data resides         *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_Bitmap(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char*bitmap)
{
    int i, j;
    unsigned short*bitmap_ptr = (unsigned short*)
    bitmap;
    GLCD_SetWindow(x, y, w, h);
    glcd_wr_cmd(0x22);
    glcd_wr_dat_start();
    for(i = (h-1)*w; i>-1; i -= w)
    {
        for(j = 0; j<w; j++)
        {
            glcd_wr_dat_only(bitmap_ptr[i+j]);
        }
    }
    glcd_wr_dat_stop();
}


/*******************************************************************************
* Scroll content of the whole display for dy pixels vertically                 *
*   Parameter:      dy:       number of pixels for vertical scroll             *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_ScrollVertical(unsigned int dy)
{
#if ( LANDSCAPE == 0)
    static unsigned int y = 0;
    y = y+dy;
    while(y>=HEIGHT) y -= HEIGHT;
    if(Himax)
    {
        glcd_wr_reg(0x01, 0x08);
        glcd_wr_reg(0x14, y >> 8); /* VSP MSB                            */
        glcd_wr_reg(0x15, y&0xFF); /* VSP LSB                            */
    }
    else
    {
        glcd_wr_reg(0x6A, y);
        glcd_wr_reg(0x61, 3);
    }
#endif
}


/*******************************************************************************
* Write a command to the LCD controller                                        *
*   Parameter:      cmd:      command to write to the LCD                      *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrCmd(unsigned char cmd)
{
    glcd_wr_cmd(cmd);
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrReg(unsigned char reg, unsigned short val)
{
    glcd_wr_reg(reg, val);
}


