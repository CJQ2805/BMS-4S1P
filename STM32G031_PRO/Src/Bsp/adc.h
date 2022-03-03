#ifndef __ADC_H
#define __ADC_H

#include "bsp.h"

typedef uint32_t  u32;

void MX_ADC1_Init(void);

float Get_adc_result(u32 ch);

void ADC_DeInit(void);

#endif

