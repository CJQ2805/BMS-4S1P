#include "bms_pro.h"
#include "bsp.h"
#include "display.h"
/**
	@breif 
	@Author:CJQ2805  
	BMS Control Code
*/
struct _bms_info_t   	gt_bms_info;

static void BMS_KeyScan(void);
static void BMS_FullChargeCap(void);
static void BMS_GaugeInfoUpdate(void);
static void BMS_DCDCInfoUpdate(void);
static void BMS_FaultHandle(void);
static void BMS_SetSafetyStatus(void);
static void BMS_ClearSafetyStatus(void);
static void BMS_DCDCCharge(void);
static void BMS_Display(void);
static void BMS_LowPowerMode(void);

void BMS_ProTask(void)
{
	BMS_GaugeInfoUpdate();
	BMS_DCDCInfoUpdate();
	BMS_FullChargeCap();
	BMS_LowPowerMode();	
	BMS_FaultHandle();
	BMS_DCDCCharge();
//	BMS_KeyScan();	暂时没做
	BMS_Display();
}




static void BMS_FullChargeCap(void)
{
	static u32_tim_dly_t dlyFullCharge;
	/*SOC大于等于100, 电流小于400mA*/
	
	if(gt_bms_info.tbms_stat.bat_cap_stat != BMS_CAP_FULL)
	{
		/* 3 second judgement FCC stat*/
		if(Ok == TMR0_Delay(((gt_bms_info.tbms_gague_info.u16soc >= 100) || (gt_bms_info.tbms_gague_info.ibat_cur < 400)),
							 &dlyFullCharge,
							 TMR0_DELAY_SEC(3)))
		{
			if(gt_bms_info.tbms_dcdc_info.u16dc_input_cur != 0)
			{
				gt_bms_info.tbms_stat.bat_cap_stat = BMS_CAP_FULL;
				gt_dcdc_interface.set_dcdc_current(0);
			}
		}		

	}
	else{
		
		if(BMS_CAP_NORMAL == gt_bms_info.tbms_stat.bat_cap_stat)
		return;	
		
			if(gt_bms_info.tbms_gague_info.u16soc < 98)
			{
				gt_bms_info.tbms_stat.bat_cap_stat = BMS_CAP_NORMAL; 
			}
	}		
}

static void BMS_GaugeInfoUpdate(void)
{
	static u32_tim_dly_t  dlyGaugeInfo;

	if(Ok != TMR0_Delay(TRUE, &dlyGaugeInfo, TMR0_DELAY_MS(250)))
	{
		return;
	}

	else
	gt_bms_info.tbms_gague_info.ibat_cur 		   = gt_gauge_info.icurrent;
	
	gt_bms_info.tbms_gague_info.u16bat_volt        = gt_gauge_info.u16voltage;
	
	if(gt_gauge_info.u16soc <= 100)
	gt_bms_info.tbms_gague_info.u16soc             = gt_gauge_info.u16soc;
	if(gt_gauge_info.u16soh <= 100)
	gt_bms_info.tbms_gague_info.u16soh             = gt_gauge_info.u16soh;
	
	gt_bms_info.tbms_gague_info.u16bat_temperature = gt_gauge_info.u16temperature;	
}



static void BMS_FaultHandle(void)
{
	static u32_tim_dly_t DlyFaultHandle;
	BMS_SetSafetyStatus();
	BMS_ClearSafetyStatus();
	
	if(Ok != TMR0_Delay((gt_bms_info.ubms_gauge_safetystat.u32val) || 
						(gt_bms_info.tbms_dcdc_info.u8fault_stat), 
						&DlyFaultHandle, 
						TMR0_DELAY_SEC(2)))
	{
		gt_bms_info.tbms_stat.fault_stat = BMS_FAULT_TEMP;
	}
	
	if(Ok != TMR0_Delay((!gt_bms_info.ubms_gauge_safetystat.u32val) || 
						(!gt_bms_info.tbms_dcdc_info.u8fault_stat), 
						&DlyFaultHandle, 
						TMR0_DELAY_MS(500)))
	{
		gt_bms_info.tbms_stat.fault_stat = BMS_FAULT_NONE;
	}
	

	
	if((gt_bms_info.tbms_stat.fault_stat == BMS_FAULT_TEMP) || (gt_bms_info.tbms_stat.fault_stat == BMS_FAULT_PERMANENT))
	{
		/*forbid charge 	这里暂时采用DCDC充电电流为0的方案进行设计，所以这里只操作了电流为0，没有做无故障的时候电流设置，电流设置在充电判断中设置了，后续这个应该改成DCDC充电使能字段*/
		gt_dcdc_interface.set_dcdc_current(0);
	}
	else{
		/*restart charging*/
	
	}
}


