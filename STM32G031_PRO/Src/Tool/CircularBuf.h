#ifndef	 __CIRCULARBUF_H
#define  __CIRCULARBUF_H
#include "bsp.h"

typedef    struct _CircularBuf {

#define COMMON_BUF_MODE     1   // 普通buf 模式
#define CIRCULAR_BUF_MODE   2 //用作环形模式

	char acName[20];
	volatile uint16_t  u16WritePos; 
	volatile uint16_t  u16ReadPos; 
	volatile uint8_t   au8RxBuf[128];
	volatile uint16_t  u16RxBufSize;
	volatile uint8_t   u8UseMode;
	
}T_CircularBuf,*PT_CircularBuf;


void ResetPosition(PT_CircularBuf ptCircBuf);
int isFull(PT_CircularBuf ptCircBuf);
int isEmpty(PT_CircularBuf ptCircBuf);
void PutData(PT_CircularBuf ptCircBuf , uint8_t u8Val);
void PutData2CircBuf(PT_CircularBuf ptCircBuf , uint8_t u8Val);
int PutData2CircBuf2(PT_CircularBuf ptCircBuf , uint8_t u8Val);
int CircBufIsEmpty(PT_CircularBuf ptCircBuf);
int GetDataFrmCircBuf(PT_CircularBuf ptCircBuf , uint8_t *pu8Val);
uint16_t ReadDataFromCircBuf(PT_CircularBuf ptCircBuf , uint8_t *pu8Val, uint16_t u16len);
uint16_t GetDataLenOfCircBuf(PT_CircularBuf ptCircBuf);
uint16_t ReadDataFromCircBuf2(PT_CircularBuf ptCircBuf , uint8_t *pu8Val, uint16_t u16len);
int read_data_from_circ_buf( uint8_t * pu8rx_buf, uint16_t u16rx_buf_len, uint16_t u16read_len, PT_CircularBuf ptCircBuf );
int put_data_to_circ_buf( uint8_t * pu8tx_data, uint16_t u16tx_data_len, PT_CircularBuf ptCircBuf );
void clear_circl_buf(PT_CircularBuf ptCircBuf);

#endif 
