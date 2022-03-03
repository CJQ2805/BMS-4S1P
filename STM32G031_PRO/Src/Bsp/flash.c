#include "flash.h"

/**
@brief  CJQ2805 
		�ο�STM32�������̶�Flash���в���
		�µ�HAL���У�FLASH_PageErase ����BANKs����
		ֻ��64bitд��һ�Σ������жϰ�ҳ����
*/


//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ
//����ֵ����Ӧ����
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)


/*STM32G031  2K һ��������64������  8���ֽ�д*/
#define STM_SECTOR_SIZE	2048		//2Kһ������


uint64_t STMFLASH_BUF[32];//�����32���ֽ�
static u8 u8FlashErase_Flag = 0;
void STMFLASH_Write(u32 WriteAddr,unsigned char *pBuffer,int WordSize)	
{
	u32 u32sector_addr;	   //������ַ
	u32 u32retain_addr;
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	FLASH_EraseInitTypeDef flash_init;
	u32 u32flash_err = 0;  	
	if(WriteAddr<STM32_ADDR_FLASH_SECTOR_0||(WriteAddr>=(STM32_ADDR_FLASH_SECTOR_0+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	HAL_FLASH_Unlock();					//����
	offaddr=WriteAddr-STM32_ADDR_FLASH_SECTOR_0;		//ʵ��ƫ�Ƶ�ַ.
	u32retain_addr= offaddr/STM_SECTOR_SIZE;			//������ַ  0~63 for STM32G031
	
	if(u32sector_addr != u32retain_addr)
	{
		u8FlashErase_Flag = 1;
		u32sector_addr = u32retain_addr;
	}
	
	if(u8FlashErase_Flag)				//��Ҫ����
	{
		u8FlashErase_Flag = 0;
		flash_init.Banks = FLASH_BANK_1;
		flash_init.TypeErase = FLASH_TYPEERASE_PAGES;
		flash_init.Page = u32sector_addr;
		flash_init.NbPages = 1;
		if(HAL_FLASHEx_Erase(&flash_init, &u32flash_err) != HAL_OK)	//�����������
		{
			printf("Erase error");
		}	
		
	}else 
	{		
		for(i = 0; i< WordSize; i++ )
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+(i*8), pBuffer[i]) != HAL_OK)//һ��д��64�ֽ�
			{
				printf("д���쳣");		
			}				
		}
	}
	 
	HAL_FLASH_Lock();		//����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}



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
		ADC_DeInit();
		UART_DeInit();
		IIC_DeInit();
		HAL_RCC_DeInit();
		HAL_DeInit();		
		//__disable_irq();
		jump2app=(foo)*(vu32*)(u32addr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)u32addr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();			
	}
}

