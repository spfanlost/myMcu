/**
 * @file    uart.h
 * @author  meng_yu
 * @brief   Low level serial definitions function header file
 * @version 0.0.1
 * @date    2020-09-09
 * 
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _UART_H_
#define _UART_H_

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
extern void uart_init(uint32_t pclk2,uint32_t bound);
extern int uart_put_char(int32_t ch);
extern int uart_get_char(void);
extern int uart_chk_char(void);


#endif /* _UART_H_ */

