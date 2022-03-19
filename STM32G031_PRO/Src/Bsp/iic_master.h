#ifndef __IIC_MASTER_H
#define __IIC_MASTER_H

#include "bsp.h"

#define     READ_SDA    PAin(12)
#define 	I2C_SCL     PAout(11)
#define 	I2C_SDA     PAout(12)

#define I2C_FREQUENCY				5  	//5us



typedef struct I2C_config_bit_t{

	uint8_t bI2C_ack    : 		1;
	
}t_I2C_bit;

void I2C_Init(void);
uint8_t I2C_HostRead(uint8_t u8I2C_Address ,uint8_t u8regAddr);
uint8_t I2C_Cont_Write(uint8_t u8I2C_Address,uint8_t u8I2C_first_Addr,uint8_t u8data_len, uint8_t *data);
void I2C_Cont_Read(uint8_t u8I2C_Address, uint8_t u8regAddr, uint8_t u8length, uint8_t *pu8buff);
#endif
