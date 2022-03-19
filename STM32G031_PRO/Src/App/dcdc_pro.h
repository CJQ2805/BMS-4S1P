#ifndef __DCDC_PRO_H
#define __DCDC_PRO_H
#include "bsp.h"

#define DCDC_IIC_ADDRESS      		0xD6

#define DCDC_IIC_DATA_LEN_ONE		1
#define DCDC_IIC_DATA_LEN_TWO       2

#define DCDC_CHARGE_OPTION0         0x00
#define	DCDC_CHARGE_CURRENT_ADDR    0x02
#define DCDC_MAX_CHARGE_VOLT_ADDR   0x04
#define DCDC_OTG_VLOT_ADDR          0x06
#define DCDC_OTG_CUR_ADDR 			0x08
#define DCDC_INPUT_VOLT_SET_ADDR	0x0A
#define DCDC_MIN_SYS_VOLT			0x0C
#define DCDC_INPUT_CUR_SET_ADDR     0x0E
//#define DCDC_INPUT_
#define DCDC_CHARGE_STATUS_0		0x20
#define DCDC_CHARGE_STATUS_1		0x21

#define DCDC_CHARGE_OPTION1_0		0x30

#define DCDC_CHARGE_OPTION2_0       0x32

#define DCDC_CHARGE_OPTION3_0       0x34
#define DCDC_CHARGE_OPTION3_1		0X35



typedef __packed struct _dcdc_charge_option0_0_t{
	
	uint8_t  CHRG_INHIBIT    	:    1;
	uint8_t	 EN_IDPM			:	 1;
	uint8_t  EN_LDO				:    1;
	uint8_t	 IBAT_GAIN			:    1;
	uint8_t  IADPT_GAIN			:    1;
	uint8_t  EN_LEARN			:    1;
	uint8_t  SYSSHORT_DIS	    :    1;
	uint8_t  RESERVED		    :    1;
	
}t_dcdc_charge_option0_0;


typedef __packed union _dcdc_charge_option0_0_u{

	uint8_t u8val;
	t_dcdc_charge_option0_0	tdcdc_charge_option0_0;

}u_dcdc_charge_option0_0;


typedef __packed struct _dcdc_charge_option2_t{
	
	uint8_t  BATOC_VTH    	:    1;
	uint8_t	 EN_VATOC		:	 1;
	uint8_t  ACOC_VTH		:    1;
	uint8_t	 EN_ACOC		:    1;
	uint8_t  RESERVED		:    1;
	uint8_t  Q2_OCP_VTH		:    1;
	uint8_t  EN_ICHG_IDCHG  :    1;
	uint8_t  EN_EXTLIM		:    1;
	
}t_dcdc_charge_option2;


typedef __packed union _dcdc_charge_option2_u{

	uint8_t u8val;
	t_dcdc_charge_option2	tdcdc_charge_option2;

}u_dcdc_charge_option2;


typedef __packed struct _dcdc_charge_status0_t{
	
	uint8_t  FAULT_OTG_UVP    	:    1;
	uint8_t	 FAULT_OTG_OVP		:	 1;
	uint8_t  FAULT_LATCHOFF		:    1;
	uint8_t	 SYSSHORT_STAT		:    1;
	uint8_t  SYSOVP_STAT		:    1;
	uint8_t  FAULT_ACOC			:    1;
	uint8_t  FAULT_BATOC	    :    1;
	uint8_t  FAULT_ACOV 		:    1;
	
}t_dcdc_charge_status0;


typedef __packed union _dcdc_charge_status0_u{

	uint8_t u8val;
	t_dcdc_charge_status0	tdcdc_charge_status0;

}u_dcdc_charge_status0;


typedef __packed struct _dcdc_charge_status1_t{
	
	uint8_t  IN_OTG		    	:    1;
	uint8_t	 IN_PCHRG			:	 1;			//是否处于预充电状态
	uint8_t  IN_FCHRG			:    1;			//是否处于快速充电状态
	uint8_t	 IN_IINDPM			:    1;		
	uint8_t  IN_VINDPM			:    1;
	uint8_t  IN_VAP				:    1;
	uint8_t  ICO_DONE		    :    1;
	uint8_t  AC_STAT	 		:    1;			//输入设备存不存在		0:没有输入   1:有输入
	
}t_dcdc_charge_status1;


