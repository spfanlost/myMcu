/**
 * @file    app_param.c
 * @author  meng_yu
 * @brief   paramater function realize
 * @version 0.0.1
 * @date    2020-10-27
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

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

#include "common.h"
#include "mcu.h"
#include "mcu_flash.h"
#include "app_param.h"

struct app_param g_param;

void app_load_param(void)
{
#ifdef PARAM_SAVE_TO_FLASH
    mcu_flash_read(PARAM_ADDR, (dword_t *)&g_param, sizeof(struct app_param)>>2);
#endif
    //fill default paramater
    if (g_param.version != PARAM_VER)
    {
        g_param.version = PARAM_VER;
        g_param.touch_adj_done = FALSE;
        app_save_param();
    }
}
void app_save_param(void)
{
#ifdef PARAM_SAVE_TO_FLASH
    mcu_flash_write(PARAM_ADDR, (dword_t *)&g_param, sizeof(struct app_param)>>2);
#endif
}

