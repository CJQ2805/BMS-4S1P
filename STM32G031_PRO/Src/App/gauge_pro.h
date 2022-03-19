#ifndef __GAUGE_PRO_H
#define __GAUGE_PRO_H
#include "bsp.h"

#define  GAUGE_SMB_ADDR 	     	  0x16

#define  BQ40Z50_TEMPERATURE_ADDR     0x08
#define  BQ40Z50_VOLTAGE_ADDR         0x09
#define  BQ40Z50_CURRENT_ADDR         0x0A
#define  BQ40Z50_SOC_ADDR			  0x0D
#define  BQ40Z50_EMPTY_TIME_ADDR	  0x12
#define  BQ40Z50_FULL_TIME_ADDR		  0x13
#define  BQ40Z50_BATTERY_STAT_ADDR    0x16
#define  BQ40Z50_SOH_ADDR			  0x4F

#define  BQ40Z50_SAFETYSTATUS_ADDR    0x0051
#define  BQ40Z50_ID  				  0x0006
#define  BQ40Z50_PFSTATUS_ADDR        0x0053

typedef __packed struct _gauge_safetystatus_t{

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

}t_gauge_safetystatus;

typedef __packed  union _gauge_safetystatus_u{
	uint32_t  u32val;
	t_gauge_safetystatus  tgauge_safetystat;
	
}u_gauge_safetystatus;

struct  _gauge_info_t {

	uint16_t  u16temperature;	
	uint16_t  u16voltage;
	int  	  icurrent;	
	uint16_t  u16soc;
	uint16_t  u16empty_time;
	uint16_t  u16full_time;
	uint16_t  u16battery_stat;
	uint16_t  u16soh;
	uint32_t  u32safety_status;
	uint32_t  u32pf_status;	//”¿æ√π ’œ
	u_gauge_safetystatus ugauge_safetystat;
};

extern struct  _gauge_info_t   gt_gauge_info;


void Gauge_Init(void);
void GAUGE_CommTask(void);
#endif
