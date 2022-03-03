#ifndef __FLASH_H
#define __FLASH_H

#include "bsp.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define STM32_FLASH_SIZE 	64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 	1              	//使能FLASH写入(0，不是能;1，使能)
#define FLASH_WAITETIME  	50000          	//FLASH等待超时时间

//FLASH起始地址
#define STM32_ADDR_FLASH_SECTOR_0    ((u32)0x08000000) 		//STM32 FLASH的起始地址
#define STM32_ADDR_FLASH_SECTOR_10	 ((u32)0x08005000)




#define STM32_BOOTLOADER_BASE      STM32_ADDR_FLASH_SECTOR_0
#define STM32_FIRMWARE_INFO_BASE  (STM32_ADDR_FLASH_SECTOR_10 - 0x40)
#define STM32_APP_BASE             STM32_ADDR_FLASH_SECTOR_10



u8 STMFLASH_GetStatus(void);				  //获得状态
u8 STMFLASH_WaitDone(u16 time);				  //等待操作结束
 
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,unsigned char *pBuffer,int WordSize);
void jump_application(u32 u32addr);
//测试写入


#endif
