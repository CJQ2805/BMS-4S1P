#ifndef __SMBus_H
#define __SMBus_H
#include "bsp.h"

#define SDA_IN() 	{GPIOA->MODER&=0XFFFF3FFF;} 	//pin7
#define SDA_OUT()	{GPIOA->MODER&=0XFFFF7FFF;}


#define 	SMB_SCL     PAout(6)
#define 	SMB_SDA     PAout(7)
#define 	READ_SDA    PAin(7)

#endif

