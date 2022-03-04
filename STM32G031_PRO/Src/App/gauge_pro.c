#include "gauge_pro.h"

/**
@brief   Gauge Model TI BQ40Z50
@Author  CJQ2805 
*/

struct  _gauge_info_t  gt_gauge_info;
static en_result_t GAUGE_DataUpdate(void);

void Gauge_Init(void)
{
	memset(&gt_gauge_info, 0x00, sizeof(gt_gauge_info));
}


void GAUGE_CommTask(void)
{
	static u32_tim_dly dlyGaugeData;
	/*update gague data 250ms*/
	if(Ok != TMR0_Delay(TRUE, &dlyGaugeData, TMR0_DELAY_MS(250)))
		return;
	

	if( Ok!= GAUGE_DataUpdate())
	{
		
	}
}


static en_result_t GAUGE_DataUpdate(void)
{
	int ivalue = 0;
	uint8_t u8err_cnt = 0;
	
	if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_VOLTAGE_ADDR)) == -1 )		
		++u8err_cnt;
	else
		gt_gauge_info.u16voltage = ivalue;
	
	if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_BATTERY_STAT_ADDR)) == -1 )
		 ++u8err_cnt;
	else
		gt_gauge_info.u16battery_stat = ivalue;		

}

