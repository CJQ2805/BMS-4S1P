#include "flash.h"

/**
@brief  CJQ2805 
		�ο�STM32�������̶�Flash���в���
		�µ�HAL���У�FLASH_PageErase ����BANKs����
*/

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ
//����ֵ����Ӧ����
static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

void STMFLASH_Read(u32 u32addr, u32 len, u32  *data)
{
	u32 i = 0;
	
	for( i = 0; i < len; i++)
	{
		data[i] = STMFLASH_ReadWord(u32addr);
		u32addr += 4; 
	}

}


u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}



/*STM32G031  2K һ��������64������  8���ֽ�д*/
#define STM_SECTOR_SIZE	2048		//2Kһ������


u8 u8FlashErase_Flag = 0;
static FLASH_EraseInitTypeDef EraseInitStruct = {0};

u8 STMFLASH_Write(u32 WriteAddr,uint64_t *pBuffer,int WordSize)	
{
	static u32 u32sector_addr = 0;	   //������ַ
	u32 u32retain_addr = 0;
	u32 u32flash_err = 0;  
	u8 flash_erro_flag = 0;
	if(WriteAddr<STM32_ADDR_FLASH_SECTOR_0||(WriteAddr>=(STM32_ADDR_FLASH_SECTOR_0+1024*STM32_FLASH_SIZE)))return 1;//�Ƿ���ַ
	
	u32retain_addr = GetPage(WriteAddr);
//	offaddr=WriteAddr-STM32_ADDR_FLASH_SECTOR_0;		//ʵ��ƫ�Ƶ�ַ.
//	u32retain_addr= offaddr/STM_SECTOR_SIZE;			//������ַ  0~63 for STM32G031
	
	if(u32sector_addr != u32retain_addr)
	{
		u8FlashErase_Flag = 1;
		u32sector_addr = u32retain_addr;
	}
	HAL_FLASH_Unlock();					//����			
	if(1 == u8FlashErase_Flag)				//��Ҫ����
	{
		delay_us(10);
		u8FlashErase_Flag = 0;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.Page = u32sector_addr;  //ʵ�ʵ�ַ��page0 ��ʼ
		EraseInitStruct.NbPages = 1;
		if(HAL_FLASHEx_Erase(&EraseInitStruct, &u32flash_err) != HAL_OK)
		{		
			printf("flash =  %d \r\n",u32flash_err);
			flash_erro_flag = 1;	
			goto flash_end;
		}
	}
	
	for(u32 i = 0; i< WordSize; i++ )
	{
		delay_ms(1);
		
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+(i*8), pBuffer[i]) != HAL_OK)	//һ��д��64�ֽ�
		{
			printf("д���쳣��\r\n");
			flash_erro_flag = 1;
			goto flash_end;
		}	
	}

flash_end:
	delay_ms(10);	
	HAL_FLASH_Lock();	//����
//	if(1 == flash_erro_flag)
//	return 1;
//	else	
	return 0; 	
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��



//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(uint32_t addr)
{
	MSR MSP, r0
	BX r14
}


void jump_application(u32 u32addr)
{
	typedef void (*foo)(void);
	foo jump2app;
	
	
    if (((*(__IO uint32_t*)u32addr) & 0x2FFE0000 ) == 0x20000000)
    {
		TIM_DeInit();
		UART_DeInit();
		HAL_RCC_DeInit();
		HAL_DeInit();

		//__disable_irq();

		jump2app=(foo)*(vu32*)(u32addr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)u32addr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();			
	}
}

