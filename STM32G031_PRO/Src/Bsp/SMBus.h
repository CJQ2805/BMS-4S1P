#ifndef __SMBus_H
#define __SMBus_H
#include "bsp.h"

#define SDA_IN() 	{GPIOA->MODER&=~(3<<(5*2));GPIOA->MODER|=0<<5*2;} 	//pin7	有点问题，百度结果为不支持位带操作无法使用
#define SDA_OUT()	{GPIOA->MODER&=~(3<<(5*2));GPIOA->MODER|=1<<5*2;}


#define 	SMB_SCL     PAout(4)
#define 	SMB_SDA     PAout(5)
#define 	READ_SDA    PAin(5)


void SMBus_Init(void);
int Read_bq40z50_data(uint8_t SMB_address, uint8_t commond);
int Read_BQ40Z50_ManuFBAccess_data(uint8_t SMB_address, uint16_t commond);

#endif

