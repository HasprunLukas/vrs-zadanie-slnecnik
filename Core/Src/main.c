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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "vd6283tx.h"
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

uint8_t Mode = 1;
uint8_t EnginePosition = 0;
int data;
int angle = 0;

// USART_TypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MX_GPIO_Init(void);
void MX_TIM2_Init(void);
void servoRotateToPosition(int angle);
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    EnginePosition = Light_Scanner(Mode, EnginePosition); // Obtaining position for engine to be move to.
    servoRotateToPosition(EnginePosition); // Setting engine to required position.
    Mode = 2; // Changing mode to 2 after initial global measuring.
    LL_mDelay(15000); // Wait time between measurements 15 seconds.

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
  LL_SetSystemCoreClock(8000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */
/*
 */

/**
 * @brief Light intensity scanning Function
 * @param uint8_t mode_of_scanning (type of scanning procedure 1 = full scan, 2 = short scan for adjustment of position),
 *     	uint8_t engine_position	(current position of the engine in degrees)
 * @retval uint8_t position of strongest light intensity
 */

uint8_t Light_Scanner(uint8_t mode_of_scanning, uint8_t engine_position)
{

  int light_intensity = 0;
  uint8_t light_intensity_position = 0;
  int measured_light = 0;

  if (mode_of_scanning == 1)
  {
    angle = 0;
    for (int degree = 0; degree <= 180; degree = degree + 5)
    {
      servoRotateToPosition(degree);
	  vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

      if (light_intensity < measured_light) // Saving position with highest measured light intensity.
      {
        light_intensity = measured_light;
        light_intensity_position = degree;
        angle = degree;
      }
    }

    return light_intensity_position;
  }
  else if (mode_of_scanning == 2)
  {
    if (engine_position < 5) // Condition for engine to not move below set minimum 0 degrees.
    {
      angle = 0; // Measuring light intensity at 0 degrees
      servoRotateToPosition(angle);
      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

      light_intensity = measured_light;
      light_intensity_position = 0;

      angle = angle + 5; // Measuring light intensity at 5 degrees
      servoRotateToPosition(angle);
      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

	  if (light_intensity < measured_light)
	  {
		  light_intensity = measured_light;
		  light_intensity_position = angle;
	  }

      return light_intensity_position;
    }
    else if (angle > 180) // Condition for engine to not move above set maximum 180 degrees.
    {
      angle = 180; // Measuring light intensity at 180 degrees
      servoRotateToPosition(angle);

      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();
	  light_intensity = measured_light;
	  light_intensity_position = angle;

      angle = angle - 5; // Measuring light intensity at 175 degrees
      servoRotateToPosition(angle);
      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

      if (light_intensity < measured_light)
      {
        light_intensity = measured_light;
        light_intensity_position = angle;
      }

      return light_intensity_position;
    }
    else // Condition for engine to be able to move in between 0 - 180 degrees.
    {
      angle = angle - 5; // Measuring light intensity 5 degrees back.
      servoRotateToPosition(angle);
      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

      light_intensity = measured_light;
      light_intensity_position = angle;

      angle = angle + 5; // Measuring light intensity at original degrees
      servoRotateToPosition(angle);
      vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

      if (light_intensity < measured_light)
      {
        light_intensity = measured_light;
        light_intensity_position = angle;
      }

      angle = angle + 5; // Measuring light intensity 5 degrees forward.
      servoRotateToPosition(angle);
	  vd6283tx_init();
	  measured_light = vd6283tx_get_als_ch2();

	  if (light_intensity < measured_light)
	  {
		  light_intensity = measured_light;
		  light_intensity_position = angle;
	  }

      return light_intensity_position;
    }
  }
  else // If parameter mode_of_scanning haven't accepted values 1 or 2 position of engine stay unchanged.
  {
    return engine_position;
  }
}

/**
 * @brief Takes the requested angle value, and rotates the servo motor to the desired rotation. 
 * If the angle is below 0 then we rotate the servo motor to the 0 degre position. If the angle is 
 * above 180 degree, then  we set the servo motor rotation position to 180 degree, else the angle
 * value will be multiplied by 5.5555 and we will add 180 to the final value.
 * We received the multiplier value from this equation ((1180-180)/180)/servo motor macimum angle reach.
 * we add 180 to the final value, because the servo motor starts rotate from pulse value 180.
 * 
 * @param angle The requested angle in degrees.
 */
void servoRotateToPosition(int angle)
{
  if (angle < 0)
  {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 180);
  }
  else if (angle == 0)
  {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 180); // 0deg
  }
  else if (angle > 0 && angle <= 180)
  {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, angle * 5.5555 + 180);
  }
  else
  {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1180);
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
