#include "timer.h"
#include "gpio.h"



volatile u32_tim_dly_t g_Tim0Period50ms = 0UL;
TIM_HandleTypeDef htim3;
/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
  
void MX_TIM3_Init(void)
{
	//不知道是多少时间的定时器，需要测试一下  50ms
  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 159;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END TIM3_Init 2 */

}


/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }

}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }

}


/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */

  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
  g_Tim0Period50ms ++;
  /* USER CODE END TIM3_IRQn 1 */
}


void TIM_DeInit(void)
{
	HAL_TIM_Base_DeInit(&htim3);
	
	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_RCC_TIM3_CLK_DISABLE();	
	HAL_NVIC_DisableIRQ(TIM3_IRQn);
	
}

/**
@brief  TIM0_Delay()
@param	

*/
en_result_t TMR0_Delay(bool judgement, u32_tim_dly_t *p_dly, uint32_t timecnt)
{
	if(false == (judgement) || (NULL == p_dly))
	{
		if(NULL != p_dly)
		{
			*p_dly = g_Tim0Period50ms;
			return ErrorInvalidParameter;
		}
	}
	
	if((g_Tim0Period50ms - *p_dly) >= timecnt)
	{
		*p_dly = g_Tim0Period50ms;
		return Ok;
	}
	
	return OperationInProgress;
}



/**
@brief 
@param
*/

u32_tim_dly_t TMR0_ResetCount(void)
{
	return g_Tim0Period50ms;
}

