#include "SMBus.h"

void SMBus_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();  
  
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET);  

}

/**
	@brief SMB 起始信号	
*/
void SMB_Start(void)
{
	SDA_OUT();
	SMB_SDA = 1;
	SMB_SCL = 1;
	delay_us(50);
	SMB_SDA = 0;
	delay_us(50);
	SMB_SCL = 0;
}


/**
	@breif SMB 停止信号
*/
void SMB_Stop(void)
{ 
	SDA_OUT();
	SMB_SCL = 0;
	SMB_SDA = 0;    
	delay_us(50);			
	SMB_SDA = 1;
	delay_us(10);	
	SMB_SCL = 1;
	delay_us(50);	
}


/**
	@breif SMB接收应答
*/
int SMB_Wait_Ack(void)
{    
	uint16_t u16tmptime = 0;
	SMB_SCL = 0;	
	SDA_IN();
	delay_us(100);
	SMB_SCL = 1;
	delay_us(10);
	while(READ_SDA) // 0应答位 
	{
		u16tmptime ++;
		if(u16tmptime > 2500)
		{
			SMB_Stop();
			printf("SMB SDA low timeout \r\n");
			return -1;
		}
	
	} 


	return 0;
}



/**
	@breif SMB发送应答
*/
void SMB_ACK(uint8_t ack)
{  
	SDA_OUT();
	SMB_SCL = 0;
	if(ack) // 应答
	{
		SMB_SDA = 0;
	} 
	else 
	{
		SMB_SDA = 1;
	}
	delay_us(70);
	SMB_SCL = 0;
	delay_us(5); 	
	SMB_SCL = 1;
	delay_us(80); 
	SMB_SCL = 0;	
	delay_us(4);	
}


/**
	@breif 写字节数据
*/
void SMB_HostWriteByte(uint8_t dat)
{
	uint8_t i = 0;
	
	delay_us(10); 
	SMB_SCL = 0;
	SDA_OUT();		

	for(i=0;i<8;i++)
	{       
		if(dat & 0x80)
		{
			SMB_SDA = 1;
		}	
		else
		{
			SMB_SDA = 0;
		}
		dat <<= 1;
		delay_us(50);
		SMB_SCL = 1;
		delay_us(50);
		SMB_SCL = 0;
		delay_us(10);
	}
	
}


/**
	@breif 读字节数据
*/
uint8_t SMB_HostReadByte(uint8_t u8ack)
{
	
	unsigned char i=0,dat = 0;
	SDA_IN();	
	SMB_SCL = 0;
	delay_us(100);
	for(i=0;i<8;i++)
	{
		SMB_SCL = 0;
		delay_us(50);
		dat <<= 1; 
		SMB_SCL = 1;
		delay_us(50);
		if(READ_SDA == 1)
			dat |= 0x01;
		
		SMB_SCL = 0;
		delay_us(10);		
	}	
	
	if(u8ack)
	SMB_ACK(1);
	else
	SMB_ACK(0);
	
	return dat;	
	
}


/**
	@breif 完整读一个字节数据
*/
uint8_t SMB_HostRead(uint8_t u8SMB_Address ,uint8_t u8regAddr)
{
	uint8_t temp = 0;
	int itimeout = 0;
	delay_ms(10);
	
	SMB_Start();
	
	SMB_HostWriteByte(u8SMB_Address);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		printf("SMB read time out");
		return 0;
	}
	
	SMB_HostWriteByte(u8regAddr);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		printf("SMB read time out");		
		return 0;
	}
	
	SMB_Start();
	
	SMB_HostWriteByte(u8SMB_Address + 1);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		printf("SMB read time out");		
		return 0;
	}

	temp = SMB_HostReadByte(0);

	SMB_Stop();
	
	return temp;

}


