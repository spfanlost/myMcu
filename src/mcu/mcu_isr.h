/**
 * @file    mcu_isr.h
 * @author  meng_yu
 * @brief   &#& function header file
 * @version 0.0.1
 * @date    2020-09-29
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */

#ifndef _MCU_ISR_H_
#define _MCU_ISR_H_

/*-----------------------------------------------------------------------------------
  Exported types
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Exported macro
-----------------------------------------------------------------------------------*/
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
enum nvic_group
{
    NVIC_GROUP0,//group0:0 bit Preemption Priority,4 bit SubPriority
    NVIC_GROUP1,//group1:1 bit Preemption Priority,3 bit SubPriority
    NVIC_GROUP2,//group2:2 bit Preemption Priority,2 bit SubPriority
    NVIC_GROUP3,//group3:3 bit Preemption Priority,1 bit SubPriority
    NVIC_GROUP4,//group4:4 bit Preemption Priority,0 bit SubPriority
};

/*-----------------------------------------------------------------------------------
  Exported variables
-----------------------------------------------------------------------------------*/
extern volatile qword_t ticks;

/*-----------------------------------------------------------------------------------
  Exported functions
-----------------------------------------------------------------------------------*/
void NVIC_Config(IRQn_Type IRQn, byte_t PreemptionPrio, byte_t SubPrio, byte_t Enable);


#endif /* _ISR_H_ */


