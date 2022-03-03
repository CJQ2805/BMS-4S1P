#ifndef __CIR_HANDLE_H
#define __CIR_HANDLE_H
#include "bsp.h"
#define USART1_RX_CIRC_BUF_LEN  256
#define RX_BUF_DATA_LEN          1

typedef uint8_t  u8;

void usart1_rx_irq(u8 u8val);
void usart1_rx_handle(void);
void cir_handle_init(void);

#endif