int Read_bq40z50_data(uint8_t SMB_address, uint8_t commond)
{
	int itimeout;
	uint16_t data1 = 0, data2 = 0, CrcData = 0;	
	uint8_t Crcbuff[3], crc = 0;
	delay_ms(10);
	
	SMB_Start();
	
	SMB_HostWriteByte(SMB_address);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		return -1;
	}
	
	SMB_HostWriteByte(commond);
	itimeout = SMB_Wait_Ack();	
	if(itimeout == -1)
	{
		return -1;
	}
	
	SMB_Start();
	
	SMB_HostWriteByte(SMB_address + 1);
	Crcbuff[0] = (SMB_address + 1);
	itimeout = SMB_Wait_Ack();	
	if(itimeout == -1)
	{
		return -1;
	}
		
	data1   = SMB_HostReadByte(1);
	data2   = SMB_HostReadByte(1);
	CrcData = SMB_HostReadByte(1);
	
	Crcbuff[1] = data1;
	Crcbuff[2] = data2;
	
	crc = CRC8(Crcbuff,3);
	
	if(crc != CrcData)
	{
		return -1;
	}
	
	SMB_Stop();
	
	data1 = data1|data2<<8;
	
	return data1;
	

}




/* 写0x16 0x00 0x06 0x00   读 0x16 0x44 0x17 0x04 0x06 0x00 0x99 0x20*/
int Read_BQ40Z50_ManuFBAccess_data(uint8_t SMB_address, uint16_t commond)
{
	uint32_t au32data_buf[10] = {0};	
	uint16_t CrcData = 0;
	uint8_t Crcbuff[8], crc = 0;	
	int itimeout = 0;
	uint8_t data1 = 0xff;

	uint32_t  u32resual_data = 0;
	
	uint8_t au8data[2] = {0};
	delay_ms(10);
	
	SMB_Start();

	SMB_HostWriteByte(0x16);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		return -1;
	}
	
	SMB_HostWriteByte(0x00);
	itimeout = SMB_Wait_Ack();	
	if(itimeout == -1)
	{
		return -1;
	}

	au8data[0] =  commond&0x00ff;	
	au8data[1] = (commond>>8); 
	
	for(uint8_t i = 0; i < 2; i++)
	{
		SMB_HostWriteByte(au8data[i]);
		itimeout = SMB_Wait_Ack();	
		if(itimeout == -1)
		{
			return -1;
		}	
	}

	SMB_Stop();
	
	
	SMB_Start();

	SMB_HostWriteByte(SMB_address);
	itimeout = SMB_Wait_Ack();
	if(itimeout == -1)
	{
		return -1;
	}
	
	SMB_HostWriteByte(0x44);
	itimeout = SMB_Wait_Ack();	
	if(itimeout == -1)
	{
		return -1;
	}
	
	SMB_Start();
	
	SMB_HostWriteByte(SMB_address + 1);
	Crcbuff[0] = SMB_address + 1;
	itimeout = SMB_Wait_Ack();	
	if(itimeout == -1)
	{
		return -1;
	}


	data1 = SMB_HostReadByte(1);
	Crcbuff[1] = data1;
	
	for(uint8_t i = 0; i < data1 ; i++)
	{
		au32data_buf[i] = SMB_HostReadByte(1);
		Crcbuff[2+i] = au32data_buf[i];
	}		
	
	CrcData = SMB_HostReadByte(0);	

	
	//最后一字节这里应该是0，NACK，但是实际测试发现BQ40Z50 不回复NACK 直接STOP 错误概率大大降低
	SMB_Stop();
	
	if(4 == data1)
	{
		crc = CRC8(Crcbuff,6);
		if(crc != CrcData)
		return -1;
		
		u32resual_data = (au32data_buf[0]<<24)+(au32data_buf[1]<<16) + (au32data_buf[2]<<8) + au32data_buf[3];	
	}
	else if(6 == data1){
		
		crc = CRC8(Crcbuff,8);	
		if(crc != CrcData)
		return -1;
		
		u32resual_data = (au32data_buf[2]<<24)+(au32data_buf[3]<<16) + (au32data_buf[4]<<8) + au32data_buf[5];		
	}

	
	return u32resual_data;
}



