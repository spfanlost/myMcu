/**
 * @file    main.c
 * @author  meng_yu
 * @brief   Blinky for my MCU EVAL test
 * @version 0.0.1
 * @date    2020-10-01
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_sys.h"
#include "mcu_isr.h"
#include "mcu_uart.h"
#include "mcu_adc.h"
#include "drv_led.h"
#if defined(USE_LCD_EN)
#include "drv_lcd.h"
#include "touch.h"
#include "mylogo.h"
#endif

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

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/

//VECT_TAB_SRAM


char text[40];
/* Import external variables from IRQ.c file                                  */

extern dword_t SystemCoreClock;

byte_t flag = 1;


/*----------------------------------------------------------------------------
      MAIN function
*----------------------------------------------------------------------------*/
int main(void)
{
 	u8 x=0;
	u8 lcd_id[12];				//存放LCD ID字符串
    word_t AD_value = 0;
    NVIC_SetPriorityGrouping(NVIC_GROUP4);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000000); //Generate interrupt each 1 ms
    drv_led_init();
    mcu_uart_init(84, 115200);
    mcu_adc_init();

#ifdef USE_LCD_EN
//    GLCD_SetBackgroundColor (GLCD_COLOR_BLACK);
//    GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
//    GLCD_ClearScreen        ();
//    GLCD_SetFont            (&GLCD_Font_12x24);
//    GLCD_DrawString         (0, 0*24, "  STM32407ZG  ");
//    GLCD_DrawString         (0, 1*24, "    Blinky    ");
//    GLCD_DrawString         (0, 2*24, " www.keil.com ");
#endif
    LCD_Init();
    TP_Init();
    Pen_data.Key_Sta=0;
    Load_TFT_surface();

// 	TOUCH_InitHard();
//    TOUCH_Calibration();       /* 四点触摸校准 */
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
//	GLCD_DrawBitmap(24, 6*24 , 107, 107, (unsigned char*)mylogo);
    while (1)
    {
        Read_TP(0);
        if(Pen_data.Key_Sta)            //触摸屏被按下
        {
            LOG_INFO("key_sts:%x, X:%d,Y:%d\r\n",Pen_data.Key_Sta,Pen_data.X0,Pen_data.Y0);
            Pen_data.Key_Sta=0;
            if(Pen_data.X0>210&&Pen_data.Y0<20)
            {
                Load_TFT_surface();//清除
            }
            else if(Pen_data.X0>0&&Pen_data.Y0<20&&Pen_data.X0<30)
            {
                POINT_COLOR=BLUE;//设置字体为蓝色
            }
            else if(Pen_data.X0>30&&Pen_data.Y0<20&&Pen_data.X0<60)
            {
                POINT_COLOR=RED;//设置字体为红色
            }
            else if(Pen_data.X0>60&&Pen_data.Y0<20&&Pen_data.X0<90)
            {
                POINT_COLOR=YELLOW;//设置字体为黄色
            }
            else if(Pen_data.X0>90&&Pen_data.Y0<20&&Pen_data.X0<120)
            {
                POINT_COLOR=BLACK;//设置字体为黑色
            }
            else if(Pen_data.X0>120&&Pen_data.Y0<20&&Pen_data.X0<150)
            {
                POINT_COLOR=GREEN;//设置字体为绿色
            }
            else if(Pen_data.X0>150&&Pen_data.Y0<20&&Pen_data.X0<180)
            {
                POINT_COLOR=GRAY;//设置字体为灰色
            }
            else if(Pen_data.X0>180&&Pen_data.Y0<20&&Pen_data.X0<210)
            {
                POINT_COLOR=BROWN;//设置字体为棕色
            }
            else
            {
                Draw_Big_Circle(Pen_data.X0,Pen_data.Y0,POINT_COLOR);//画图
            }
        }
        else
        {
            delay_ms(1);  //没有按键按下的时候
        }
    }
//  	while(1)
//	{
//		switch(x)
//		{
//			case 0:LCD_Clear(WHITE);break;
//			case 1:LCD_Clear(BLACK);break;
//			case 2:LCD_Clear(BLUE);break;
//			case 3:LCD_Clear(RED);break;
//			case 4:LCD_Clear(MAGENTA);break;
//			case 5:LCD_Clear(GREEN);break;
//			case 6:LCD_Clear(CYAN);break;
//			case 7:LCD_Clear(YELLOW);break;
//			case 8:LCD_Clear(BRRED);break;
//			case 9:LCD_Clear(GRAY);break;
//			case 10:LCD_Clear(LGRAY);break;
//			case 11:LCD_Clear(BROWN);break;
//		}
//		POINT_COLOR=RED;
//		LCD_ShowString(10,40,240,24,24,"Apollo STM32",POINT_COLOR);
//		LCD_ShowString(10,80,240,24,24,"TFTLCD TEST",POINT_COLOR);
//		LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK",POINT_COLOR);
// 		LCD_ShowString(10,130,240,16,16,lcd_id,POINT_COLOR);		//显示LCD ID
//		LCD_ShowString(10,150,240,12,12,"2015/12/9",POINT_COLOR);
//	    x++;
//		if(x==12)x=0;
//        drv_led_toggle(LED1_PIN|LED2_PIN);
//		delay_ms(1000);
//	}

//    while(1)
//    {
//        mcu_adc_start_conv();
//        if(get_sys_ticks()%(500*1000) <= 30)
//        {
//            drv_led_toggle(LED1_PIN|LED2_PIN);
//            AD_value = mcu_adc_get_conv(); /* Read AD  value                 */
//            //LOG_INFO ("Value=%#X\r\n", AD_value);
//#if 0 //def USE_LCD_EN
//            GLCD_SetForegroundColor (GLCD_COLOR_YELLOW);
//            sprintf(text, "Value=%#X", AD_value);
//            GLCD_DrawString(0, 3*24, text);
//            GLCD_SetForegroundColor (GLCD_COLOR_RED);
//            GLCD_DrawBargraph(0, 4*24, 14*12, 24, AD_value*200/0xFFF);
//            if(flag)
//            {
//                flag = 0;
//                GLCD_DrawBitmap(24, 6*24 , 107, 107, (unsigned char*)mylogo);
//            }
//#endif
//        }
//    }
}


