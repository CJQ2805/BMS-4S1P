#include "iic_master.h"

/**
	@author CJQ2805  2022/03/03
	@brief 模拟IIC 采用开漏输出模式，一定需要上拉电阻
	
	这里还没测试过，不知道类位带操作能不能用
*/


t_I2C_bit  tI2C_bit;
void I2C_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();  
  
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_SET);  

}

/**
	@brief I2C 起始信号	
*/
void I2C_Start(void)
{      
	delay_us(I2C_FREQUENCY);
	I2C_SDA = 1;		
	delay_us(I2C_FREQUENCY);
	I2C_SCL = 1;	
	delay_us(I2C_FREQUENCY);			
	I2C_SDA = 0;	
	delay_us(I2C_FREQUENCY);			
	I2C_SCL = 0;	
	delay_us(I2C_FREQUENCY);
}

/**
	@breif I2C 停止信号
*/
void I2C_Stop(void)
{                   		
	I2C_SDA = 0;    
	delay_us(I2C_FREQUENCY);			
	I2C_SCL = 1;
	delay_us(I2C_FREQUENCY);	
	I2C_SDA = 1;	
	delay_us(I2C_FREQUENCY);
	delay_us(I2C_FREQUENCY);

}


/**
	@breif I2C接收应答
*/
void I2C_HostReceiveAck(void)
{    
	uint8_t u8tmptime = 0;
	delay_us(I2C_FREQUENCY);
	I2C_SCL = 1;
	I2C_SDA = 1;
	
	delay_us(I2C_FREQUENCY); 	
	while(READ_SDA) // 0无应答位 
	{
		u8tmptime ++;
		if(u8tmptime > 250)
		{
			tI2C_bit.bI2C_ack = 0;
			I2C_SCL = 0;	
			printf("i2c wait timeout \r\n");
			return;
		}

	} 
	
	tI2C_bit.bI2C_ack = 1;
	I2C_SCL = 0;	
}



/**
	@breif I2C发送应答
*/
void I2C_HostSendAck(uint8_t ack)
{  
	I2C_SCL = 0;
	
	if(ack == 1) // 不应答
	{
		I2C_SDA = 1;
	} 
	else 
	{
		I2C_SDA = 0;
	}
	
	I2C_SCL = 1;
	delay_us(I2C_FREQUENCY); 	
	I2C_SCL = 0;
}



/**
	@breif 写字节数据
*/
void I2C_HostWriteByte(uint8_t dat)
{
	uint8_t i = 0;
	I2C_SCL = 0;
	
	for(i=0;i<8;i++)
	{       
		if((dat << i) & 0x80)
		{
			I2C_SDA = 1;
		}	
		else
		{
			I2C_SDA = 0;
		}	
		I2C_SCL = 1;
		delay_us(I2C_FREQUENCY);
		I2C_SCL = 0;
		delay_us(I2C_FREQUENCY);
	}
	
    I2C_HostReceiveAck();	
}


/**
	@breif 读字节数据
*/
uint8_t I2C_HostReadByte(void)
{	
	uint16_t i=0,dat = 0;
	
	I2C_SCL = 0;
	delay_us(I2C_FREQUENCY);
	for(i=0;i<8;i++)
	{
		dat <<= 1; 
		I2C_SCL = 1;
		delay_us(I2C_FREQUENCY);
		I2C_SDA = 1;
		if(I2C_SDA == 1)
			dat |= 0x01;
		else 
			dat |= 0x00;
		
		I2C_SCL = 0;
		delay_us(I2C_FREQUENCY);		
	}	
	
	return((uint8_t)dat);	
	
}



/**
	@breif 完整写一个字节数据
*/
uint8_t I2C_HostWrite(uint8_t u8I2C_Address,uint8_t u8regAddr,uint8_t u8data)
{
	
	I2C_Start();
	
	I2C_HostWriteByte(u8I2C_Address);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	}
	
	I2C_HostWriteByte(u8regAddr);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	}
	

	I2C_HostWriteByte(u8data);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	
	}
		
	I2C_Stop();
	
	return 1;

}

/**
	@breif 连续写数据
*/
uint8_t I2C_Cont_Write(uint8_t u8I2C_Address,uint8_t u8I2C_first_Addr,uint8_t u8data_len, uint8_t *data)
{
	
	I2C_Start();

	I2C_HostWriteByte(u8I2C_Address);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();return 0;}
	
	I2C_HostWriteByte(u8I2C_first_Addr);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();return 0;}
	
	for(uint8_t i = 0; i < u8data_len; i++)
	{
		I2C_HostWriteByte(data[i]);
		if(tI2C_bit.bI2C_ack == 0)
		{
			I2C_Stop();return 0;}			
	}
	
	I2C_Stop();	
	
	return  1;
}

/**
	@breif 完整读一个字节数据
*/
uint8_t I2C_HostRead(uint8_t u8I2C_Address ,uint8_t u8regAddr)
{
	uint8_t temp = 0;

	I2C_Start();
	
	I2C_HostWriteByte(u8I2C_Address);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	}
	
	I2C_HostWriteByte(u8regAddr);
	if(tI2C_bit.bI2C_ack == 0) 
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	}
	
	I2C_Start();
	
	I2C_HostWriteByte(u8I2C_Address + 1);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
		return 0;
	}

	temp = I2C_HostReadByte();

	I2C_HostSendAck(1);

	I2C_Stop();
	
	return temp;

}


void I2C_Cont_Read(uint8_t u8I2C_Address, uint8_t u8regAddr, uint8_t u8length, uint8_t *pu8buff)
{
	
	I2C_Start();	

	I2C_HostWriteByte(u8I2C_Address);
	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
	}
	
	I2C_HostWriteByte(u8regAddr);
	if(tI2C_bit.bI2C_ack == 0) 
	{
		I2C_Stop();
		printf("I2C is timeout");
	}
	
	I2C_Start();	
	
	I2C_HostWriteByte(u8I2C_Address + 1);	

	if(tI2C_bit.bI2C_ack == 0)
	{
		I2C_Stop();
		printf("I2C is timeout");
	}	
	
	for(uint8_t  i = 0; i < u8length -1; i++)
	{
		*pu8buff = I2C_HostReadByte();
		I2C_HostSendAck(0);
		pu8buff++;
	}
	
	*pu8buff = I2C_HostReadByte();	
	I2C_HostSendAck(1);
	
	I2C_Stop();
			
}


