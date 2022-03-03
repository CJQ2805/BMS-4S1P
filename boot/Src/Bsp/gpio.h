#ifndef __GPIO_H
#define __GPIO_H
#include "bsp.h"

void MX_GPIO_Init(void);


#define PC6Out(x)	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,x)


#endif

