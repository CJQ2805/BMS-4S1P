#include "dcdc_pro.h"
#include "bsp.h"

/**
	@breif
	@Author: CJQ2805
	DCDC Model: SC8886
*/

struct  _dcdc_info_t  gt_dcdc_info; 
static void set_dcdc_data(uint8_t u8register_address, uint8_t datalen, uint8_t* pu8data);
static void DCDC_UpdateData(void);
static void DCDC_FaultHandle(void);

void DCDC_Init(void)
{
	memset(&gt_dcdc_info, 0x00, sizeof(gt_dcdc_info));
	
	uint8_t au8iic_data[2]= {0};
	
//	uint16_t u16charge_current  			= 3000;  //3A
//	uint16_t u16max_charge_voltage			= 16288; //16.2V  充电截止电压，根据电池串数决定

	/*set charge option0 PWM Freq 1200Khz*/
	gt_dcdc_info.u16charge_option0 = 0x0EE5;	
	au8iic_data[0] = (gt_dcdc_info.u16charge_option0 >> 8)&0xff;
	au8iic_data[1] = gt_dcdc_info.u16charge_option0&0xff;
	set_dcdc_data(DCDC_CHARGE_OPTION0, DCDC_IIC_DATA_LEN_TWO, au8iic_data);
}


/*******************************Read****************************/
static void read_dcdc_chargecurrent(void)
{
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_CURRENT_ADDR);
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_CURRENT_ADDR+1);
	
	gt_dcdc_info.u16charge_current = (((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0] ;	
}

static void read_dcdc_input_voltage(void)
{
	//这里感觉有点问题 ，需要float类型
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_INPUT_VOLT_SET_ADDR);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_INPUT_VOLT_SET_ADDR+1);
	
	gt_dcdc_info.u16input_voltage = ((((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0]);
}


static void read_dcdc_max_charge_voltage(void)
{
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_MAX_CHARGE_VOLT_ADDR);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_MAX_CHARGE_VOLT_ADDR+1);
	
	gt_dcdc_info.u16max_charge_voltage = ((((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0]);	
}

static void read_dcdc_otg_voltage(void)
{
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_OTG_VLOT_ADDR);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_OTG_VLOT_ADDR +1);	

	gt_dcdc_info.u16otg_voltage = (((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0];
}

static void read_dcdc_otg_current(void)
{
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_OTG_CUR_ADDR);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_OTG_CUR_ADDR +1);

	gt_dcdc_info.u16otg_current = (((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0];
}


static void read_dcdc_input_current(void)
{	
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_INPUT_CUR_SET_ADDR);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_INPUT_CUR_SET_ADDR +1);	

	gt_dcdc_info.u16input_current = (((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0];
}

static void read_dcdc_charge_option0(void)
{	
	uint8_t au8iic_data[2]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,0x00);	
	au8iic_data[1] = I2C_HostRead(DCDC_IIC_ADDRESS,0x01);
	
	gt_dcdc_info.u16charge_option0 = (((uint16_t)au8iic_data[1] << 8)) + au8iic_data[0];	
}

static void read_dcdc_charge_option1(void)
{	
	uint8_t au8iic_data[]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_OPTION1_0);		
	gt_dcdc_info.udcdc_charge_option1.u8val = au8iic_data[0];	
}

static void read_dcdc_charge_option2(void)
{	
	uint8_t au8iic_data[]= {0};	
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_OPTION2_0);		
	gt_dcdc_info.udcdc_charge_option2.u8val = au8iic_data[0];	
}



static void read_dcdc_charge_status1(void)
{
	uint8_t au8iic_data[] = {0};
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_STATUS_0);
	
	gt_dcdc_info.udcdc_charge_status0.u8val = au8iic_data[0];
}

static void read_dcdc_charge_status2(void)
{
	uint8_t au8iic_data[] = {0};
	au8iic_data[0] = I2C_HostRead(DCDC_IIC_ADDRESS,DCDC_CHARGE_STATUS_1);	

	gt_dcdc_info.udcdc_charge_status1.u8val = au8iic_data[0];	
}


/*******************************Set_DATA**********************************/

static void set_dcdc_data(uint8_t u8register_address, uint8_t datalen, uint8_t* pu8data)
{
	I2C_Cont_Write(DCDC_IIC_ADDRESS, u8register_address, datalen, pu8data);	
}


static void set_dcdc_current_02(uint16_t u16current)
{
	uint8_t au8data[2]= {0};	
	au8data[0] = u16current &0xff;
	au8data[1] = (u16current >> 8)&0xff;
	I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_CURRENT_ADDR, DCDC_IIC_DATA_LEN_TWO, au8data);		
}
	
static void set_dcdc_otg_voltage_06(uint16_t u16voltage)
{
	uint8_t au8data[2]= {0};	
	au8data[0] = u16voltage &0xff;
	au8data[1] = (u16voltage >> 8)&0xff;
	I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_OTG_VLOT_ADDR, DCDC_IIC_DATA_LEN_TWO, au8data);			
}

