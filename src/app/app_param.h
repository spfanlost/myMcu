/**
 * @file    app_param.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-10-27
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _APP_PARAM_H_
#define _APP_PARAM_H_

#include "touch.h"

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/

#define PARAM_SAVE_TO_FLASH

#ifdef PARAM_SAVE_TO_FLASH
    #define PARAM_ADDR   ADDR_FLASH_SECTOR_6  /* 0x080E0000 Flash最后128K扇区用来存放参数 */
#endif
#define PARAM_VER               (0x00000104)

struct app_param
{
    uint32_t version;
    uint32_t touch_adj_done;
    struct touch_param touch_dat;
};

extern struct app_param g_param;

void app_load_param(void);
void app_save_param(void);


#endif /* _APP_PARAM_H_ */

