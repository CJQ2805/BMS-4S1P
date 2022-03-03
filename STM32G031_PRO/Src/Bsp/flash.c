#include "flash.h"

/**
@brief  CJQ2805 
		参考STM32其他工程对Flash进行操作
		新的HAL库中，FLASH_PageErase 新增BANKs参数
		只能64bit写入一次，所以判断按页擦除
*/


//读取指定地址的半字(16位数据)
//faddr:读地址
//返回值，对应数据
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)


/*STM32G031  2K 一个扇区，64个扇区  8个字节写*/
#define STM_SECTOR_SIZE	2048		//2K一个扇区


uint64_t STMFLASH_BUF[32];//最多是32个字节
static u8 u8FlashErase_Flag = 0;
void STMFLASH_Write(u32 WriteAddr,unsigned char *pBuffer,int WordSize)	
{
	u32 u32sector_addr;	   //扇区地址
	u32 u32retain_addr;
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	FLASH_EraseInitTypeDef flash_init;
	u32 u32flash_err = 0;  	
	if(WriteAddr<STM32_ADDR_FLASH_SECTOR_0||(WriteAddr>=(STM32_ADDR_FLASH_SECTOR_0+1024*STM32_FLASH_SIZE)))return;//非法地址
	
	HAL_FLASH_Unlock();					//解锁
	offaddr=WriteAddr-STM32_ADDR_FLASH_SECTOR_0;		//实际偏移地址.
	u32retain_addr= offaddr/STM_SECTOR_SIZE;			//扇区地址  0~63 for STM32G031
	
	if(u32sector_addr != u32retain_addr)
	{
		u8FlashErase_Flag = 1;
		u32sector_addr = u32retain_addr;
	}
	
	if(u8FlashErase_Flag)				//需要擦除
	{
		u8FlashErase_Flag = 0;
		flash_init.Banks = FLASH_BANK_1;
		flash_init.TypeErase = FLASH_TYPEERASE_PAGES;
		flash_init.Page = u32sector_addr;
		flash_init.NbPages = 1;
		if(HAL_FLASHEx_Erase(&flash_init, &u32flash_err) != HAL_OK)	//擦除这个扇区
		{
			printf("Erase error");
		}	
		
	}else 
	{		
		for(i = 0; i< WordSize; i++ )
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr+(i*8), pBuffer[i]) != HAL_OK)//一次写入64字节
			{
				printf("写入异常");		
			}				
		}
	}
	 
	HAL_FLASH_Lock();		//上锁
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}



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
		ADC_DeInit();
		UART_DeInit();
		IIC_DeInit();
		HAL_RCC_DeInit();
		HAL_DeInit();		
		//__disable_irq();
		jump2app=(foo)*(vu32*)(u32addr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)u32addr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();			
	}
}

