#ifndef _SDRAM_H_
#define _SDRAM_H_

#define SDRAM_BANK_ADDR                          ((uint32_t)0xD0000000)
#define SDRAM_SIZE                               ((uint32_t)0x2000000)
#define SDRAM_SWAP_BANK_ADDR                     ((uint32_t)0x80000000)
#define SDRAM_REMAP_BANK_ADDR                    ((uint32_t)0x00000000)
 
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval);
void SDRAM_Init(void);
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);

#endif