typedef __packed union _dcdc_charge_status1_u{

	uint8_t u8val;
	t_dcdc_charge_status1	tdcdc_charge_status1;

}u_dcdc_charge_status1;




typedef __packed struct _dcdc_charge_option1_t{
	
	uint8_t  EB_AUTO_WAKEUP    	    :    1;
	uint8_t	 EN_SHIP_DCHG		    :	 1;
	uint8_t  EN_PTM				    :    1;
	uint8_t	 EN_FORCE_LATCHOFF		:    1;
	uint8_t  CMP_DEG				:    2;
	uint8_t  CMP_POL				:    1;
	uint8_t  CMP_REF				:    1;
	
}t_dcdc_charge_option1;


typedef __packed union _dcdc_charge_option1_u{

	uint8_t u8val;
	t_dcdc_charge_option1	tdcdc_charge_option1;

}u_dcdc_charge_option1;



typedef __packed struct _dcdc_charge_option3_0_t{
	
	uint8_t  PSYS_OTG_IDCHG    	:   1;
	uint8_t	 BATFETOFF_HIZ	    :	1;
	uint8_t  OTG_RANGE_LOW	    :   1;
	uint8_t	 IL_AVG             :   2;
	uint8_t  OTG_VAP_MODE		:   1;
	uint8_t  EN_CON_VAP			:   1;
	uint8_t  RESERVED           :   1;
	
}t_dcdc_charge_option3_0;



typedef __packed union _dcdc_charge_option3_0_u{

	uint8_t u8val;
	t_dcdc_charge_option3_0	tdcdc_charge_option3_0;

}u_dcdc_charge_option3_0;




typedef __packed struct _dcdc_charge_option3_1_t{
	
	uint8_t  RESERVED    	:    3;
	uint8_t	 EN_ICO_MODE	:	 1;
	uint8_t  EN_OTG			:    1;
	uint8_t	 RESET_VINDPM   :    1;
	uint8_t  RESET_REG		:    1;
	uint8_t  EN_HIZ			:    1;

}t_dcdc_charge_option3_1;



typedef __packed union _dcdc_charge_option3_1_u{

	uint8_t u8val;
	t_dcdc_charge_option3_1	tdcdc_charge_option3_1;

}u_dcdc_charge_option3_1;



__packed struct  _dcdc_info_t{
	
	uint16_t u16charge_option0;		 // 00 01 	
	uint16_t u16charge_current; 	 // 02 03
	uint16_t u16max_charge_voltage;	 // 04 05
	uint16_t u16otg_voltage;	 	 // 06 07
	uint16_t u16otg_current;		 // 08 09
	uint16_t u16input_voltage;       // 0A 0B  大概有1.3偏差，+1.3得到外部电压值
	uint16_t u16min_sys_voltage;	 // 0C 0D
	uint16_t u16input_current;	     // OE 0F
		
	u_dcdc_charge_status0    udcdc_charge_status0;	  //0x20
	u_dcdc_charge_status1    udcdc_charge_status1;	  //0x21
	
	u_dcdc_charge_option1    udcdc_charge_option1;	  //0x30
	u_dcdc_charge_option2    udcdc_charge_option2;    //0x32
	u_dcdc_charge_option3_0  udcdc_charge_option3_0;  //0x34 REG0x35[4] =1 and REG0x34[5] = 1
	u_dcdc_charge_option3_1  udcdc_charge_option3_1;  //0x35  设置OTG反向充电使用	
	
};



struct _dcdc_interface_t{
	
	void (*set_dcdc_info)(uint8_t u8register_address, uint8_t datalen, uint8_t* pu8data);
	void (*set_dcdc_current)(uint16_t u16current);
	void (*set_dcdc_otg_voltage)(uint16_t u16voltage);
	void (*set_dcdc_otg_current)(uint16_t u16current);
	void (*set_dcdc_otg_option3_start)(bool otg_stat);
	void (*set_dcdc_input_current)(uint16_t u16current);
};

extern struct _dcdc_interface_t gt_dcdc_interface;
extern struct _dcdc_info_t	gt_dcdc_info;


void DCDC_CommTask(void);
void DCDC_Init(void);

#endif

