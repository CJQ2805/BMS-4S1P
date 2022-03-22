#include "app.h"

void APP_Init(void)
{
	BSP_Init();
	APP_ParamInit();
	APP_ParamLoad();
}


void APP_PollingTask(void)
{	
    static u32_tim_dly_t dlyApp, dlyReset;	
	static u8 x;
	
	
	/*tasks polling*/
	GAUGE_CommTask();
	DCDC_CommTask();
	BMS_ProTask();
	Usart1_RxHandle();

//	 if(Ok == TMR0_Delay(TRUE, &dlyApp, TMR0_DELAY_MS(500)))
//	 {	
//		x = !x;
//		PC6Out(x);		 
//	 }
//	 
//	 if(Ok == TMR0_Delay(TRUE, &dlyReset, TMR0_DELAY_MS(10000)))
//	 {	
//		//jump_application(STM32_BOOTLOADER_BASE);
//	 }
	 
	IWDG_Feed();
}


void APP_ParamInit(void)
{
	Gauge_Init();
	DCDC_Init();
	Cir_HandleInit();
}


void APP_ParamLoad(void)
{


}
