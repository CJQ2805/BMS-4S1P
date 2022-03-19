#ifndef __BMS_PRO_H
#define __BMS_PRO_H
#include "bsp.h"


typedef __packed struct _bms_gauge_safetystat_t
{
	uint32_t   CUV     :    1;
	uint32_t   COV     :    1;
	uint32_t   OCC1    :    1;
	uint32_t   OCC2    :    1;
	uint32_t   OCD1    :    1;
	uint32_t   OCD2    :    1;
	uint32_t   AOLD    :    1;
	uint32_t   AOLDL   :    1;
	uint32_t   ASCC    :    1;
	uint32_t   ASCCL   :    1;
	uint32_t   ASCD    :    1;
	uint32_t   ASCDL   :    1;
	uint32_t   OTC     :    1;	
	uint32_t   OTD     :    1;	
	uint32_t   CUVC    :    1;	
	uint32_t   RSVD4   :    1;	
	uint32_t   OTF     :    1;	
	uint32_t   RSVD3   :    1;
	uint32_t   PTO     :    1;	
	uint32_t   RSVD2   :    1;
	uint32_t   CTO     :    1;
	uint32_t   RSVD1   :    1;	
	uint32_t   OC      :    1;	
	uint32_t   CHGC    :    1;	
	uint32_t   CHGV    :    1;
	uint32_t   PCHGC   :    1;
	uint32_t   UTC     :    1;
	uint32_t   UTD     :    1;	
	uint32_t   COVL    :    1;
	uint32_t   OCDL    :    1;
	uint32_t   RSVD0   :    2;	
	
}t_bms_gauge_safetystat;


typedef __packed  union _bms_gauge_safetystat_u
{
	uint32_t  u32val;
	t_bms_gauge_safetystat tbms_gauge_safetystat; 

}u_bms_gauge_safetystat;


typedef __packed  struct  _bms_gague_info_t{

	uint16_t u16bat_volt;
	uint16_t u16soc;
	uint16_t u16soh;
	uint16_t u16bat_temperature;
	int      ibat_cur;	

}t_bms_gague_info;


typedef __packed struct  _bms_dcdc_info_t{

	uint16_t u16dc_chg_cur;
	uint16_t u16dc_max_volt;
	uint16_t u16dc_otg_volt;
	uint16_t u16dc_otg_cur;
	uint16_t u16dc_input_volt;
	uint16_t u16dc_min_sys_volt;
	uint16_t u16dc_input_cur;
	
	uint8_t  u8fault_stat;
	
}t_bms_dcdc_info;

__packed  struct _bms_temporary_info_t{
	
	/*gauge info*/
	uint16_t u16bat_volt;
	uint16_t u16soc;
	uint16_t u16soh;
	uint16_t u16bat_temperature;
	int      ibat_cur;		

};

enum _bms_adapter_stat_e{
	
	BMS_ADAPTER_NONE = 0,
	BMS_ADAPTER_ONLINE,
	
};

enum _bms_charge_stat_e{
	
	BMS_CHARGE_NONE = 0,
	BMS_CHARGE_CHARGE,
	BMS_CHARGE_DISCHARGE,
	BMS_CHARGE_CHARG_AND_DISCHARG,
};

enum _bms_cap_stat_e{
	
	BMS_CAP_NORMAL,
	BMS_CAP_FULL,
	BMS_CAP_LOW,

};

enum _bms_fault_stat_e{

	BMS_FAULT_NONE,
	BMS_FAULT_TEMP, //ÁÙÊ±
	BMS_FAULT_PERMANENT,
};

struct _bms_stat_t{

	enum _bms_adapter_stat_e   adapter_stat;
	enum _bms_charge_stat_e    charge_stat;
	enum _bms_cap_stat_e       bat_cap_stat;
	enum _bms_fault_stat_e     fault_stat;
};


struct _bms_info_t{
	
	uint8_t  u8bms_key_stat;		//°´¼ü×´Ì¬

	/*gauge info*/
	t_bms_gague_info  tbms_gague_info;
	/*dcdc info*/
	t_bms_dcdc_info   tbms_dcdc_info;
	
	/*stat*/
	struct _bms_stat_t    		   tbms_stat;
	struct _bms_temporary_info_t   t_temporary_info;
	u_bms_gauge_safetystat   ubms_gauge_safetystat;
	
};



	
void BMS_ProTask(void);


#endif

