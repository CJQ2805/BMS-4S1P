#ifndef __GPIO_H
#define __GPIO_H
#include "bsp.h"

void MX_GPIO_Init(void);

typedef enum
{
  LED3 = 0,
  LED_GREEN = LED3
} Led_TypeDef;


#define LEDn                               1

#define LED3_PIN                           GPIO_PIN_6
#define LED3_GPIO_PORT                     GPIOC
#define LED3_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOC_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do {LED3_GPIO_CLK_ENABLE(); } while(0U)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   LED3_GPIO_CLK_DISABLE())

#define PC6Out(x)	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,x)


#endif

