#include "app.h"

void APP_Init(void)
{
	BSP_Init();
	APP_ParamInit();
	APP_ParamLoad();
}


void APP_PollingTask(void)
{	
    static u32_tim_dly_t apptim_dly, reset_dly;	
	static u8 x;
	
	GAUGE_CommTask();	
	usart1_rx_handle();

//	 if(Ok == TMR0_Delay(TRUE, &apptim_dly, TMR0_DELAY_MS(500)))
//	 {	
//		x = !x;
//		PC6Out(x);		 
//	 }
//	 
//	 if(Ok == TMR0_Delay(TRUE, &reset_dly, TMR0_DELAY_MS(10000)))
//	 {	
//		//jump_application(STM32_BOOTLOADER_BASE);
//	 }
	 
	 
	IWDG_Feed();
}


void APP_ParamInit(void)
{
	Gauge_Init();	
	cir_handle_init();
}


void APP_ParamLoad(void)
{


}
