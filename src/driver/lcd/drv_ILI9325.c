

/**
 * @file    drv_ili9325.c
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
#include "drv_ili9325.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/
dword_t drv_lcd_ili9325_bl(byte_t sta);
dword_t drv_lcd_ili9325_bl_init(void);
void drv_lcd_ili9325_bus_init(void);
static __inline void drv_lcd_wr_cmd(byte_t cmd);
static __inline void drv_lcd_wr_dat(word_t dat);
static __inline word_t drv_lcd_rd_dat(void);
static __inline void drv_lcd_wr_reg(byte_t reg, word_t val);
static word_t drv_lcd_rd_reg(byte_t reg);
void drv_lcd_wr_n_reg(byte_t reg, dword_t n, word_t val);

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/
struct lcd_drv drv_ili9325 =
{
    drv_lcd_ili9325_bl_init,
    drv_lcd_ili9325_bl,
    drv_lcd_ili9325_bus_init,
    drv_lcd_wr_cmd,
    drv_lcd_wr_dat,
    drv_lcd_rd_dat,
    drv_lcd_wr_reg,
    drv_lcd_rd_reg,
    drv_lcd_wr_n_reg,
};

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/
/**
 * @brief  This function LCD Backlight api
 * @param  sta: 1: turn on Backlight;0:turn off Backlight
 * @note:  call
 */
dword_t drv_lcd_ili9325_bl(byte_t sta)
{
    if(sta)
    {
        mcu_io_set(GPIOB, PIN15);
    }
    else
    {
        mcu_io_reset(GPIOB, PIN15);
    }
    return SUCCESS;
}

