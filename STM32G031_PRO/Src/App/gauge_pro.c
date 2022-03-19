#include "gauge_pro.h"

/**
	@brief 
	@Author  CJQ2805 
	Gauge Model TI BQ40Z50

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
	
	static u32_tim_dly_t dlyGaugeData, dlySOCData;
	static uint8_t u8index = 0;

	int ivalue = 0 ;
	uint8_t u8err_cnt = 0;
	/* update gauge data 500ms */	
	if(Ok != TMR0_Delay(TRUE ,&dlyGaugeData ,TMR0_DELAY_MS(200)))
		return u8err_cnt;		
	//read and save the valid values
	
	switch(u8index)
	{
	
		case 0:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_VOLTAGE_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16voltage = ivalue;	

		break;

		case 1:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_BATTERY_STAT_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16battery_stat = ivalue;	
		
		break;
		
		case 2:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_EMPTY_TIME_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16empty_time = ivalue;				
		
		break;
		
		case 3:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_FULL_TIME_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16full_time = ivalue;
			
		break;

		case 4:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_CURRENT_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.icurrent = ivalue;
			
		break;

		
		case 5:
			u8index ++;			
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_TEMPERATURE_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16temperature = ivalue;	
		
		break;

		case 6:
			u8index ++;		
			if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_SOH_ADDR)) == -1 )
				 ++u8err_cnt;
			else
				gt_gauge_info.u16soh = ivalue;	
			
		break;
			
		case 7:
			u8index ++;
			if((ivalue = Read_BQ40Z50_ManuFBAccess_data(GAUGE_SMB_ADDR, BQ40Z50_SAFETYSTATUS_ADDR)) == -1)
				 ++u8err_cnt;
			else {
				gt_gauge_info.u32safety_status = ivalue;
				gt_gauge_info.ugauge_safetystat.u32val = ivalue;
			}
		
		break;

		case 8:
			u8index ++;
			if((ivalue = Read_BQ40Z50_ManuFBAccess_data(GAUGE_SMB_ADDR, BQ40Z50_PFSTATUS_ADDR)) == -1)
				 ++u8err_cnt;
			else
				gt_gauge_info.u32pf_status = ivalue;
		
		break;
		
		default:
		
			u8index = 0;
			
		break;
	}
	/*update SOC alone*/
	if(Ok == TMR0_Delay(TRUE ,&dlySOCData ,TMR0_DELAY_MS(400)))
	{
		if((ivalue = Read_bq40z50_data(GAUGE_SMB_ADDR, BQ40Z50_SOC_ADDR)) == -1 )
			 ++u8err_cnt;
		else
			gt_gauge_info.u16soc = ivalue;		
	}	

		
	
	if(u8err_cnt)
	{
		u8err_cnt = 0;
		return ErrorCommunication;	
	}

	else 
		return Ok;
}
