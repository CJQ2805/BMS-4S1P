#include "iap.h"
#include "combin_pack.h"


volatile unsigned int g_u32Seq;

struct _iap_arg_t{

	uint8_t  u8app_start_flag;
	uint8_t  u8boot_start_flag;

};

struct _iap_arg_t  gt_iap_atg;
__align(4) uint8_t  g_au8flashreadbuf[RX_DATA_MAX_LEN];



static u8 cmd_start_update(void)
{
	u32 u32addr = 0;
	u32 u32len = 0;  		//数据长度
	u32 u32SEQ1 = 0;		//包数（第几包）
	u32 u32CRC_Upper = 0;	//上位机的CRC值
	u32 u32CRC_Vaule = 0;	//下位机计算的CRC
	
	u32len = g_au8Rxbuffer[3];

	if(gu16rx_index == (u32len + 9))
	{
		u32CRC_Upper = 0;
		if(g_au8Rxbuffer[u32len + 8] == 0xBB)
		{
			u32SEQ1 = g_au8Rxbuffer[u32len + 4];
			u32SEQ1 <<= 8;
			u32SEQ1 |= g_au8Rxbuffer[u32len+ 5];
			
			u32CRC_Upper = g_au8Rxbuffer[u32len + 6];
			u32CRC_Upper <<= 8;
			u32CRC_Upper |= g_au8Rxbuffer[u32len+ 7];
			u32CRC_Vaule = CRC_16((u8 *)g_au8Rxbuffer, u32len + 6);
			
			if(u32CRC_Vaule == u32CRC_Upper)
			{
				if(g_u32Seq == u32SEQ1)
				{
					volatile uint8_t *pSrc = g_au8Rxbuffer + 6;
		
					u32addr = g_au8Rxbuffer[4];
					u32addr <<= 8;
					u32addr |= g_au8Rxbuffer[5];
					u32addr += STM32_ADDR_FLASH_SECTOR_0;
					
					if(iap_wire_app_hex(u32addr, u32len - 2, (u8 *)pSrc)!= HAL_OK)
					{
						send_respond(CMD_Write_Error);
						gt_iap_atg.u8boot_start_flag = 1;
						return 1;								
					}
					
	//								STMFLASH_Read(u32Addr, len-2, (u32*)g_au8flashreadbuf);
					
	//								if(BufferCmp((u8 *)(g_au8Rxbuffer+6), g_au8flashreadbuf, len-2))
	//								{
	//									send_respond(Write_Error);
	//									gt_iap_atg.u8boot_start_flag = 1;
	//									return;
	//								}
					send_respond(CMD_CRC_Ok);
					g_u32Seq++;
					gu16rx_index = 0;
				}
				else if(g_u32Seq == (u32SEQ1 + 1))
				{
					g_u32Seq--;
					send_respond(CMD_CRC_Ok);	
					g_u32Seq++;
				}
				else
				{
					send_respond(CMD_CRC_Ok);									
				}	
			}
			else
			{
				send_respond(CMD_CRC_Error);
				gu16rx_index = 0;
				return 2;						
			
			}
		}
		else
		{
			send_respond(CMD_CRC_Error);
			gu16rx_index = 0;
			return 2;
		}
	}
	else
	{
		send_respond(CMD_CRC_Error);
		gu16rx_index = 0;
		return 2;					
	}

	return 0;
}

static u8 cmd_file_end(void)
{
	u32 u32addr = 0;
	u32 u32CRC_Vaule = 0;	//下位机计算的CRC	
	
	if(g_au8Rxbuffer[6] == 0xBB)
	{
		u32addr = g_au8Rxbuffer[4];
		u32addr <<= 8;
		u32addr |= g_au8Rxbuffer[5];
		u32CRC_Vaule = CRC_16(g_au8Rxbuffer, 4);
		
		if(u32CRC_Vaule == u32addr)
		{
			g_u32Seq = 0;
			send_respond(CMD_Load_OK);
			gt_iap_atg.u8app_start_flag = 1;
			return 1;
		}
	}

}

static u8 cmd_erase(void)
{
	u32 u32CRC_Upper = 0;	//上位机的CRC值
	u32 u32CRC_Vaule = 0;	//下位机计算的CRC
	
	if(g_au8Rxbuffer[6] == 0xBB)
	{
		u32CRC_Upper = 0;
		u32CRC_Upper = g_au8Rxbuffer[4];
		u32CRC_Upper <<= 8;
		u32CRC_Upper |= g_au8Rxbuffer[5];
		u32CRC_Vaule = CRC_16(g_au8Rxbuffer, 4);
		
		if(u32CRC_Upper == u32CRC_Vaule)
		{
			g_u32Seq = 0;	
			gu16rx_index = 0;						
			send_respond(CMD_EraseOk);
		}
		else
			send_respond(CMD_Error_Frame);
	}
	else
		send_respond(CMD_Error_Frame);		

	return 0;
}