static void BMS_DCDCInfoUpdate(void)
{
	static u32_tim_dly_t  dlyDcDcInfo;

	if(Ok != TMR0_Delay(TRUE, &dlyDcDcInfo, TMR0_DELAY_MS(200)))
	{
		return;
	}
	/*important basics parameter */
	gt_bms_info.tbms_dcdc_info.u16dc_chg_cur      = gt_dcdc_info.u16charge_current;
	gt_bms_info.tbms_dcdc_info.u16dc_input_cur    = gt_dcdc_info.u16input_current;
	gt_bms_info.tbms_dcdc_info.u16dc_input_volt   = gt_dcdc_info.u16input_voltage;
	gt_bms_info.tbms_dcdc_info.u16dc_max_volt     = gt_dcdc_info.u16max_charge_voltage;
	gt_bms_info.tbms_dcdc_info.u16dc_min_sys_volt = gt_dcdc_info.u16min_sys_voltage;
	gt_bms_info.tbms_dcdc_info.u16dc_otg_cur      = gt_dcdc_info.u16otg_current;
	gt_bms_info.tbms_dcdc_info.u16dc_otg_volt     = gt_dcdc_info.u16otg_voltage;

	/*fault parameter*/
	gt_bms_info.tbms_dcdc_info.u8fault_stat       = gt_dcdc_info.udcdc_charge_status0.u8val;
	
	/*BMS adapter stat*/
	gt_bms_info.tbms_stat.adapter_stat			  = gt_dcdc_info.udcdc_charge_status1.tdcdc_charge_status1.AC_STAT;
	
}


static void BMS_DCDCCharge(void)
{
	
	static u32_tim_dly_t  dlyDcDcCharge, dlyChargeStat;
	
	if(Ok == TMR0_Delay(((gt_bms_info.tbms_stat.bat_cap_stat != BMS_CAP_FULL) &&
						(gt_bms_info.tbms_stat.fault_stat != BMS_FAULT_TEMP)),
						&dlyDcDcCharge, TMR0_DELAY_SEC(1)))
	{
		if(!gt_bms_info.tbms_dcdc_info.u16dc_chg_cur)
		{
			gt_dcdc_interface.set_dcdc_current(3000);
			return;
		}

	}	
	/*update charge stat*/
	if(Ok == TMR0_Delay(TRUE,&dlyChargeStat, TMR0_DELAY_MS(100)))
	{
		if(gt_bms_info.tbms_gague_info.ibat_cur > (200))
		{
			if(BMS_CHARGE_CHARGE != gt_bms_info.tbms_stat.charge_stat)
			{	
				gt_bms_info.tbms_stat.charge_stat = BMS_CHARGE_CHARGE;
			}
		}	
		else if(((gt_bms_info.tbms_gague_info.ibat_cur >( -300)) && (gt_bms_info.tbms_gague_info.ibat_cur < 201)) ||
				  gt_bms_info.tbms_stat.adapter_stat == BMS_ADAPTER_NONE)
		{
			if(BMS_CHARGE_NONE != gt_bms_info.tbms_stat.charge_stat)
			{				
				gt_bms_info.tbms_stat.charge_stat = BMS_CHARGE_NONE;	
			}
		}
		else
		{
			if(BMS_CHARGE_NONE != gt_bms_info.tbms_stat.charge_stat)
			{				
				gt_bms_info.tbms_stat.charge_stat = BMS_CHARGE_DISCHARGE;	
			}		
		
		}
	}
		
}

