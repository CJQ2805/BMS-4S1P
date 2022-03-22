#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "bsp.h" 

#define LED_LIGHT_ON			0
#define LED_LIGHT_OFF			1

#define LED1				  PBout(0)
#define LED2				  PBout(1)
#define LED3				  PBout(2)
#define LED4				  PBout(3)

enum led_soc_percent_e{

	SOC_PERCENT_LOW_POWER,
	SOC_PERCENT_5_25,
	SOC_PERCENT_25_50,
	SOC_PERCENT_50_75,
	SOC_PERCENT_75_100,
};

enum _dis_charge_stat_e{
	
	DIS_CHARGE_NONE = 0,
	DIS_CHARGE_CHARGE,
	DIS_CHARGE_DISCHARGE,
	DIS_CHARGE_CHARG_AND_DISCHARG,
	DIS_CHARGE_FAULT,
};


void led_erro_display(void);
void led_Disply(enum _dis_charge_stat_e estat, uint16_t u16soc);
#endif