static u8 cmd_exit_update(void)
{
	u32 u32CRC_Upper = 0;	//上位机的CRC值
	u32 u32CRC_Vaule = 0;	//下位机计算的CRC	
	if(g_au8Rxbuffer[6] == 0xBB)
	{
		u32CRC_Upper = g_au8Rxbuffer[4];
		u32CRC_Upper <<= 8;
		u32CRC_Upper |= g_au8Rxbuffer[5];
		u32CRC_Vaule = CRC_16(g_au8Rxbuffer, 4);
		
		if(u32CRC_Vaule == u32CRC_Upper)
		{
			send_respond(CMD_ExitOk);
			gt_iap_atg.u8boot_start_flag = 1;						
		}
		else
			send_respond(CMD_Error_Frame);
	}
	else
		send_respond(CMD_Error_Frame);				
					
}

void iap_update(void)
{
	volatile u32 Wait_Cnt = 1000;
	static u8 x;
	
	gt_iap_atg.u8app_start_flag  = 0;
	gt_iap_atg.u8boot_start_flag = 0;
		
	u8 u8stat = 0;
	
	while(Wait_Cnt)
	{
		delay_ms(6);
		
		if(gu8rx_data_flag)
		{
			gu8rx_data_flag = 0;
			gu16rx_index = 0;
			if(CMD_Rec_Data_Frame == g_au8Rxbuffer[0])
			{
				g_u32Seq = 0;
				send_respond(CMD_Update_Ready);
				break;
			}
			
		}
		Wait_Cnt--;
	}
	
	if(Wait_Cnt == 0)
	{
		send_respond(CMD_Over_Time);
		gt_iap_atg.u8boot_start_flag = 1;
		return;
	}	
	delay_ms(30);
	
	while(1)
	{
		x = !x;
		PC6Out(x);	
		delay_ms(60);
		
		/*喂狗*/
		IWDG_Feed();
		
		if(g_au8Rxbuffer[0] == 0xFE && g_au8Rxbuffer[1] == 0xFE)
		{
			
			switch(g_au8Rxbuffer[2])
			{
				case CMD_Start_Update:
					
					u8stat = cmd_start_update();
		
				break;
				
				case CMD_File_End:
					
					u8stat = cmd_file_end();
				
				break;
				
				
				case CMD_Erase:
					
					cmd_erase();
				
				break;
				
				case CMD_Exit_Update:
					
					cmd_exit_update();
				
				break;
			
				default:
					
					gu16rx_index = 0;					
					send_respond(CMD_CRC_Error);
					u8stat = 1;	
				
				break;
			}
			
			if(1 == u8stat)
			{
				return;
			}
			else if(2 == u8stat)
			{
				continue;
			}

		}
		else
		{
			gu16rx_index = 0;
		}
	
	}
}


uint8_t BufferCmp(uint8_t *buf1,uint8_t *buf2,uint8_t size)
{
	uint8_t i=0;
	for(i=0;i<size;i++)
	{
		if(buf1[i] != buf2[i])
		{
			return 1;
		}
	}
	return 0;
}



__align(8) uint64_t au64download_buff[16];
u8 iap_wire_app_hex(uint32_t u32addr, uint32_t u32len, uint8_t *pu8buf)
{
	uint32_t data_num = 0, j = 0;
	uint32_t i = 0, u32app_addr = u32addr; 
	uint8_t  *pu8combin = pu8buf;
	uint64_t u64temp = 0;

	
	data_num = u32len % 8;
	
	if(data_num != 0)
	{
		return 1;
	}
	
	data_num = u32len/8;
	
	for(j = 0; j< data_num; j++)
	{
		
		u64temp |= (uint64_t)pu8combin[7+(i*8)]<<56;		
		u64temp |= (uint64_t)pu8combin[6+(i*8)]<<48;		
		u64temp |= (uint64_t)pu8combin[5+(i*8)]<<40;
		u64temp |= (uint64_t)pu8combin[4+(i*8)]<<32;
		u64temp |= (uint64_t)pu8combin[3+(i*8)]<<24;
		u64temp |= (uint64_t)pu8combin[2+(i*8)]<<16;
		u64temp |= (uint64_t)pu8combin[1+(i*8)]<<8;	
		u64temp |= (uint64_t)pu8combin[0+(i*8)];
		
		au64download_buff[i++] = u64temp;
		u64temp = 0;
	}
	if(16 == i )
	{
		i = 0;
		if(STMFLASH_Write(u32app_addr, au64download_buff, 16) != HAL_OK)
		return 1;	
		u32app_addr += 64*16; 
	}
	
	if(i)
		if(STMFLASH_Write(u32app_addr, au64download_buff, i) != HAL_OK)	
		return 1;

	return 0;
}





void Run_App_Check(void)
{

	if(gt_iap_atg.u8app_start_flag == 1)
	{
		jump_application(STM32_APP_BASE);
	}
	
	if(gt_iap_atg.u8boot_start_flag == 1)
	{
		jump_application(STM32_BOOTLOADER_BASE);
	}

}
