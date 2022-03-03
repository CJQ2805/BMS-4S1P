#include "cir_handle.h"
#include "CircularBuf.h"
#include "bsp.h"


T_CircularBuf  gt_rx_circ_buf;

u8 au8circ_rx_buff[USART1_RX_CIRC_BUF_LEN] = {0};

static int circ_usart1_rx_data_handle(u8 *pu8buff);

void usart1_rx_irq(u8 u8val)
{
	PutData2CircBuf(&gt_rx_circ_buf, u8val);
}


void usart1_rx_handle(void)
{

	u8 au8rx_buf_data[1] = {0};	
	circ_usart1_rx_data_handle(au8rx_buf_data);
	
}


static int circ_usart1_rx_data_handle(u8 *pu8buff)
{
	int irx_len = -1;
	u8 au8rx_buf_data[USART1_RX_CIRC_BUF_LEN] = {0};	
	static u8 x;
	if(isEmpty(&gt_rx_circ_buf))
	{
		printf("circ buf is empty");
		return -1;
	}
	
	
	irx_len = read_data_from_circ_buf(au8rx_buf_data, 256, 1, &gt_rx_circ_buf);
	if(irx_len != 1)
	return -1;
	
	if(0xFE != au8rx_buf_data[0])
	return -1;
	
	irx_len = read_data_from_circ_buf(au8rx_buf_data, 256, 1, &gt_rx_circ_buf);
	if(irx_len != 1)
	return -1;

	if(0xFE != au8rx_buf_data[0])
	return -1;	
	delay_ms(10);
	irx_len = read_data_from_circ_buf(au8rx_buf_data, 256, 5, &gt_rx_circ_buf);
	if(irx_len != 5)
	return -1;
	
	if(au8rx_buf_data[4] == 0xBB)
	{
		switch(au8rx_buf_data[0])
		{
			case 0x35:
				//÷ÿ∆Ù÷∏¡Ó
				jump_application(STM32_BOOTLOADER_BASE);
			
			break;
			
			case 0x37:	
				
				x = !x;
				PC6Out(x);
			
			break;
		}
	
	}	
}

void cir_handle_init(void)
{
	memset(&gt_rx_circ_buf, 0x00, sizeof(gt_rx_circ_buf));
	gt_rx_circ_buf.u16RxBufSize = USART1_RX_CIRC_BUF_LEN;
	gt_rx_circ_buf.u8UseMode    = CIRCULAR_BUF_MODE;
}
