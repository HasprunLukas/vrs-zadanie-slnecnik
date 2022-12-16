/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TIM_HandleTypeDef htim1;

//UART_HandleTYpeDef huart1;

ll_servo_t servo_1;
uint8_t RxBuff[12];
uint8_t RxByte;
uint8_t RxLen = 0;
uint8_t Mode = 1;
uint8_t EnginePosition = 0;

USART_TypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_TIM1_Init(void);
uint8_t Light_Scanner(uint8_t mode_of_scanning, uint8_t engine_position);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // LL_USART_EnableIT_RXNE(USART1);
  servo_1.slave_tim = TIM1;
  servo_1.channel = LL_TIM_CHANNEL_CH1;

  servo_1.angle_min = 1850;
  servo_1.angle_max = 8050;
  servoMotorInit(&servo_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  EnginePosition = Light_Scanner(Mode, EnginePosition); // Obtaining position for engine to be move to.

	  engine_position_set = EnginePosition; // Setting engine to required position.


	  Mode = 2; // Changing mode to 2 after initial global measuring.
	  LL_mDelay(60000); // Wait time between measurements 60 seconds.


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */
/*
void Execute_CMD(void)
{
	const char s[2] = " ";
	char *val[8];
	char *p = strtok((char*)RxBuff, s);
	uint8_t i = 0;
	while (p != NULL)
	{
		val[i++] = p;
		p = strtok(NULL, s);
	}

	if (strcmp(val[0], "ROT") == 0)
	{
		servo_t tmp = (strcmp(val[1], "S1") == 0) ? S1 : S2;
		Servo_SetAngle(&tmp, atoi(val[2]));
	}

	if (strcmp(val[0], "CAL") == 0)
	{
		servo_t tmp = (strcmp(val[1], "S1") == 0) ? S1 : S2;
		Servo_Calibrate(&tmp, atoi(val[2]));
	}
}
*/

//void MX_GPIO_Init(void)
//{
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//
//}

void USART_Rx_Callback(void)
{
	uint8_t pTemp = LL_USART_ReceiveData8(USART1);
	if (pTemp != 0xa)	// Newline
	{
		if (pTemp != 0xd)	// CR
		{
			RxBuff[RxLen] = pTemp;
			RxLen++;
		}
	}else{
		RxLen = 0;
		Execute_CMD();
		memset(RxBuff, '\0', sizeof(RxBuff));
	}
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 *//*
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}



/**
  * @brief Light intensity scanning Function
  * @param uint8_t mode_of_scanning (type of scanning procedure 1 = full scan, 2 = short scan for adjustment of position),
  *     	uint8_t engine_position	(current position of the engine in degrees)
  * @retval uint8_t position of strongest light intensity
  */

// engine_position_set is placeholder name for variable of engine input.
// measured_light is placeholder name for variable of sensor output as strength of measured light.
uint8_t Light_Scanner(uint8_t mode_of_scanning, uint8_t engine_position)
{

	uint16_t light_intensity = 0;
	uint8_t light_intensity_position = 0;

	if (mode_of_scanning == 1)
	{
		engine_position_set = 0; // Setting engine to position 0 degrees.
		LL_mDelay(3000);

		for (int degree = 0; degree <= 120; degree = degree + 5)
		{
			engine_position_set = degree;
			LL_mDelay(2000);

			if (light_intensity < measured_light) // Saving position with highest measured light intensity.
			{
				light_intensity = measured_light;
				light_intensity_position = degree;
			}
		}

		return light_intensity_position;
	}
	else if (mode_of_scanning == 2)
	{
		if (engine_position < 5) // Condition for engine to not move below set minimum 0 degrees.
		{
			engine_position_set = 0; // Measuring light intensity 5 degrees back minimum 0.
			LL_mDelay(2000);

			light_intensity = measured_light;
			light_intensity_position = 0;

			engine_position_set = engine_position + 5; // Measuring light intensity 5 degrees forward.
			LL_mDelay(4000);

			if (light_intensity < measured_light)
			{
				light_intensity = measured_light;
				light_intensity_position = engine_position + 5;
			}

			return light_intensity_position;
		}
		else if (engine_position > 115) // Condition for engine to not move above set maximum 120 degrees.
		{
			engine_position_set = engine_position - 5; // Measuring light intensity 5 degrees back.
			LL_mDelay(2000);

			light_intensity = measured_light;
			light_intensity_position = engine_position - 5;

			engine_position_set = 120; // Measuring light intensity 5 degrees forward maximum 120.
			LL_mDelay(4000);

			if (light_intensity < measured_light)
			{
				light_intensity = measured_light;
				light_intensity_position = 120;
			}

			return light_intensity_position;
		}
		else // Condition for engine to be able to move in between 0 - 120 degrees.
		{
			engine_position_set = engine_position - 5; // Measuring light intensity 5 degrees back.
			LL_mDelay(2000);

			light_intensity = measured_light;
			light_intensity_position = engine_position - 5;

			engine_position_set = engine_position + 5; // Measuring light intensity 5 degrees forward.
			LL_mDelay(4000);

			if (light_intensity < measured_light)
			{
				light_intensity = measured_light;
				light_intensity_position = engine_position + 5;
			}

			return light_intensity_position;
		}
	}
	else // If parameter mode_of_scanning haven't accepted values 1 or 2 position of engine stay unchanged.
	{
		return engine_position;
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

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
