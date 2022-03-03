#include "iic.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = 2;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
	hi2c1.Instance->CR1|=0x08;
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}






/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB9     ------> I2C1_SDA
    PB6     ------> I2C1_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */
	
  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PA11 [PA9]     ------> I2C2_SCL
    PA12 [PA10]     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }

}

///**
//* @brief I2C MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hi2c: I2C handle pointer
//* @retval None
//*/
//void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
//{
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspDeInit 0 */

//  /* USER CODE END I2C1_MspDeInit 0 */
//    /* Peripheral clock disable */
////    __HAL_RCC_I2C1_CLK_DISABLE();
//	__HAL_RCC_I2C1_CLK_ENABLE();
//    /**I2C1 GPIO Configuration
//    PB9     ------> I2C1_SDA
//    PB6     ------> I2C1_SCL
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

//    /* I2C1 interrupt DeInit */
//    HAL_NVIC_EnableIRQ(I2C1_IRQn);
//  /* USER CODE BEGIN I2C1_MspDeInit 1 */

//  /* USER CODE END I2C1_MspDeInit 1 */
//  }
//  else if(hi2c->Instance==I2C2)
//  {
//  /* USER CODE BEGIN I2C2_MspDeInit 0 */

//  /* USER CODE END I2C2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C2_CLK_DISABLE();

//    /**I2C2 GPIO Configuration
//    PA11 [PA9]     ------> I2C2_SCL
//    PA12 [PA10]     ------> I2C2_SDA
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);

//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

//    /* I2C2 interrupt DeInit */
//    HAL_NVIC_DisableIRQ(I2C2_IRQn);
//  /* USER CODE BEGIN I2C2_MspDeInit 1 */

//  /* USER CODE END I2C2_MspDeInit 1 */
//  }

//}
u8 au8Rx_Buffer[40] = {0};
__IO u32 u32ISR1_reg = 0;
u8 u8Rx_Data_len = 0; 
void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hi2c1);

	
	u32ISR1_reg = hi2c1.Instance->ISR; 
	
	if(u32ISR1_reg & I2C_FLAG_ADDR )
	{
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_ADDR);
	    hi2c1.Instance->CR1 |= 0x08;	
		u8Rx_Data_len = 0;
	}
	
	if(u32ISR1_reg & I2C_FLAG_RXNE)
	{
		au8Rx_Buffer[u8Rx_Data_len++] = hi2c1.Instance->RXDR;
		//__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_RXNE);
		//u32ISR1_reg = 0;
	}
	
	if(u32ISR1_reg & I2C_FLAG_STOPF)
	{
		//u32ISR1_reg = 0;
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_STOPF);
	}

}

/**
  * @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23.
  */
void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  /* USER CODE END I2C1_IRQn 0 */
  if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c1);
  } else {
	I2C1_EV_IRQHandler();
  }
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}


u8 u8transfer = 0;

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	HAL_I2C_Slave_Receive_IT(&hi2c1,&u8transfer, 1); 
}
/**
  * @brief This function handles I2C2 global interrupt.
  */
void I2C2_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_IRQn 0 */

  /* USER CODE END I2C2_IRQn 0 */
  if (hi2c2.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c2);
  } else {
    HAL_I2C_EV_IRQHandler(&hi2c2);
  }
  /* USER CODE BEGIN I2C2_IRQn 1 */

  /* USER CODE END I2C2_IRQn 1 */
}


