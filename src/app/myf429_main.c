/**
 * @file    myf429_main.c
 * @author  meng_yu
 * @brief   Blinky for my MCU EVAL test
 * @version 0.0.1
 * @date    2020-10-01
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "task.h"
#include "mcu.h"
#include "mcu_isr.h"

/*-----------------------------------------------------------------------------------
  Private declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Extern variables declaration
-----------------------------------------------------------------------------------*/
extern dword_t SystemCoreClock;

/*-----------------------------------------------------------------------------------
  Global variables definition
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions declaration
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
  Local functions definition
-----------------------------------------------------------------------------------*/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 360, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(180000000);
  LL_SetSystemCoreClock(180000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

void uart_init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  /**USART1 GPIO Configuration
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX
    */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART1_IRQn);


  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);

  LL_USART_Enable(USART1);

  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void gpio_init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None 
  * @retval None
  */
void button_init(void)
{

}

/**
  * @brief  Function to manage Button push
  * @param  None
  * @retval None
  */
void button_callback(void)
{
  /* Turn LED1 Off on User push-button press (allow to restart sequence) */
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_0);
}

/*----------------------------------------------------------------------------
      MAIN function
*----------------------------------------------------------------------------*/
int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  SystemClock_Config();
  SystemCoreClockUpdate();

  task_init();
  gpio_init();
  button_init();
  uart_init();
  LOG_INFO("Device: %s, SystemCoreClock:%dMHz\r", DEVICE_STR, SystemCoreClock / 1000000);
  while (1)
  {
    LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_13);
    LL_mDelay(500);
    LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_13);
    LL_mDelay(500);
    task_now = task_now->pnxt;
    if (task_now->flag)
    {
      task_now->func();
    }
  }
}
