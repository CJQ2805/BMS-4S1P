#include "combin_pack.h"
#include "iap.h"

unsigned char res_arr[8] = {0};

void send_respond(unsigned char respond)
{
	unsigned int crc;
	res_arr[0] = 0xFE; //1 2�ֽڰ�ͷ
	res_arr[1] = 0xFE;
	res_arr[2] = respond;
	
	res_arr[3] = ((g_u32Seq)>>8);
	res_arr[4] = (unsigned char)g_u32Seq;
	
	crc  = CRC_16(res_arr, 5);
	res_arr[5] = (unsigned char)(crc >>8);
	res_arr[6] = (unsigned char)crc;
	
	res_arr[7] = 0xBB; //��β
	
	usart1_tx(res_arr,8);
}

uint32_t  CRC_Check(uint8_t *Buf)
{
	  uint32_t  u32_tmp;
	  uint32_t  u32_tmp1;	
	
	  u32_tmp=Get_CrcValue(Buf,4,5);
		u32_tmp1 = CRC_16(Buf,4);  

    if(u32_tmp1==u32_tmp){
		   return 1;
		}
		else{
			return 0;
		}
	  
}

uint32_t  Get_CrcValue(uint8_t *Buf,uint8_t bit1,uint8_t bit2)
{
		uint32_t  u32_tmp;
		u32_tmp = *(Buf+bit1);
		u32_tmp <<= 8;
		u32_tmp |= *(Buf+bit2);
	
	 return u32_tmp;
}


unsigned int CRC_16(u8 ptr[],unsigned char len)      //ptrΪ����ָ�룬lenΪ���ݳ��ȣ������ֽ�����
{
	unsigned char i,j=0;
	unsigned int Crc_Value=0;             //ÿ��ʹ��ǰ��Ҫ���㣬�������ӡ�������ͬһ��ֵҪ���ж��ѭ�������һ��Ҫ�ڿ�ʼǰ���㡣
	while(len--)
	{
		for(i=0x80;i!=0;i>>=1)              //forѭ����ʵ�ֵĹ���Ϊ��������һ��Ϊ1������λʱ��������������Ϊ0��������������������ʽ����������㣬
		{                                   //���õ���ֵ����CRC�У���������֡��һ��������������ֵ�����������������λҲΪһ����CRC����һλ����0x1021���������
			                                  //���������������λΪ0ʱ����CRC��Ӧ��λΪ0��ֻ��CRC����һλ����Ϊ1��Ҫ��0x1021�����
			if((Crc_Value&0x8000)!=0)         //���CRC�����ĳλ���Ӧ������λͬʱΪ1����Ҫ��CRC��������������㣬�൱��û�иı�CRC��ֵ��ֻ�ǽ�������һλ��
			{                                 //��Ϊ������Ƕ�Ϊ1ʱ������ζ����֡����λ���Ӧ���������Ӧ����λ��Ϊ1���������Ľ����0����ôʵ����һ���ǲ��ø�λ�����������ġ�
				Crc_Value<<=1;                  //����˼�����һλһλ�����
				Crc_Value^=0x1021;
			}

			else Crc_Value<<=1;

			if((ptr[j]&i)!=0) Crc_Value^=0x1021;
		 }
	 j++;
	}
  return((uint16_t)Crc_Value);
}

