/**
 * @file    bsp_myf407_lcd.c
 * @author  meng_yu
 * @brief   myf407 board lcd function realize
 * @version 0.0.1
 * @date    2020-10-17
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#include "common.h"
#include "mcu.h"
#include "mcu_io.h"
#include "bsp_myf407_lcd.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/
dword_t bsp_lcd_ili9341_bl(byte_t sta);
void bsp_lcd_ili9341_bus_init(void);
dword_t bsp_lcd_ili9341_write_cmd(word_t cmd);
dword_t bsp_lcd_ili9341_write_data(byte_t *data, dword_t cnt);
dword_t bsp_lcd_ili9341_read_data(byte_t *data, dword_t cnt);

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
struct bsp_lcd bsp_myf407_lcd =
{
    BUS_LCD_16B,
    bsp_lcd_ili9341_bus_init,
    bsp_lcd_ili9341_bl,
    bsp_lcd_ili9341_write_cmd,
    bsp_lcd_ili9341_write_data,
    bsp_lcd_ili9341_read_data,
};

/**
 * @brief  This function LCD Backlight api
 * @param  sta: 1: turn on Backlight;0:turn off Backlight
 * @note:  call
 */
dword_t bsp_lcd_ili9341_bl(byte_t sta)
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

dword_t bsp_lcd_ili9341_bl_init(void)
{
    mcu_io_clk_enable(GPIOB_CLK);
    mcu_io_config(GPIOB, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);
    bsp_lcd_ili9341_bl(FALSE);
    return SUCCESS;
}

void bsp_lcd_ili9341_fsmc_init(void)
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
static void bsp_lcd_ili9341_fsmc_gpio_init(void)
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

void bsp_lcd_ili9341_bus_init(void)
{
    bsp_lcd_ili9341_fsmc_init();
    bsp_lcd_ili9341_fsmc_gpio_init();
    bsp_lcd_ili9341_bl_init();
}


dword_t bsp_lcd_ili9341_write_cmd(word_t cmd)
{
    LCD->LCD_REG=cmd;
    return 0;
}

dword_t bsp_lcd_ili9341_write_data(byte_t *data, dword_t len)
{
    byte_t i;
    word_t *p = (word_t *)data;
    for(i=0; i<len; i++)
    {
        LCD->LCD_RAM=*(p++);
    }
    return 0;
}

dword_t bsp_lcd_ili9341_read_data(byte_t *data, dword_t len)
{
    byte_t i;
    word_t *p = (word_t *)data;
    for(i=0; i<len; i++)
    {
        *(p++) = LCD->LCD_RAM;
    }
    return 0;
}

