#ifndef __USART_H
#define __USART_H
#include "bsp.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

HAL_StatusTypeDef usart1_tx(u8 *pu8data, u16 u16size);
HAL_StatusTypeDef usart2_tx(u8 *pu8data, u16 u16size);
void UART_DeInit(void);
void init_rx_usart(void);

extern volatile u8 gu8rx_data_flag;
extern volatile u16 gu16rx_index;

#endif

