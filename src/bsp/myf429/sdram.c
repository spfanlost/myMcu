
#include "mcu.h"
#include "common.h"
#include "mcu_isr.h"
#include "sdram.h"


/*
 ****************************************************************************
 * PB5 --> SDCKE1  | PE0 --> NBL0  | PF0 --> A0    | PG0 --> A10            *
 * PB6 --> SDNE1   | PE1 --> NBL1  | PF1 --> A1    | PG1 --> A11            *
 * ----------------| PE8 --> D5    | PF2 --> A2    | PG2 --> A12            *
 * PC0 --> SDNWE   | PE9 --> D6    | PF3 --> A3    | PG4 --> BA0            *
 * ----------------| PE10--> D7    | PF4 --> A4    | PG5 --> BA1            *
 * PD0 --> D2      | PE11--> D8    | PF5 --> A5    | PG8 --> SDCLK          *
 * PD1 --> D3      | PE12--> D9    | PF11--> SDNRAS| PG15--> SDNCAS         *
 * PD8 --> D13     | PE13--> D10   | PF12--> A6    |                        *
 * PD9 --> D14     | PE14--> D11   | PF13--> A7    |                        *
 * PD10--> D15     | PE15--> D12   | PF14--> A8    |                        *
 * PD14--> D0      |---------------| PF15--> A9    |                        *
 * PD15--> D1      |                                                        *
 ****************************************************************************
 */
  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PC0   ------> FMC_SDNWE
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  PB5   ------> FMC_SDCKE1
  PB6   ------> FMC_SDNE1
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  */
static void sdram_gpio_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_12;
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3
                          |LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_11|LL_GPIO_PIN_12
                          |LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
    LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_4
                          |LL_GPIO_PIN_5|LL_GPIO_PIN_8|LL_GPIO_PIN_15;
    LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10
                          |LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14
                          |LL_GPIO_PIN_15|LL_GPIO_PIN_0|LL_GPIO_PIN_1;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_14
                          |LL_GPIO_PIN_15|LL_GPIO_PIN_0|LL_GPIO_PIN_1;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

//向SDRAM发送命令
//bankx:0,向BANK5上面的SDRAM发送指令
//      1,向BANK6上面的SDRAM发送指令
//cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
//refresh:自刷新次数(cmd=3时有效)
//regval:模式寄存器的定义
//返回值:0,正常;1,失败.
u8 SDRAM_Send_Cmd(u8 bankx, u8 cmd, u8 refresh, u16 regval) {
    u32 retry = 0;
    u32 tempreg = 0;
    tempreg |= cmd << 0;			//设置指令
    tempreg |= 1 << (4 - bankx);		//设置发送指令到bank5还是6
    tempreg |= refresh << 5;		//设置自刷新次数
    tempreg |= regval << 9;			//设置模式寄存器的值
    FMC_Bank5_6->SDCMR = tempreg;	//配置寄存器
    while((FMC_Bank5_6->SDSR & (1 << 5))) { //等待指令发送完成
        retry++;
        if(retry > 0X1FFFFF)return 1;
    }
    return 0;
}

