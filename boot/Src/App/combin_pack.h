#ifndef __COMBIN_PACK_H
#define __COMBIN_PACK_H
#include "bsp.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void send_respond(unsigned char respond);
uint32_t  Get_CrcValue(uint8_t *Buf,uint8_t bit1,uint8_t bit2);
unsigned int CRC_16(u8 ptr[],unsigned char len) ;  
#endif

