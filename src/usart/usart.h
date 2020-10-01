

/**
 * @file    uart.h
 * @author  meng_yu
 * @brief   Low level serial definitions function header file
 * @version 0.0.1
 * @date    2020-09-09
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _USART_H_
#define _USART_H_

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
extern void usart_init(dword_t pclk2, dword_t bound);
extern int usart_put_char(int32_t ch);
extern int usart_get_char(void);
extern int usart_chk_char(void);

#endif /* _USART_H_ */

