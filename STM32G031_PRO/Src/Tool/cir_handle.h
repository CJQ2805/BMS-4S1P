#ifndef __CIR_HANDLE_H
#define __CIR_HANDLE_H
#include "bsp.h"
#define USART1_RX_CIRC_BUF_LEN  256
#define RX_BUF_DATA_LEN          1

typedef uint8_t  u8;

void Usart1_RxIrq(u8 u8val);
void Usart1_RxHandle(void);
void Cir_HandleInit(void);

#endif
