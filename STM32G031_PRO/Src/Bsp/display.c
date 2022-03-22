#include "display.h"
#include "bsp.h"

void Display_GpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);  
  
}


void led_5_25_display(enum _dis_charge_stat_e estat)
{
	if(estat == BMS_CHARGE_CHARGE)
	{
		//充电逻辑在这里
		LED1 =!LED1;
		LED2 = LED_LIGHT_OFF;
		LED3 = LED_LIGHT_OFF;
		LED4 = LED_LIGHT_OFF;	
		
	}
	else if(estat == DIS_CHARGE_FAULT)
	{
		led_erro_display();					
	}
	else{
		//不是充电就不闪烁
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_OFF;
		LED3 = LED_LIGHT_OFF;
		LED4 = LED_LIGHT_OFF;	
	
	}
}

void led_25_50_display(enum _dis_charge_stat_e estat)
{
	if(estat == BMS_CHARGE_CHARGE)
	{
		LED1 = LED_LIGHT_ON;
		LED2 =!LED2;
		LED3 = LED_LIGHT_OFF;
		LED4 = LED_LIGHT_OFF;
	}
	else if(estat == DIS_CHARGE_FAULT)
	{
		led_erro_display();					
	}
	else
	{
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_ON;
		LED3 = LED_LIGHT_OFF;
		LED4 = LED_LIGHT_OFF;
	}
}


void led_50_75_display(enum _dis_charge_stat_e estat)
{
	if(estat == BMS_CHARGE_CHARGE)
	{
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_ON;
		LED3 = !LED3;
		LED4 = LED_LIGHT_OFF;
	}
	else if(estat == DIS_CHARGE_FAULT)
	{
		led_erro_display();			
	}	
	else
	{
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_ON;
		LED3 = LED_LIGHT_ON;
		LED4 = LED_LIGHT_OFF;		
	}
}


void led_75_100_display(enum _dis_charge_stat_e estat)
{
	if(estat == BMS_CHARGE_CHARGE)
    {
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_ON;
		LED3 = LED_LIGHT_ON;
		LED4 = !LED4;
	}
	else if(estat == DIS_CHARGE_FAULT)
	{
		led_erro_display();					
	}	
	else
	{
		LED1 = LED_LIGHT_ON;
		LED2 = LED_LIGHT_ON;
		LED3 = LED_LIGHT_ON;
		LED4 = LED_LIGHT_ON;
	}
}

enum led_soc_percent_e led_SocToDisplay(uint16_t u16soc)
{
	
	if( u16soc > 75 && u16soc <= 100)
	{
		return SOC_PERCENT_75_100;
	}
	else if(u16soc > 50 && u16soc <= 75)
	{
		return SOC_PERCENT_50_75;
	}
	else if(u16soc > 25 && u16soc <= 49)
	{
		return SOC_PERCENT_25_50;
	}
	else if(u16soc > 5 && u16soc <= 25)
	{
		return SOC_PERCENT_5_25;	
	}
	else
	{
		return SOC_PERCENT_LOW_POWER;	
	}
}

void led_Disply(enum _dis_charge_stat_e estat, uint16_t u16soc)
{
	static u32_tim_dly_t DlyLedDisplay, DlyLowPower;
	enum led_soc_percent_e  esoc_percent;
	
	esoc_percent = led_SocToDisplay(u16soc);
	
	switch(esoc_percent)
	{
		
		case SOC_PERCENT_LOW_POWER:
			
		if(estat == BMS_CHARGE_CHARGE)
		{
			//充电逻辑在这里
			if(Ok == TMR0_Delay(TRUE,&DlyLowPower,TMR0_DELAY_MS(500)))
			{
				
				LED1 =!LED1;
			}	
			
		}
		else if(estat == DIS_CHARGE_FAULT)
		{
			led_erro_display();					
		}		
		else{
			
			if(Ok == TMR0_Delay(TRUE,&DlyLowPower,TMR0_DELAY_MS(200)))
			{
				
				LED1 =!LED1;
			}		
		}		

		
		break;
		
		case SOC_PERCENT_5_25:
			
		if(Ok == TMR0_Delay(TRUE,&DlyLedDisplay,TMR0_DELAY_MS(500)))
		{
			led_5_25_display(estat);
		}			
	
		break;
		
		case SOC_PERCENT_25_50:
			
		if(Ok == TMR0_Delay(TRUE,&DlyLedDisplay,TMR0_DELAY_MS(500)))
		{
			led_25_50_display(estat);
		}	
		
		break;
		
		case SOC_PERCENT_50_75:
			
		if(Ok == TMR0_Delay(TRUE,&DlyLedDisplay,TMR0_DELAY_MS(500)))
		{
			led_50_75_display(estat);
		}
		
		break;
		
		case SOC_PERCENT_75_100:
			
		if(Ok == TMR0_Delay(TRUE,&DlyLedDisplay,TMR0_DELAY_MS(500)))
		{
			led_75_100_display(estat);
		}	
		
		break;
		
	}

	
}


void led_light_off(void)
{
	LED1 = LED_LIGHT_OFF;
    LED2 = LED_LIGHT_OFF;	
	LED3 = LED_LIGHT_OFF;
	LED4 = LED_LIGHT_OFF;
}

/**
	欠压这里的时间需要和充电进行区分，时间频率上做修改
*/
void led_under_voltage_display(void)
{
	LED1 =! LED1;
}

/**
	时间由上层定义，这里只设计逻辑
*/
void led_erro_display(void)
{
	LED1 =! LED1;
	LED2 =! LED2;
	LED3 =! LED3;
	LED4 =! LED4;
}
