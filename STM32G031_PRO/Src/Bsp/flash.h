#ifndef __FLASH_H
#define __FLASH_H

#include "bsp.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define STM32_FLASH_SIZE 	64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1              	//ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_WAITETIME  	50000          	//FLASH�ȴ���ʱʱ��

//FLASH��ʼ��ַ
#define STM32_ADDR_FLASH_SECTOR_0    ((u32)0x08000000) 		//STM32 FLASH����ʼ��ַ
#define STM32_ADDR_FLASH_SECTOR_10	 ((u32)0x08005000)




#define STM32_BOOTLOADER_BASE      STM32_ADDR_FLASH_SECTOR_0
#define STM32_FIRMWARE_INFO_BASE  (STM32_ADDR_FLASH_SECTOR_10 - 0x40)
#define STM32_APP_BASE             STM32_ADDR_FLASH_SECTOR_10



u8 STMFLASH_GetStatus(void);				  //���״̬
u8 STMFLASH_WaitDone(u16 time);				  //�ȴ���������
 
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,unsigned char *pBuffer,int WordSize);
void jump_application(u32 u32addr);
//����д��


#endif
