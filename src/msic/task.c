/**
 * @file    task.c
 * @author  meng_yu
 * @brief   &#& function realize
 * @version 0.0.1
 * @date    2020-10-31
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
#include "task.h"


TASK *task_now;
TASK  task_nul;

static void task_idle(void)
{
    ;
}

void task_init(void)
{
    task_now      = &task_nul;
    task_nul.func =  task_idle;
    task_nul.flag =  0;
    task_nul.ppre = &task_nul;
    task_nul.pnxt = &task_nul;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Do Not Insert Or Remove Task In ISR !!
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void task_clr(TASK *tp)
{
    tp->func = NULL;
    tp->flag = 0;
    tp->pnxt = tp;
    tp->ppre = tp;
}

static void task_dup_check(TASK *tp)
{
    TASK *temp = &task_nul;
    dword_t num = 0;
    while(temp->pnxt != &task_nul)
    {
        if(tp == temp->pnxt)
        {
            LOG_INFO("[%s],dup task:%08x\r",__FUNCTION__,temp->pnxt);
            //ASSERT(0, M_SYS, F_TASK);
        }
        temp = temp->pnxt;
        num++;
        if(num > 25)
            break;
    }
}

void task_add_pre(TASK *tp, fptr_t func, dword_t flag)
{
    task_dup_check(tp);
    tp->func = func;
    tp->flag = flag;
    tp->pnxt = task_now;
    tp->ppre = task_now->ppre;

    task_now->ppre->pnxt = tp;
    task_now->ppre       = tp;
}

void task_add_nxt(TASK *tp, fptr_t func, dword_t flag)
{
    task_dup_check(tp);
    tp->func = func;
    tp->flag = flag;
    tp->pnxt = task_now->pnxt;
    tp->ppre = task_now;

    task_now->pnxt->ppre = tp;
    task_now->pnxt       = tp;
}

void task_del(TASK *tp)
{
    tp->ppre->pnxt = tp->pnxt;
    tp->pnxt->ppre = tp->ppre;
    tp->flag = 0;
    tp->func = NULL;

    if(task_now == tp) // bugfix! This line is must!
    {
        if (task_now == tp->ppre)
            LOG_INFO("[%s]tp:%08x\r", __FUNCTION__, tp);
        //ASSERT(task_now != tp->ppre, M_SYS, F_TASK);
        task_now = tp->ppre;
    }

    tp->pnxt = tp;
    tp->ppre = tp;
}

void task_for_each(void)
{
    TASK *temp = &task_nul;
    dword_t num = 0;

    while(temp->pnxt != &task_nul)
    {
        LOG_INFO("[%s],task:%08x\r",__FUNCTION__,temp->pnxt);
        temp = temp->pnxt;
        num++;
        if(num > 10)
            break;
    }
}