static void BMS_LowPowerMode(void)
{
	static u32_tim_dly_t dlyLowPower;
	if( Ok == TMR0_Delay(((BMS_CHARGE_NONE == gt_bms_info.tbms_stat.charge_stat)||
						  (BMS_CHARGE_FAULT == gt_bms_info.tbms_stat.charge_stat)), 
						   &dlyLowPower, TMR0_DELAY_SEC(10)) )
	{
		WakeUp_GPIOSet();
//		led_light_off();
//		printf("\r\n enter sleep mode!");	
//		SYS_EnterSTOPMode(SYS_PWR_WFI);	//进入停止模式
	}
}

static void BMS_Display(void)
{

	switch(gt_bms_info.tbms_stat.charge_stat)
	{
		case BMS_CHARGE_NONE:
		case BMS_CHARGE_DISCHARGE:	

		led_Disply((enum _dis_charge_stat_e)BMS_CHARGE_NONE, gt_bms_info.tbms_gague_info.u16soc);
		
		break;
		
		
		case BMS_CHARGE_CHARGE:
		case BMS_CHARGE_CHARG_AND_DISCHARG:
			
		led_Disply((enum _dis_charge_stat_e)BMS_CHARGE_CHARGE, gt_bms_info.tbms_gague_info.u16soc);	
		
		break;
		
		case BMS_CHARGE_FAULT:
			
		led_Disply((enum _dis_charge_stat_e)BMS_CHARGE_FAULT, gt_bms_info.tbms_gague_info.u16soc);	
		
		break;
	
	}
}

/*BMS key_Scan has not been implemented*/

//static void BMS_KeyScan(void)
//{
//	key_Scan(&gt_bms_info.u8bms_key_stat);  //获取按键情况	
//	if(gt_bms_info.u8bms_key_stat)
//	{
//		/* key logic processing */
//		gt_bms_info.u8bms_key_stat = 0;	
//	}
//}


