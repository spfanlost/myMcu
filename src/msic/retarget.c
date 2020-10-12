

/**
 * @file    retarget.c
 * @author  meng_yu
 * @brief   'Retarget' layer for target-dependent low level functions realize
 * @version 0.0.1
 * @date    2020-09-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include <rt_misc.h>
#include "mcu_uart.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/
#if defined (__CC_ARM)
#pragma import (__use_no_semihosting_swi )


struct __FILE
{
int handle; /* Add whatever you need here */
};


#endif

FILE __stdout;
FILE __stdin;

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/
int fputc ( int c, FILE * f )
{
    return ( mcu_uart_put_char ( c ) );
}


int fgetc ( FILE * f )
{
    return ( mcu_uart_get_char () );
}


int ferror ( FILE * f )
{
    /* Your implementation of ferror */
    return EOF;
}


void _ttywrch ( int c )
{
    mcu_uart_put_char ( c );
}


void _sys_exit ( int return_code )
{

label:

    goto label; /* endless loop */
}


