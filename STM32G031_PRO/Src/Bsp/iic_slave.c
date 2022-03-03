#include "iic_slave.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */ 
u8 au8rx_buffer[1] = {0x00};

uint8_t I2C_REG_TEST[I2C_REG_TEST_SIZE];
//I2C接收中断用到变量
//
uint8_t I2CFirstByteFlag = 0;  //接收第一个字节数据标志
uint8_t I2CAddressOffset = 0;  //接收主机地址
uint8_t i2c_regtemp = 0;
uint8_t I2C_DEVICE_REGADDRESS = 0;
uint8_t I2C_DEVICE_REGOFFSET = 0;



void MX_I2C1_Init(void)
{
  uint32_t i2c_temp = 0;
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
	
  #if I2C_NOSTRETCH_EN

  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;	
  #else
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  #endif
	
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
    __I2C1_CLK_ENABLE();
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
	hi2c1.Instance->CR2 &= ~I2C_CR2_NACK;
 	i2c_temp = 0;
	i2c_temp = (I2C_CR1_ERRIE/*!< Error interrupts enable */  |\
	            I2C_CR1_STOPIE/*!< STOP detection Interrupt enable */ |\
	            I2C_CR1_NACKIE/*!< Not acknowledge received Interrupt enable */ |\
	            I2C_CR1_ADDRIE/*!< Address match Interrupt enable (slave only) */ |\
	            I2C_CR1_RXIE/*!< RX Interrupt enable */ |\
	            I2C_CR1_TXIE/*!< TX Interrupt enable */);
  
	hi2c1.Instance->CR1 |= i2c_temp;
  
	//HAL_I2C_Slave_Receive_IT(&hi2c1,au8rx_buffer, 1);  
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
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
//    __HAL_RCC_I2C1_CLK_DISABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB9     ------> I2C1_SDA
    PB6     ------> I2C1_SCL
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    /* I2C1 interrupt DeInit */
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PA11 [PA9]     ------> I2C2_SCL
    PA12 [PA10]     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

    /* I2C2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I2C2_IRQn);
  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }

}


//void I2C1_Handler(void)
//{
//	u32ISR1_reg.u32val = hi2c1.Instance->ISR; 
//	if(u32ISR1_reg.u32val & I2C_FLAG_ADDR)
//	{
//		hi2c1.Instance->ICR |= 0x08;
//			
//		if(u32ISR1_reg.tIIC_ISR_Register.DIR)
//		{
//			u32ISR1_reg.tIIC_ISR_Register.TXE = 1;
//		}
//	}

//	if(hi2c1.Instance->ISR & I2C_FLAG_TXIS)
//	{
//		hi2c1.Instance->TXDR = 0x0a;	
//	}

//}

volatile u_IIC_ISR_Register u32ISR1_reg;
u32 u32I2C_DeviceFlag = 0;

void I2C1_EV_IRQHandler(void)
{
	//HAL_I2C_EV_IRQHandler(&hi2c1);

	
	u32ISR1_reg.u32val = hi2c1.Instance->ISR; 
	
	if((u32ISR1_reg.u32val & I2C_ISR_ADDR) != 0)
	{
		#if I2C_NOSTRETCH_EN
		{
			//Slave into transmitter mode, host read data
			if((hi2c1.Instance->ISR & I2C_ISR_DIR) != 0)
			{
				hi2c1.Instance->ISR |= I2C_ISR_TXE;
				u32I2C_DeviceFlag |= I2C_DEVICE_FLAG_DIR_SEND;
			}
			else // Slave into receive data from host
			{
				u32I2C_DeviceFlag &= ~I2C_DEVICE_FLAG_DIR_SEND;
			}
		
		}
		#else
		{
			if ((hi2c1.Instance->ISR & I2C_ISR_DIR) != 0)  /*!< Slave enter transmitter mode, Host read data */
			{
				u32I2C_DeviceFlag |= I2C_DEVICE_FLAG_DIR_SEND;
				hi2c1.Instance->ISR |= I2C_ISR_TXE;  /*!< set TXE bit, I2C_TXDR empty */
				//
				//根据不同寄存器地址来填充data1
				//
				if (I2C_DEVICE_REGADDRESS == I2C_REG_TEST_ADD)
				{
					if (I2C_DEVICE_REGOFFSET < I2C_REG_TEST_SIZE)
					{
						I2C_DEVICE->TXDR = I2C_REG_TEST[I2C_DEVICE_REGOFFSET];
						I2C_DEVICE_REGOFFSET++;
					}
					else
					{
						I2C_DEVICE->TXDR = I2C_DEVICE_DR_NULL_S;
					}
				}
				else
				{
					I2C_DEVICE->TXDR = I2C_DEVICE_DR_NULL_R;
				}
			}
			else  //Receiver mode
			{
				i2c_dev.I2C_DeviceFlag &= ~I2C_DEVICE_FLAG_DIR_SEND;
			}
		}
		#endif
