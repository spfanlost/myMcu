

/**
 * @file    retarget.c
 * @author  meng_yu
 * @brief   'Retarget' layer for target-dependent low level functions realize
 * @version 0.0.1
 * @date    2020-09-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include <rt_misc.h>
#include "common.h"
#include "mcu.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

FILE __stdout;
FILE __stdin;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
int fputc(int c, FILE *f)
{
  while (!LL_USART_IsActiveFlag_TC(USART1))
    ;
  LL_USART_TransmitData8(USART1, c);
  return c;
}

int fgetc(FILE *f)
{
  return LL_USART_ReceiveData8(USART1);
}

int ferror(FILE *f)
{
  return EOF;
}

void _ttywrch(int c)
{
  LL_USART_TransmitData8(USART1, c);
}

void _sys_exit(int return_code)
{

label:

  goto label; /* endless loop */
}
