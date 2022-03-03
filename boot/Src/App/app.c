#include "app.h"


void APP_Init(void)
{
	BSP_Init();
}


void APP_PollingTask(void)
{
    static u32_tim_dly_t apptim_dly , jumpapp_dly;	
	static u8 x;
	
	/*Î¹¹·*/
	IWDG_Feed();	
	
	delay_ms(1);

	if(gu8rx_data_flag)
	{
		 gu8rx_data_flag = 0;
		 gu16rx_index = 0;
		
		if(CMD_Compel_Update == g_au8Rxbuffer[0])
		{
			send_respond(CMD_Compel_Ready);				
			iap_update();
		}
	}
	
	if(Ok == TMR0_Delay(TRUE, &apptim_dly, TMR0_DELAY_MS(1000)))
	{	
		x = !x;
		PC6Out(x);		 
	}
	
	Run_App_Check();

}

