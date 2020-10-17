/**
 * @file    mcu_uart.h
 * @author  meng_yu
 * @brief   mcu uart function header file
 * @version 0.0.1
 * @date    2020-10-11
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_UART_H_
#define _MCU_UART_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define MCU_UART_PORT GPIOA
#define MCU_UART_TX_PIN PIN9
#define MCU_UART_RX_PIN PIN10

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
extern void mcu_uart_init(dword_t pclk2, dword_t bound);
extern int mcu_uart_put_char(int32_t ch);
extern int mcu_uart_get_char(void);
extern int mcu_uart_chk_char(void);


#endif /* _MCU_UART_H_ */


