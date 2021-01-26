

/**
 * @file    cmdline.h
 * @author  meng_yu
 * @brief   common_cmd function header file
 * @version 0.0.1
 * @date    2020-09-01
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#ifndef _COMMON_CMD_H_
#define _COMMON_CMD_H_

#include "mcu.h"

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/
enum ASCII
{
  NUL = 0,
  SOH,
  STX,
  ETX,
  EOT,
  ENQ,
  ACK,
  BEL,
  BS,
  HT,
  LF,
  VT,
  FF,
  CR,
  SO,
  SI,
  DLE,
  DC1,
  DC2,
  DC3,
  DC4,
  NAK,
  SYN,
  ETB,
  CAN,
  EM,
  SUB,
  ESC,
  FS,
  GS,
  RS,
  US,
  SPACE, //32
  DEL = 127
};

enum CMD_VAL
{
  CMD_INIT,
  CMD_VAL1,
  CMD_VAL2,
  CMD_VAL3,
  CMD_NUM
};

typedef struct
{
  void (*pfn)(char *str, byte_t *pos);
  const char *cmd;
  byte_t paranum;
  byte_t rev[2];
} uart_cmd_t;

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define UART_RX_BUF_SIZE 128
#define RX_NEXT_IDX(cur_idx) ((cur_idx + 1) % UART_RX_BUF_SIZE)
#define RX_PRE_IDX(cur_idx) ((cur_idx - 1) % UART_RX_BUF_SIZE)
#define RX_ADVANCE_IDX(p) \
  do                      \
  {                       \
    p = RX_NEXT_IDX(p);   \
  } while (0)
#define RX_ADVANCE_PIDX(p) \
  do                       \
  {                        \
    p = RX_PRE_IDX(p);     \
  } while (0)
#define RX_PUT_CHAR(c)        \
  do                          \
  {                           \
    UARTRXBuf[RXBuf_pos] = c; \
  } while (0)
#define RX_GET_CHAR(c)        \
  do                          \
  {                           \
    c = UARTRXBuf[RXBuf_pos]; \
  } while (0)
#define RX_INI_POS() \
  do                 \
  {                  \
    RXBuf_pos = 0;   \
  } while (0)

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/

#define CMD_CHK_CHAR() (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
#define CMD_GET_CHAR() LL_USART_ReceiveData8(USART1)
#define CMD_PUT_CHAR(c) LL_USART_TransmitData8(USART1, c)

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void uart_cmd(void);
void common_cmd_isr(void);

#endif /* _COMMON_CMD_H_ */