//SDRAM初始化
void SDRAM_Init(void) 
{

    u32 sdctrlreg = 0, sdtimereg = 0;
    u16 mregval = 0;
    
    // RCC->AHB3ENR |= 1 << 0; //使能FMC时钟
    LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_FMC);
    sdram_gpio_init();
    
    // FMC_SDRAMInitStructure.FMC_Bank = FMC_Bank2_SDRAM;
    // FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_9b;
    // FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_13b;
    // FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
    // FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
    // FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_3; 
    // FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable;
    // FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2;  
    // FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_Enable;
    // FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_0;
    // FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
    /* SDRAM bank control register configuration */
    sdctrlreg |= 1 << 0;				//9位列地址
    sdctrlreg |= 2 << 2;				//13位行地址
    sdctrlreg |= 1 << 4;				//16位数据位宽
    sdctrlreg |= 1 << 6;				//4个内部存区(4 BANKS)
    sdctrlreg |= 3 << 7;				//3个CAS延迟
    sdctrlreg |= 0 << 9;				//允许写访问
    sdctrlreg |= 0 << 13;				//读通道延迟0个HCLK
    FMC_Bank5_6->SDCR[1] = sdctrlreg;	//Set FMC_SDCR1 register.
    sdctrlreg = 0;
    sdctrlreg |= 2 << 10;				//SDRAM时钟=HCLK/2=192M/2=96M=10.4ns  
    sdctrlreg |= 1 << 12;				//使能突发访问
    FMC_Bank5_6->SDCR[0] = sdctrlreg;	
    //look reference manual 37.7.3 SDRAM controller functional description (P1611) for details
    //RBURST and RPIPE must be programmed in the FMC_SDCR1 register.

    // FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = 2;      
    // FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 7;  
    // FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = 4; 
    // FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = 7; 
    // FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = 2;
    // FMC_SDRAMTimingInitStructure.FMC_RPDelay = 2; 
    // FMC_SDRAMTimingInitStructure.FMC_RCDDelay = 2;    
    sdtimereg |= 1 << 0;				//加载模式寄存器到激活时间的延迟为2个时钟周期
    sdtimereg |= 6 << 4;				//退出自刷新延迟为7个时钟周期
    sdtimereg |= 3 << 8;				//自刷新时间为6个时钟周期  //4
    sdtimereg |= 6 << 12;				//行循环延迟为6个时钟周期  //7
    sdtimereg |= 1 << 16;				//恢复延迟为2个时钟周期
    sdtimereg |= 1 << 20;				//行预充电延迟为2个时钟周期
    sdtimereg |= 1 << 24;				//行到列延迟为2个时钟周期
    FMC_Bank5_6->SDTR[1] = sdtimereg;	//设置FMC BANK5 SDRAM时序寄存器

    SDRAM_Send_Cmd(1, 1, 1, 0);		//时钟配置使能 FMC_Command_Mode_CLK_Enabled
    LL_mDelay(10);
    SDRAM_Send_Cmd(1, 2, 1, 0);		//对所有存储区预充电 FMC_Command_Mode_PALL
    SDRAM_Send_Cmd(1, 3, 8, 0);		//设置自刷新次数 FMC_Command_Mode_AutoRefresh
    // tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
    //                  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
    //                  SDRAM_MODEREG_CAS_LATENCY_3           |
    //                  SDRAM_MODEREG_OPERATING_MODE_STANDARD |
    //                  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    mregval |= 0 << 0;					//设置突发长度:8(可以是1/2/4/8)  //TODO: 000
    mregval |= 0 << 3;					//设置突发类型:连续(可以是连续/交错)
    mregval |= 3 << 4;					//设置CAS值:3(可以是2/3)
    mregval |= 0 << 7;					//设置操作模式:0,标准模式
    mregval |= 1 << 9;					//设置突发写模式:1,单点访问
    SDRAM_Send_Cmd(1, 4, 1, mregval);	//设置SDRAM的模式寄存器 FMC_Command_Mode_LoadMode

    //刷新频率计数器(以SDCLK频率计数),计算方法:
    //COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
    //我们使用的SDRAM刷新周期为64ms,SDCLK=192/2=96Mhz,行数为8192(2^13).
    //所以,COUNT=64*1000*96/8192-20=730
    /* Set the refresh rate counter */
    /* (7.81 us x Freq) - 20 */
    /* Set the device refresh counter */
    FMC_Bank5_6->SDRTR = 321 << 1;		//设置刷新频率计数器 //
}

//在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FMC_SDRAM_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u32 n) {
    for(; n != 0; n--) {
        *(vu8*)(SDRAM_BANK_ADDR + WriteAddr) = *pBuffer;
        WriteAddr++;
        pBuffer++;
    }
}

//在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FMC_SDRAM_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u32 n) {
    for(; n != 0; n--) {
        *pBuffer++ = *(vu8*)(SDRAM_BANK_ADDR + ReadAddr);
        ReadAddr++;
    }
}
