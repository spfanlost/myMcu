/**
 * @file    task.h
 * @author  meng_yu
 * @brief   task function header file
 * @version 0.0.1
 * @date    2020-10-31
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _TASK_H_
#define _TASK_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/
typedef struct _TASK
{
    void          (*func)(void);
    dword_t         flag;
    dword_t         parameter;
    struct _TASK *pnxt;         // used as hooked test, ppre == pnxt
    struct _TASK *ppre;         // or ppre == &task or pnxt == &task
} TASK;

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
#define cTASK_IDLE      0
#define cTASK_BUSY      1
#define cTASK_ACTIVE    1

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/
extern TASK *task_now;

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/

void task_init(void);
void task_clr(TASK *tp);
void task_add_pre(TASK *tp, fptr_t func, dword_t flag);
void task_add_nxt(TASK *tp, fptr_t func, dword_t flag);
void task_del(TASK *tp);
void task_for_each(void);

#endif /* _TASK_H_ */