//		u8Rx_Data_len = 0;
//		hi2c1.Instance->ISR |= 0x01;
//	    hi2c1.Instance->ICR |= 0x08;			
	    __HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_ADDR);
	}
	
	
	if((hi2c1.Instance->ISR & I2C_FLAG_TXIS) != 0)
	{

		if (I2C_DEVICE_REGADDRESS == I2C_REG_TEST_ADD)  //read and write
		{
			if(I2C_DEVICE_REGOFFSET < I2C_REG_TEST_SIZE)
			{
				hi2c1.Instance->TXDR = I2C_REG_TEST[I2C_DEVICE_REGOFFSET];
				I2C_DEVICE_REGOFFSET++;
				if(I2C_DEVICE_REGOFFSET >= I2C_REG_TEST_SIZE)
				{
					I2C_DEVICE_REGOFFSET = 0;
				}
			}
			else
			{
				hi2c1.Instance->TXDR = I2C_DEVICE_DR_NULL_S;
			}
		}
		else
		{
			hi2c1.Instance->TXDR = I2C_DEVICE_DR_NULL_R;
		}
		
//		if(u32ISR1_reg.tIIC_ISR_Register.DIR)
//		{
//			hi2c1.Instance->TXDR = 0x0a;		
//		}
//		else
//		{
//			
//		}		
	}	
	
	if(hi2c1.Instance->ISR & I2C_FLAG_RXNE)
	{
		if(I2CFirstByteFlag != 0)
		{
			if (I2C_DEVICE_REGADDRESS == I2C_REG_TEST_ADD)  //read and write
			{
				if(I2C_DEVICE_REGOFFSET < I2C_REG_TEST_SIZE)
				{
					I2C_REG_TEST[I2C_DEVICE_REGOFFSET] = hi2c1.Instance->RXDR;
					I2C_DEVICE_REGOFFSET++;
					if(I2C_DEVICE_REGOFFSET >= I2C_REG_TEST_SIZE)
					{
						I2C_DEVICE_REGOFFSET = 0;
					}
				}
				else
				{
					i2c_regtemp = hi2c1.Instance->RXDR; 
				}
			}
			else
			{
				i2c_regtemp = hi2c1.Instance->RXDR; 
				i2c_regtemp = 0;
			}
		}
		else
		{
			I2C_DEVICE_REGADDRESS = hi2c1.Instance->RXDR;

			I2CFirstByteFlag++;
		}
		//au8Rx1_Buffer[u8Rx_Data_len++] = hi2c1.Instance->RXDR;
		//__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_RXNE);
		//u32ISR1_reg = 0;
	}

	if ((hi2c1.Instance->ISR & I2C_ISR_NACKF) != 0)
	{
		__HAL_I2C_CLEAR_FLAG(&hi2c1, I2C_ISR_NACKF);
		if (I2CFirstByteFlag != 0)
		{
			I2CFirstByteFlag = 0;
		}
		u32I2C_DeviceFlag |= I2C_DEVICE_FLAG_NACK;
	}
	
	
	if(hi2c1.Instance->ISR & I2C_FLAG_STOPF)
	{
		if((u32I2C_DeviceFlag & I2C_DEVICE_FLAG_DIR_SEND) != 0)
		{
			#if I2C_NOSTRETCH_EN
			
			#else
			hi2c1.Instance->ISR |= I2C_ISR_TXE;
			hi2c1.Instance->ISR |= I2C_ISR_TXIS;
			#endif
			
			u32I2C_DeviceFlag &= ~I2C_DEVICE_FLAG_DIR_SEND;
			u32I2C_DeviceFlag |= I2C_DEVICE_FLAG_READ;
		}
		else
		{
			u32I2C_DeviceFlag |= I2C_DEVICE_FLAG_WRITE;
		}
		
		if(I2CFirstByteFlag != 0)
		{
			I2CFirstByteFlag = 0;
		}
		
//		u32ISR1_reg.u32val = 0;
//		hi2c1.Instance->ICR |= 0x10;
//		I2C1->CR1 |= 0x01;
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ISR_STOPF);
	}

}

void I2C1_ER_IRQHandler(void)
{
	if((hi2c1.Instance->ISR & I2C_ISR_BERR) != 0)
	{
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ICR_BERRCF);
	}
	
	if((hi2c1.Instance->ISR & I2C_ISR_ARLO) != 0)
	{
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ICR_ARLOCF);		
	}
	
	if((hi2c1.Instance->ISR & I2C_FLAG_OVR) != 0)
	{
		__HAL_I2C_CLEAR_FLAG(&hi2c1,I2C_ICR_OVRCF);	
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
	I2C1_ER_IRQHandler();
  } else {
	HAL_I2C_EV_IRQHandler(&hi2c1);
	I2C1_EV_IRQHandler();
  }
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
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


void IIC_DeInit(void)
{	
	HAL_I2C_DeInit(&hi2c1);	
	__HAL_RCC_I2C1_CLK_ENABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
	
	HAL_I2C_DeInit(&hi2c2);	
    __HAL_RCC_I2C2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
    HAL_NVIC_DisableIRQ(I2C2_IRQn);	
}



//void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
//{

//}

//u8 au8tx_buffer[1] = {0x01};
//void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
//{
//	HAL_I2C_Slave_Transmit_IT(&hi2c1,au8tx_buffer, 1);
//}
