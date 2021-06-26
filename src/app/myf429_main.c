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
#include "sdram.h"
#include "ltdc.h"

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

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25
#define PLL_N      360
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      8
/*!< Time out for HSE start up */
#define HSE_STARTUP_TIMEOUT    ((uint16_t)0x05000)

void Set_System_Clock(void) {

    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    //SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */

    /* Reset the RCC clock configuration to the default reset state ------------*/
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;            //设置HISON,开启内部高速RC振荡
    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;                     //CFGR清零
    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;            //HSEON,CSSON,PLLON清零
    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;                  //PLLCFGR恢复复位值
    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;            //HSEBYP清零,外部晶振不旁路
    /* Disable all interrupts */
    RCC->CIR = 0x00000000;                      //禁止RCC时钟中断

    /*            PLL (clocked by HSE) used as System clock source                */
    RCC->CR |= 1 << 16;                         //HSE 开启
    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CR & (1 << 17);
        StartUpCounter++;                       //等待HSE RDY
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if((RCC->CR & (1 << 17)) != 0) {
        HSEStatus = (uint32_t)0x01;
    } else {
        HSEStatus = (uint32_t)0x00;
    }
    if(HSEStatus == (uint32_t)0x01) {
        /* Select regulator voltage output Scale 1 mode */
        RCC->APB1ENR |= 1 << 28;                //电源接口时钟使能
        PWR->CR |= 3 << 14;                     //高性能模式,时钟可到180Mhz
        /* HCLK = SYSCLK / 1*/
        RCC->CFGR |= 0 << 4;                    //HCLK 不分频
        /* PCLK1 = HCLK / 4*/
        RCC->CFGR |= 5 << 10;                   //APB1 4分频
        /* PCLK2 = HCLK / 2*/
        RCC->CFGR |= 4 << 13;                   //APB2 2分频
        /* Disable the main PLL */
        RCC->CR &= ~(1 << 24);                  //关闭主PLL
        /* Configure the main PLL */
        RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) |
                       (PLL_Q << 24) | (1 << 22); //配置主PLL,PLL时钟源来自HSE
        /* Enable the main PLL */
        RCC->CR |= 1 << 24;                     //打开主PLL
        /* Wait till the main PLL is ready */
        while((RCC->CR & (1 << 25)) == 0) {
        }
        /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
        FLASH->ACR |= 1 << 8;                   //指令预取使能.
        FLASH->ACR |= 1 << 9;                   //指令cache使能.
        FLASH->ACR |= 1 << 10;                  //数据cache使能.
        FLASH->ACR |= 5 << 0;                   //5个CPU等待周期.
        /* Select the main PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(0 << 3));
        RCC->CFGR |= 2 << 0;
        /* Wait till the main PLL is used as system clock source */
        while((RCC->CFGR & (uint32_t)(3 << 2)) != 2 << 2) {
        }
    } else {
        /* If HSE fails to start-up, the application will have wrong clock
             configuration. User can add here some code to deal with this error */
    }

    //配置向量表
#ifdef  VECT_TAB_RAM
    SCB->VTOR = (1 << 29) | (0x0 & (uint32_t)0xFFFFFE00); //NVIC向量表偏移寄存器,VTOR低9位保留,基址|偏移量.
#else
    SCB->VTOR = 0 | (0x0 & (uint32_t)0xFFFFFE00);
#endif

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
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART1_IRQn);
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

  /* GPIO Ports Clock Enable */
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
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
void MX_DMA2D_Init(void)
{

  /* Peripheral clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
  LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M);
  LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);//LL_DMA2D_OUTPUT_MODE_ARGB8888
  LL_DMA2D_SetLineOffset(DMA2D, 0);
  LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);//LL_DMA2D_INPUT_MODE_ARGB8888
  LL_DMA2D_FGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_NO_MODIF);
  LL_DMA2D_FGND_SetAlpha(DMA2D, 0);
  LL_DMA2D_FGND_SetLineOffset(DMA2D, 0);

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
    u32 mem = 0;
    SystemClock_Config();
    SystemCoreClockUpdate();

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(RCC_IRQn);

    task_init();
    gpio_init();
    button_init();
    uart_init();
    SDRAM_Init();
    LOG_INFO("Device: %s, SystemCoreClock:%dMHz\r", DEVICE_STR, SystemCoreClock / 1000000);
    // LOG_INFO("SDRAM:%x %x\r\n", SDRAM_BANK_ADDR,LCD429_FRAME_BUFFER);
    //memset((void *)LCD429_FRAME_BUFFER, 200, BUFFER_OFFSET*2);
    LCD429_InitHard();
    LOG_INFO("LCD BUFFER:%x %x\r\n", LCD429_FRAME_BUFFER,BUFFER_OFFSET);
    // MX_DMA2D_Init();
    while (1)
    {
        LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_13);
        LL_mDelay(100);
        LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_13);
        LL_mDelay(100);
        task_now = task_now->pnxt;
        if (task_now->flag)
        {
            task_now->func();
        }
    }
}
