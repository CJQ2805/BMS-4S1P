#include "flash.h"

/**
@brief  CJQ2805 
		参考STM32其他工程对Flash进行操作
		新的HAL库中，FLASH_PageErase 新增BANKs参数
*/

//读取指定地址的半字(16位数据)
//faddr:读地址
//返回值，对应数据
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



/*STM32G031  2K 一个扇区，64个扇区  8个字节写*/
#define STM_SECTOR_SIZE	2048		//2K一个扇区


u8 u8FlashErase_Flag = 0;
static FLASH_EraseInitTypeDef EraseInitStruct = {0};

u8 STMFLASH_Write(u32 WriteAddr,uint64_t *pBuffer,int WordSize)	
{
	static u32 u32sector_addr = 0;	   //扇区地址
	u32 u32retain_addr = 0;
	u32 u32flash_err = 0;  
	u8 flash_erro_flag = 0;
	if(WriteAddr<STM32_ADDR_FLASH_SECTOR_0||(WriteAddr>=(STM32_ADDR_FLASH_SECTOR_0+1024*STM32_FLASH_SIZE)))return 1;//非法地址
	
	u32retain_addr = GetPage(WriteAddr);
//	offaddr=WriteAddr-STM32_ADDR_FLASH_SECTOR_0;		//实际偏移地址.
//	u32retain_addr= offaddr/STM_SECTOR_SIZE;			//扇区地址  0~63 for STM32G031
	
	if(u32sector_addr != u32retain_addr)
	{
		u8FlashErase_Flag = 1;
		u32sector_addr = u32retain_addr;
	}
	HAL_FLASH_Unlock();					//解锁			
	if(1 == u8FlashErase_Flag)				//需要擦除
	{
		delay_us(10);
		u8FlashErase_Flag = 0;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.Page = u32sector_addr;  //实际地址从page0 开始
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
		
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+(i*8), pBuffer[i]) != HAL_OK)	//一次写入64字节
		{
			printf("写入异常！\r\n");
			flash_erro_flag = 1;
			goto flash_end;
		}	
	}

flash_end:
	delay_ms(10);	
	HAL_FLASH_Lock();	//上锁
//	if(1 == flash_erro_flag)
//	return 1;
//	else	
	return 0; 	
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数



//设置栈顶地址
//addr:栈顶地址
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

		jump2app=(foo)*(vu32*)(u32addr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)u32addr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();			
	}
}

