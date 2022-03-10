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

#endif
