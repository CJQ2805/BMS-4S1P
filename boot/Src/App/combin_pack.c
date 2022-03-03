#include "combin_pack.h"
#include "iap.h"

unsigned char res_arr[8] = {0};

void send_respond(unsigned char respond)
{
	unsigned int crc;
	res_arr[0] = 0xFE; //1 2字节包头
	res_arr[1] = 0xFE;
	res_arr[2] = respond;
	
	res_arr[3] = ((g_u32Seq)>>8);
	res_arr[4] = (unsigned char)g_u32Seq;
	
	crc  = CRC_16(res_arr, 5);
	res_arr[5] = (unsigned char)(crc >>8);
	res_arr[6] = (unsigned char)crc;
	
	res_arr[7] = 0xBB; //包尾
	
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


unsigned int CRC_16(u8 ptr[],unsigned char len)      //ptr为数据指针，len为数据长度（传输字节数）
{
	unsigned char i,j=0;
	unsigned int Crc_Value=0;             //每次使用前都要清零，否则会叠加。像这种同一个值要进行多次循环计算的一定要在开始前清零。
	while(len--)
	{
		for(i=0x80;i!=0;i>>=1)              //for循环中实现的功能为：遇到第一个为1的数据位时，假设后面的数据为0，后面的数据与生成码简式进行异或运算，
		{                                   //将得到的值放入CRC中，当成数据帧第一次与生成码的异或值，如果接下来的数据位也为一，则将CRC左移一位再与0x1021做异或运算
			                                  //如果接下来的数据位为0时，若CRC对应的位为0则只将CRC左移一位，若为1则要与0x1021做异或。
			if((Crc_Value&0x8000)!=0)         //如果CRC里面的某位与对应的数据位同时为1，则要将CRC进行两次异或运算，相当于没有改变CRC的值，只是将它左移一位，
			{                                 //因为如果他们都为1时，则意味数据帧数据位与对应的生成码对应数据位都为1，他们异或的结果是0，那么实际下一次是不用该位再做异或运算的。
				Crc_Value<<=1;                  //基本思想就是一位一位的异或。
				Crc_Value^=0x1021;
			}

			else Crc_Value<<=1;

			if((ptr[j]&i)!=0) Crc_Value^=0x1021;
		 }
	 j++;
	}
  return((uint16_t)Crc_Value);
}