dword_t drv_lcd_ili9325_bl_init(void)
{
    mcu_io_clk_enable(GPIOB_CLK);
    mcu_io_config(GPIOB, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    drv_lcd_ili9325_bl(FALSE);
    return SUCCESS;
}

/*-----------------------------------------------------------------------------------
  LCD FSMC functions definition
-----------------------------------------------------------------------------------*/
void drv_lcd_ili9325_fsmc_init(void)
{

    RCC->AHB3ENR |= (1UL << 0);     /* Enable FSMC clock                  */
    FSMC_Bank1->BTCR[(4-1)*2+1] =   /* Bank4 NOR/SRAM timing register     */
    /* configuration                      */
    (0 << 28) | /* FSMC AccessMode A                  */
    (2 << 24) | /* Data Latency                       */
    (2 << 20) | /* CLK Division                       */
    (0 << 16) | /* Bus Turnaround Duration            */
    (9 << 8) |  /* Data SetUp Time                    */
    (1 << 4) |  /* Address Hold Time                  */
    (5 << 0);   /* Address SetUp Time                 */

    FSMC_Bank1->BTCR[(4-1)*2+0] = /* Control register                   */

    (0 << 19) | /* Write burst disabled               */
    (0 << 15) | /* Async wait disabled                */
    (0 << 14) | /* Extended mode disabled             */
    (0 << 13) | /* NWAIT signal is disabled           */
    (1 << 12) | /* Write operation enabled            */

    (0 << 11) | /* NWAIT signal is active one data    */
                /* cycle before wait state            */
    (0 << 10) | /* Wrapped burst mode disabled        */
    (0 << 9) |  /* Wait signal polarity active low    */
    (0 << 8) |  /* Burst access mode disabled         */
    (1 << 4) |  /* Memory data  bus width is 16 bits  */
    (0 << 2) |  /* Memory type is SRAM                */
    (0 << 1) |  /* Address/Data Multiplexing disable  */
    (1 << 0);   /* Memory Bank enable                 */
}
static void drv_lcd_ili9325_fsmc_gpio_init(void)
{
    static dword_t fsmc_inited = 0;
    if(fsmc_inited)
        return;
    fsmc_inited = 1;
    mcu_io_clk_enable(GPIOA_CLK|GPIOD_CLK|GPIOE_CLK|GPIOF_CLK|GPIOG_CLK);

    /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE)                           */
    /* PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1)                 */
    mcu_io_config(GPIOD, PIN0 | PIN1 | PIN4 | PIN5 | PIN8 | PIN9 | PIN10 | PIN14 | PIN15,
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    mcu_io_af_config(GPIOD, 0, 12); //PD.00(D2),AF12
    mcu_io_af_config(GPIOD, 1, 12); //PD.01(D3),AF12
    mcu_io_af_config(GPIOD, 4, 12); //PD.04(NOE),AF12
    mcu_io_af_config(GPIOD, 5, 12); //PD.05(NWE),AF12
    mcu_io_af_config(GPIOD, 8, 12); //PD.08(D13),AF12
    mcu_io_af_config(GPIOD, 9, 12); //PD.09(D14),AF12
    mcu_io_af_config(GPIOD, 10, 12); //PD.10(D15),AF12
    mcu_io_af_config(GPIOD, 14, 12); //PD.14(D0),AF12
    mcu_io_af_config(GPIOD, 15, 12); //PD.15(D1),AF12

    /* PE.07(D4), PE.08(D5),  PE.09(D6),  PE.10(D7), PE.11(D8)                  */
    /* PE.12(D9), PE.13(D10), PE.14(D11), PE.15(D12)                            */
    mcu_io_config(GPIOE, PIN7 | PIN8 | PIN9 | PIN10 | PIN11 | PIN12 | PIN13 | PIN14 | PIN15,
        GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    mcu_io_af_config(GPIOE, 7, 12); //PE.07(D4),AF12
    mcu_io_af_config(GPIOE, 8, 12); //PE.08(D5),AF12
    mcu_io_af_config(GPIOE, 9, 12); //PE.09(D6),AF12
    mcu_io_af_config(GPIOE, 10, 12); //PE.10(D7),AF12
    mcu_io_af_config(GPIOE, 11, 12); //PE.11(D8),AF12
    mcu_io_af_config(GPIOE, 12, 12); //PE.12(D9),AF12
    mcu_io_af_config(GPIOE, 13, 12); //PE.13(D10),AF12
    mcu_io_af_config(GPIOE, 14, 12); //PE.14(D11),AF12
    mcu_io_af_config(GPIOE, 15, 12); //PE.15(D12),AF12

    /* PF.00(A0 (RS)) */
    mcu_io_config(GPIOF, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    mcu_io_af_config(GPIOF, 12, 12); //PE.12(A6),AF12

    /* PG.12(NE4 (LCD/CS)) - CE1(LCD /CS)                                       */
    mcu_io_config(GPIOG, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_RES);
    mcu_io_af_config(GPIOG, 12, 12); //PE.12(NE4),AF12
}

void drv_lcd_ili9325_bus_init(void)
{
    drv_lcd_ili9325_fsmc_init();
    drv_lcd_ili9325_fsmc_gpio_init();
}

static __inline void drv_lcd_wr_cmd(byte_t cmd)
{
    LCD_REG16 = cmd;
}

static __inline void drv_lcd_wr_dat(word_t dat)
{
    LCD_DAT16 = dat;
}

static __inline word_t drv_lcd_rd_dat(void)
{
    return (LCD_DAT16);
}

static __inline void drv_lcd_wr_reg(byte_t reg, word_t val)
{
    drv_lcd_wr_cmd(reg);
    drv_lcd_wr_dat(val);
}

static word_t drv_lcd_rd_reg(byte_t reg)
{
    drv_lcd_wr_cmd(reg);
    return (drv_lcd_rd_dat());
}

static __inline void wr_dat_start(void)
{
    /* only used for SPI interface */
}

static __inline void wr_dat_stop(void)
{
    /* only used for SPI interface */
}

void drv_lcd_wr_n_reg(byte_t reg, dword_t n, word_t val)
{
  drv_lcd_wr_cmd(reg);
  wr_dat_start();
  while (n--)
  {
    drv_lcd_wr_dat(val);
  }
  wr_dat_stop();
}

