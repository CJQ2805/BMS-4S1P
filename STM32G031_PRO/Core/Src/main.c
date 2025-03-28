/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /*APP��ַ��0x8005000 ��ʼ*/	
#if BOOT_STAT
  SCB->VTOR = FLASH_BASE | 0x5000; /* Vector Table Relocation in Internal FLASH. */ 
  //__enable_irq();	
#endif
  u32_tim_dly_t  tim_dly;
  static u8 x;
	
  APP_Init();	
	
  while (1)
  {
	  
	 APP_PollingTask();

	 if(Ok == TMR0_Delay(TRUE, &tim_dly, TMR0_DELAY_MS(2000)))
	 {
		x = !x;
		PC6Out(x);		 
		printf("CJQ2805 test \r\n"); 		
	 }
 
  }

}



/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
