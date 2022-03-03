#ifndef __SMBus_H
#define __SMBus_H
#include "bsp.h"

#define SDA_IN() 	{GPIOA->MODER&=0XFFFF3FFF;} 
#define SDA_OUT()	{GPIOA->MODER&=0XFFFF7FFF;}


#define 	SMB_SCL     PAout(6)
#define 	SMB_SDA     PAout(7)


#endif