static void set_dcdc_otg_current_08(uint16_t u16current)
{
	uint8_t au8data[2]= {0};	
	au8data[0] = u16current &0xff;
	au8data[1] = (u16current >> 8)&0xff;
	I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_OTG_CUR_ADDR, DCDC_IIC_DATA_LEN_TWO, au8data);		
}

static void set_dcdc_input_current_0e(uint16_t u16current)
{
	uint8_t au8data[2]= {0};	
	au8data[0] = u16current &0xff;
	au8data[1] = (u16current >> 8)&0xff;
	I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_INPUT_CUR_SET_ADDR, DCDC_IIC_DATA_LEN_TWO, au8data);	
}
	
static void set_dcdc_otg_option3_start_35(bool otg_stat)
{
	u_dcdc_charge_option3_0  udcdc_charge_option3_0;
	u_dcdc_charge_option3_1  udcdc_charge_option3_1;
	if(otg_stat)
	{
		udcdc_charge_option3_0.tdcdc_charge_option3_0.OTG_VAP_MODE = 1;
		udcdc_charge_option3_1.tdcdc_charge_option3_1.EN_OTG = 1;
		I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_OPTION3_0, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_option3_0.u8val);
		I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_OPTION3_1, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_option3_1.u8val);		
	}
	else
	{
		udcdc_charge_option3_0.tdcdc_charge_option3_0.OTG_VAP_MODE = 0;
		udcdc_charge_option3_1.tdcdc_charge_option3_1.EN_OTG = 0;
		I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_OPTION3_0, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_option3_0.u8val);
		I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_OPTION3_1, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_option3_1.u8val);	
	}
}


static void DCDC_UpdateData(void)
{
	static uint8_t u8index = 0;
	static u32_tim_dly_t update_tim_dly;
	
	if(Ok != TMR0_Delay(TRUE ,&update_tim_dly ,TMR0_DELAY_MS(500)))
	return;
	
	switch(u8index)
	{
		case 0:
		u8index++;			
		read_dcdc_chargecurrent();			
		read_dcdc_input_voltage();				
		read_dcdc_max_charge_voltage();		
		

		
		case 1:
		u8index++;				
		read_dcdc_charge_option1();					
		read_dcdc_charge_option2();					
		
		break;
		
		case 2:
		u8index++;
		read_dcdc_charge_option0();			
		read_dcdc_charge_status1();					
		read_dcdc_charge_status2();			
	
		break;
		
		case 3:
		u8index++;			
		read_dcdc_otg_current();					
		read_dcdc_otg_voltage();					
		read_dcdc_input_current();			
		
		break;
		
		
		default:
			u8index = 0;
		break;
	}
		

}


static void DCDC_FaultHandle(void)
{
	u_dcdc_charge_status0   udcdc_charge_status0;
	u_dcdc_charge_option1	udcdc_charge_option1;
	udcdc_charge_option1.u8val = gt_dcdc_info.udcdc_charge_option1.u8val;
	udcdc_charge_status0.u8val = gt_dcdc_info.udcdc_charge_status0.u8val;	
	if(gt_dcdc_info.udcdc_charge_status1.u8val)
	{
		/*clear SYSSHORT_STAT fault and clear STSOVP_STAT fault*/
		if(gt_dcdc_info.udcdc_charge_status0.tdcdc_charge_status0.SYSSHORT_STAT
		|| gt_dcdc_info.udcdc_charge_status0.tdcdc_charge_status0.SYSOVP_STAT
		)
		{
			udcdc_charge_status0.tdcdc_charge_status0.SYSSHORT_STAT = 0;
			udcdc_charge_status0.tdcdc_charge_status0.SYSOVP_STAT = 0;		
			I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_STATUS_0, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_status0.u8val);				
		}
		/*clear FAULT_LATCHOFF stat /set option1 (for datasheet)*/
		if(gt_dcdc_info.udcdc_charge_status0.tdcdc_charge_status0.FAULT_LATCHOFF)
		{
			udcdc_charge_option1.tdcdc_charge_option1.EN_FORCE_LATCHOFF = 0;
			I2C_Cont_Write(DCDC_IIC_ADDRESS, DCDC_CHARGE_OPTION1_0, DCDC_IIC_DATA_LEN_ONE, &udcdc_charge_option1.u8val);				
		}	
	}
}

void DCDC_CommTask(void)
{
	DCDC_FaultHandle();
	DCDC_UpdateData();
}

struct _dcdc_interface_t gt_dcdc_interface = {

	.set_dcdc_info = set_dcdc_data,
	.set_dcdc_current = set_dcdc_current_02,
	.set_dcdc_otg_voltage = set_dcdc_otg_voltage_06,
	.set_dcdc_otg_current = set_dcdc_otg_current_08,
	.set_dcdc_otg_option3_start = set_dcdc_otg_option3_start_35,
	.set_dcdc_input_current = set_dcdc_input_current_0e,
	
};
