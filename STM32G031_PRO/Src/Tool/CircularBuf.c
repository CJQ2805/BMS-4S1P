#include "CircularBuf.h"
#include "bsp.h"
void ResetPosition(PT_CircularBuf ptCircBuf)
{

	if(!ptCircBuf)
		return;

	ptCircBuf->u16WritePos = 0;
	ptCircBuf->u16ReadPos  = 0;

}

int isFull(PT_CircularBuf ptCircBuf)
{
	if(!ptCircBuf)
		return 0;
	return (((ptCircBuf->u16WritePos + 1) % ptCircBuf->u16RxBufSize) == ptCircBuf->u16ReadPos);
}


int isEmpty(PT_CircularBuf ptCircBuf)
{
	if(!ptCircBuf)
		return 0;

	return (ptCircBuf->u16WritePos == ptCircBuf->u16ReadPos);
}



void PutData(PT_CircularBuf ptCircBuf , uint8_t u8Val)
{
	if((!ptCircBuf))
	{	
		printf("PutData");
		return ;
	}

	if( ptCircBuf->u16WritePos  >= ptCircBuf->u16RxBufSize )
	{
		//printf("erro:u16WritePos");
		return ;
	}
	ptCircBuf->au8RxBuf[ptCircBuf->u16WritePos++] = u8Val;
	
}


 void PutData2CircBuf(PT_CircularBuf ptCircBuf , uint8_t u8Val)
{
	if((!ptCircBuf))
	{
		return ;
	}
	
	if(isFull(ptCircBuf))
	{
		printf("CircBuf %s is full" , ptCircBuf->acName);
		return;
	}
	else
	{ 
		ptCircBuf->au8RxBuf[ptCircBuf->u16WritePos] = u8Val;
		ptCircBuf->u16WritePos = (ptCircBuf->u16WritePos + 1) % ptCircBuf->u16RxBufSize;	
	}	
}





int PutData2CircBuf2(PT_CircularBuf ptCircBuf , uint8_t u8Val)
{
	 int ierro = 0;
	 if( (!ptCircBuf) )
	 {
		 printf("pointer is null");
		 return -1;
	 }
	 
	 if (isFull(ptCircBuf))
	 {
		 printf("CircBuf %s is full" , ptCircBuf->acName);
		 return 0;
	 }
	 else
	 { 
		 ptCircBuf->au8RxBuf[ptCircBuf->u16WritePos] = u8Val;
		 ptCircBuf->u16WritePos = (ptCircBuf->u16WritePos + 1) % ptCircBuf->u16RxBufSize;	
		 ierro = 1;
	 }	
	 return ierro;
}

int CircBufIsEmpty(PT_CircularBuf ptCircBuf)
{
	return  isEmpty(ptCircBuf);

}


int GetDataFrmCircBuf(PT_CircularBuf ptCircBuf , uint8_t *pu8Val)
{

	 if( (!ptCircBuf) )
		 return -1;

	 if (isEmpty(ptCircBuf))
		 return 0;
	 else
	 {
		 *pu8Val = ptCircBuf->au8RxBuf[ptCircBuf->u16ReadPos];
		 ptCircBuf->u16ReadPos = (ptCircBuf->u16ReadPos + 1) % ptCircBuf->u16RxBufSize;
		 return 1;
	 }
}



/*
	return data len
*/
uint16_t ReadDataFromCircBuf(PT_CircularBuf ptCircBuf , uint8_t *pu8Val, uint16_t u16len)
{
	uint16_t  i = 0;
	for(i = 0; i < u16len; i++ )
	{
		if(GetDataFrmCircBuf(ptCircBuf, &pu8Val[i]) != 1)
		 	break;
	}

	return i;

}



uint16_t GetDataLenOfCircBuf(PT_CircularBuf ptCircBuf)
{

	uint16_t u16DataLen = 0;
	uint16_t u16WritePosTmp = ptCircBuf->u16WritePos;
	
	if(u16WritePosTmp == ptCircBuf->u16ReadPos)
	{
		return 0;
	}
	else if(ptCircBuf->u16ReadPos > u16WritePosTmp )
	{

		u16DataLen = ptCircBuf->u16RxBufSize - ptCircBuf->u16ReadPos + u16WritePosTmp;
	}
	else 
	{
		u16DataLen = u16WritePosTmp - ptCircBuf->u16ReadPos;

	}


	return u16DataLen;

}



 uint16_t ReadDataFromCircBuf2(PT_CircularBuf ptCircBuf , uint8_t *pu8Val, uint16_t u16len)
{

	return ReadDataFromCircBuf( ptCircBuf, pu8Val, u16len );

}





int read_data_from_circ_buf( uint8_t * pu8rx_buf, uint16_t u16rx_buf_len, uint16_t u16read_len, PT_CircularBuf ptCircBuf )
{
	uint8_t	u8read_delay_conter = 0;
	uint16_t u16real_len = 0, u16read_total = 0;
	
	do{
		u16real_len = ReadDataFromCircBuf2(ptCircBuf , &pu8rx_buf[u16read_total], u16read_len - u16read_total);
		u16read_total += u16real_len;
		if(!u16real_len){
			
			u8read_delay_conter++;	
		
		}else if(u16read_total == u16read_len){
			break;
		}
		else{
			
			u8read_delay_conter = 0;
		}
		
		if( 5 <= u8read_delay_conter)
		{
			break;
		}

	}while(u16read_total != u16read_len);

	return u16read_total;

}




int put_data_to_circ_buf( uint8_t * pu8tx_data, uint16_t u16tx_data_len, PT_CircularBuf ptCircBuf )
{
	uint8_t	u8tx_delay_conter = 0;
	uint16_t u16tx_index = 0;
	do{
		if(PutData2CircBuf2(ptCircBuf, pu8tx_data[u16tx_index]) != 1)
		{
			u8tx_delay_conter++;	
		}
		else
		{
			u8tx_delay_conter = 0;
			u16tx_index++;
		}

		if(5 <= u8tx_delay_conter)
		{
			break;
		}


	}while(u16tx_index != u16tx_data_len);
	
	return u16tx_index;

}



void clear_circl_buf(PT_CircularBuf ptCircBuf)
{
	memset(&(ptCircBuf->au8RxBuf) , 0 , ptCircBuf->u16WritePos);
	ptCircBuf->u16WritePos = 0;
	ptCircBuf->u16ReadPos  = 0;
}






