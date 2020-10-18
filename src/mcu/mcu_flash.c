/**
 * @file    mcu_flash.c
 * @author  meng_yu
 * @brief   mcu inside flash function realize
 * @version 0.0.1
 * @date    2020-10-18
 *
 * @copyright Copyright (c) 2020 imyumeng@qq.com All rigthts reserved.
 */
#include "common.h"
#include "mcu.h"
#include "mcu_isr.h"
#include "mcu_flash.h"

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

void mcu_flash_unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

void mcu_flash_lock(void)
{
    FLASH->CR |= (dword_t)1 << 31;
}

/**
 * @brief  This function mcu_flash_get_status
 * @param  None
 * @return flash status
 * @retval enum flash_status
 * @note:  call
 */
byte_t mcu_flash_get_status(void)
{
    dword_t res = 0;
    res = FLASH->SR;
    if(res & (1 << 16))return FLASH_STS_BUSY;
    else if(res & (1 << 4))return FLASH_STS_ABNOAMAL;
    else if(res & (1 << 5))return FLASH_STS_ABNOAMAL;
    else if(res & (1 << 6))return FLASH_STS_ABNOAMAL;
    else if(res & (1 << 7))return FLASH_STS_ABNOAMAL;
    return FLASH_STS_NOAMAL;
}

/**
 * @brief  This function mcu_flash_wait_done
 * @param  time: wait_done time
 * @return flash status
 * @retval enum flash_status
 * @note:  call
 */
byte_t mcu_flash_wait_done(dword_t time)
{
    byte_t res = FLASH_STS_NOAMAL;
    do {
        res = mcu_flash_get_status();
        if(res != FLASH_STS_BUSY)
            break;
        delay_us(10);
        time--;
    } while(time);
    if(time == 0)
        res = FLASH_STS_TIMEOUT;
    return res;
}

/**
 * @brief  This function mcu_flash_erase_sector
 * @param  sector_addr: sector address, range is 0~11
            0~3,is 16K sector; 4, is 64K sector; 5~11,is 128K sector.
 * @return flash status
 * @retval enum flash_status
 * @note:  call
 */
byte_t mcu_flash_erase_sector(dword_t sector_addr)
{
    byte_t res = FLASH_STS_NOAMAL;
    res = mcu_flash_wait_done(200000);      //wait flash done
    if(res == FLASH_STS_NOAMAL)
    {
        FLASH->CR &= ~(3 << 8);             //clear PSIZE config
        FLASH->CR |= 2 << 8;                //set to 32bit,ensure VCC=2.7~3.6V!!
        FLASH->CR &= ~(0X1F << 3);          //clear config
        FLASH->CR |= sector_addr << 3;      //set erase sector address
        FLASH->CR |= 1 << 1;                //erase sector
        FLASH->CR |= 1 << 16;               //erase sector start
        res = mcu_flash_wait_done(200000);  //wait flash done
        if(res != FLASH_STS_BUSY)
        {
            FLASH->CR &= ~(1 << 1);         //clear sector erase flag
        }
    }
    return res;
}

/**
 * @brief  This function mcu_flash_write_dword
 * @param  faddr: write_dword address, must be dowrd align
 * @param  dat: write_dword data
 * @return flash status
 * @retval enum flash_status
 * @note:  call
 */
byte_t mcu_flash_write_dword(dword_t faddr, dword_t dat)
{
    byte_t res;
    res = mcu_flash_wait_done(0XFF);
    if(res == FLASH_STS_NOAMAL)
    {
        FLASH->CR &= ~(3 << 8);     //clear PSIZE config
        FLASH->CR |= 2 << 8;        //set to 32bit,ensure VCC=2.7~3.6V!!
        FLASH->CR |= 1 << 0;        //program enable
        *(vu32*)faddr = dat;        //write data
        res = mcu_flash_wait_done(0XFF); //wait flash done
        if(res != FLASH_STS_BUSY)
        {
            FLASH->CR &= ~(1 << 0); //clear PG bit.
        }
    }
    return res;
}

/**
 * @brief  This function mcu_flash_read_dword
 * @param  faddr: write_dword address, must be dowrd align
 * @return write_dword data
 * @note:  call
 */
dword_t mcu_flash_read_dword(dword_t faddr)
{
    return *(vu32*)faddr;
}

/**
 * @brief  This function mcu_flash_get_flash_sector
 * @param  addr: address
 * @return address is in sector num
 * @retval 0~
 * @note:  call
 */
