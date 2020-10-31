/**
 * @file    app_paint.c
 * @author  meng_yu
 * @brief   &#& function realize
 * @version 0.0.1
 * @date    2020-10-31
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "task.h"
#include "mcu.h"
#include "mcu_isr.h"
#include "app_paint.h"
#include "drv_lcd.h"
#include "touch.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/
TASK  task_paint;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/
void app_paint_process(void);

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/

void app_paint_init(void)
{
    pen_dat.Key_Sta=0;
    touch_load_draw_ui();
    POINT_COLOR=RED;
    task_clr(&task_paint);
    task_add_nxt(&task_paint, app_paint_process, TRUE);
}

void app_paint_process(void)
{
    touch_scan(0);
    if(pen_dat.Key_Sta)
    {
        LOG_INFO("key_sts:%x, X:%d,Y:%d\r\n",pen_dat.Key_Sta,pen_dat.X0,pen_dat.Y0);
        pen_dat.Key_Sta=0;
        if(pen_dat.X0>210&&pen_dat.Y0<20)
        {
            touch_load_draw_ui();
        }
        else if(pen_dat.X0>0&&pen_dat.Y0<20&&pen_dat.X0<30)
        {
            POINT_COLOR=BLUE;
        }
        else if(pen_dat.X0>30&&pen_dat.Y0<20&&pen_dat.X0<60)
        {
            POINT_COLOR=RED;
        }
        else if(pen_dat.X0>60&&pen_dat.Y0<20&&pen_dat.X0<90)
        {
            POINT_COLOR=YELLOW;
        }
        else if(pen_dat.X0>90&&pen_dat.Y0<20&&pen_dat.X0<120)
        {
            POINT_COLOR=BLACK;
        }
        else if(pen_dat.X0>120&&pen_dat.Y0<20&&pen_dat.X0<150)
        {
            POINT_COLOR=GREEN;
        }
        else if(pen_dat.X0>150&&pen_dat.Y0<20&&pen_dat.X0<180)
        {
            POINT_COLOR=GRAY;
        }
        else if(pen_dat.X0>180&&pen_dat.Y0<20&&pen_dat.X0<210)
        {
            POINT_COLOR=BROWN;
        }
        else
        {
            touch_draw_point(pen_dat.X0,pen_dat.Y0,POINT_COLOR);
        }
    }
    else
    {
        delay_ms(1);
    }
}