static void BMS_SetSafetyStatus(void)
{
	/*SET*/
	static u32_tim_dly_t  CUV_SET_dly, COV_SET_dly, OCC1_SET_dly, OCC2_SET_dly, OCD1_SET_dly, OCD2_SET_dly, 
	AOLD_SET_dly, AOLDL_SET_dly, ASCC_SET_dly, ASCCL_SET_dly, ASCD_SET_dly, ASCDL_SET_dly, OTC_SET_dly,
	OTD_SET_dly, CUVC_SET_dly, OTF_SET_dly, PTO_SET_dly, CTO_SET_dly, OC_SET_dly, CHGC_SET_dly, CHGV_SET_dly,
	PCHGC_SET_dly, UTC_SET_dly, UTD_SET_dly, COVL_SET_dly, OCDL_SET_dly;
	
	/*set AOLD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.AOLD, &AOLD_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.AOLD = 1;	

	/*set AOLDL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.AOLDL, &AOLDL_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.AOLDL = 1;	
	
	/*set ASCC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCC, &ASCC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCC = 1;	

	/*set ASCCL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCCL, &ASCCL_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCCL = 1;	
	
	/*set ASCD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCD, &ASCD_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCD = 1;

	/*set ASCDL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCDL, &ASCDL_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCDL = 1;	
	
	/*set CHGC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CHGC, &CHGC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CHGC = 1;	
	
	/*set CHGV*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CHGV, &CHGV_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CHGV = 1;
	
	/*set COV*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.COV, &COV_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.COV = 1;
	
	/*set COVL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.COVL, &COVL_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.COVL = 1;

	/*set CTO*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CTO, &CTO_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CTO = 1;
	
	/*set CUV*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CUV, &CUV_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CUV = 1;	

	/*set CUVC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CUVC, &CUVC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CUVC = 1;

	/*set OC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OC, &OC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OC = 1;	
	
	/*set OCC1*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCC1, &OCC1_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCC1 = 1;	
	
	/*set OCC2*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCC2, &OCC2_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCC2 = 1;	
	
	/*set OCD1*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCD1, &OCD1_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCD1 = 1;	
	
	/*set OCD2*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCD2, &OCD2_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCD2 = 1;

	
	/*set OCDL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCDL, &OCDL_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCDL = 1;

	/*set OTC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTC, &OTC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTC = 1;	
	

	/*set OTD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTD, &OTD_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTD = 1;	
	
	/*set OTF*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTF, &OTF_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTF = 1;	
	
	/*set PCHGC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.PCHGC, &PCHGC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.PCHGC = 1;	
	
	/*set PTO*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.PTO, &PTO_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.PTO = 1;

	/*set UTC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.UTC, &UTC_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.UTC = 1;	
	
	/*set UTD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.UTD, &UTD_SET_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.UTD = 1;	
}

static void BMS_ClearSafetyStatus(void)
{

	/*CLEAR*/	
	static u32_tim_dly_t  CUV_CLEAR_dly, COV_CLEAR_dly, OCC1_CLEAR_dly, OCC2_CLEAR_dly, OCD1_CLEAR_dly, OCD2_CLEAR_dly, 
	AOLD_CLEAR_dly, AOLDL_CLEAR_dly, ASCC_CLEAR_dly, ASCCL_CLEAR_dly, ASCD_CLEAR_dly, ASCDL_CLEAR_dly, OTC_CLEAR_dly,
	OTD_CLEAR_dly, CUVC_CLEAR_dly, OTF_CLEAR_dly, PTO_CLEAR_dly, CTO_CLEAR_dly, OC_CLEAR_dly, CHGC_CLEAR_dly, CHGV_CLEAR_dly,
	PCHGC_CLEAR_dly, UTC_CLEAR_dly, UTD_CLEAR_dly, COVL_CLEAR_dly, OCDL_CLEAR_dly;	
	

	/*clear AOLD*/	
	if(Ok == TMR0_Delay(!gt_gauge_info.ugauge_safetystat.tgauge_safetystat.AOLD, &AOLD_CLEAR_dly, TMR0_DELAY_MS(2000)))	
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.AOLD = 0;
	
	/*clear AOLDL*/
	if(Ok == TMR0_Delay(!gt_gauge_info.ugauge_safetystat.tgauge_safetystat.AOLDL, &AOLDL_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.AOLDL = 0;	
	
	/*clear ASCC*/	
	if(Ok == TMR0_Delay(!gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCC, &ASCC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCC = 0;

	/*clear ASCCL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCCL, &ASCCL_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCCL = 0;

	/*clear ASCD*/
	if(Ok == TMR0_Delay(!gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCD, &ASCD_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCD = 0;

	/*clear ASCDL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.ASCDL, &ASCDL_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.ASCDL = 0;
	
	/*clear CHGC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CHGC, &CHGC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CHGC = 0;

	/*clear CHGV*/	
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CHGV, &CHGV_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CHGV = 0;

	/*clear COV*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.COV, &COV_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.COV = 0;	
	
	/*clear COVL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.COVL, &COVL_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.COVL = 0;	

	/*clear CTO*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CTO, &CTO_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CTO = 0;
	
	/*clear CUV*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CUV, &CUV_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CUV = 0;	
	
	/*clear CUVC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.CUVC, &CUVC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.CUVC = 0;

	/*clear OC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OC, &OC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OC = 0;	
	
	/*clear OCC1*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCC1, &OCC1_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCC1 = 0;		
		
	/*clear OCC2*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCC2, &OCC2_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCC2 = 0;
	
	/*clear OCD1*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCD1, &OCD1_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCD1 = 0;	
	
	/*clear OCD2*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCD2, &OCD2_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCD2 = 0;	
	
	/*clear OCDL*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OCDL, &OCDL_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OCDL = 0;	
	
	/*clear OTC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTC, &OTC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTC = 0;	
	
	/*clear OTD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTD, &OTD_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTD = 0;	
		
	/*clear OTF*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.OTF, &OTF_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.OTF = 0;	
	
	/*clear PCHGC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.PCHGC, &PCHGC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.PCHGC = 0;		
	
	/*clear PTO*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.PTO, &PTO_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.PTO = 0;	
	
	/*clear UTC*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.UTC, &UTC_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.UTC = 0;	

	/*clear UTD*/
	if(Ok == TMR0_Delay(gt_gauge_info.ugauge_safetystat.tgauge_safetystat.UTD, &UTD_CLEAR_dly, TMR0_DELAY_MS(2000)))
		gt_bms_info.ubms_gauge_safetystat.tbms_gauge_safetystat.UTD = 0;	
	
}