byte_t mcu_flash_get_flash_sector(dword_t addr)
{
    if(addr < ADDR_FLASH_SECTOR_1)return 0;
    else if(addr < ADDR_FLASH_SECTOR_2)return 1;
    else if(addr < ADDR_FLASH_SECTOR_3)return 2;
    else if(addr < ADDR_FLASH_SECTOR_4)return 3;
    else if(addr < ADDR_FLASH_SECTOR_5)return 4;
    else if(addr < ADDR_FLASH_SECTOR_6)return 5;
    else if(addr < ADDR_FLASH_SECTOR_7)return 6;
    else if(addr < ADDR_FLASH_SECTOR_8)return 7;
    else if(addr < ADDR_FLASH_SECTOR_9)return 8;
    else if(addr < ADDR_FLASH_SECTOR_10)return 9;
    else if(addr < ADDR_FLASH_SECTOR_11)return 10;
    else if(addr < ADDR_FLASH_SECTOR_12)return 11;
    else if(addr < ADDR_FLASH_SECTOR_13)return 12;
    else if(addr < ADDR_FLASH_SECTOR_14)return 13;
    else if(addr < ADDR_FLASH_SECTOR_15)return 14;
    else if(addr < ADDR_FLASH_SECTOR_16)return 15;
    else if(addr < ADDR_FLASH_SECTOR_17)return 16;
    else if(addr < ADDR_FLASH_SECTOR_18)return 17;
    else if(addr < ADDR_FLASH_SECTOR_19)return 18;
    else if(addr < ADDR_FLASH_SECTOR_20)return 19;
    else if(addr < ADDR_FLASH_SECTOR_21)return 20;
    else if(addr < ADDR_FLASH_SECTOR_22)return 21;
    else if(addr < ADDR_FLASH_SECTOR_23)return 22;
    return 7;
}

/**
 * @brief  This function mcu_flash write data at address
    特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
             写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
             写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
             没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.
    该函数对OTP区域也有效!可以用来写OTP区!
    OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
 * @param  wr_addr: write data address, must be dowrd align
 * @param  dw_buf: write data
 * @param  wr_num: write data dword num
 * @note:  call
 */
void mcu_flash_write(dword_t wr_addr, dword_t *dw_buf, dword_t wr_num)
{
    byte_t status = 0;
    dword_t addrx = 0;
    dword_t endaddr = 0;
    if(wr_addr < STM32_FLASH_BASE || wr_addr % 4)
        return; //address invld
    mcu_flash_unlock();
    FLASH->ACR &= ~(1 << 10);           //flash erase doing,must close data fetch!!
    addrx = wr_addr;                    //wr dat start addr
    endaddr = wr_addr + wr_num * 4;     //wr dat end addr
    if(addrx < 0X1FFF0000)              //flash erase only in main flash range
    {
        while(addrx < endaddr)          //check if not FFFFFFFF range,erase first)
        {
            if(mcu_flash_read_dword(addrx) != 0XFFFFFFFF)
            {
                status = mcu_flash_erase_sector(mcu_flash_get_flash_sector(addrx));
                if(status)
                {
                    break;    //flash write abnormal
                }
            }
            else
            {
                addrx += 4;
            }
        }
    }
    if(status == 0)
    {
        while(wr_addr < endaddr)
        {
            if(mcu_flash_write_dword(wr_addr, *dw_buf))
            {
                break;  //flash write abnormal
            }
            wr_addr += 4;
            dw_buf++;
        }
    }
    FLASH->ACR |= 1 << 10;      //flash erase done,open data fetch
    mcu_flash_lock();
}

/**
 * @brief  This function mcu_flash read data at address
 * @param  rd_addr: read data address, must be dowrd align
 * @param  dw_buf: write data
 * @param  wr_num: write data dword num
 * @note:  call
 */
void mcu_flash_read(dword_t rd_addr, dword_t *dw_buf, dword_t rd_num) {
    dword_t i;
    for(i = 0; i < rd_num; i++)
    {
        dw_buf[i] = mcu_flash_read_dword(rd_addr);
        rd_addr += 4;
    }
}

//////////////////////////////////////////for test///////////////////////////////////////////
/**
 * @brief  This function mcu_flash_test_write
 * @param  wr_addr: write data address, must be dowrd align
 * @param  WriteData: write data 1 dword
 * @note:  call
 */
void mcu_flash_test_write(dword_t wr_addr, dword_t wr_dat)
{
    mcu_flash_write(wr_addr, &wr_dat, 1);
}


