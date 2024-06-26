/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "usart.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PROCESS_C02(uint8_t c);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t r;
//C02 SENSOR STATE
enum {
	CO2_STATE_BYTE_0 = 0,
	CO2_STATE_BYTE_1 = 1,
	CO2_STATE_BYTE_2 = 2,
	CO2_STATE_BYTE_3 = 3,
	CO2_STATE_BYTE_4 = 4,
	CO2_STATE_BYTE_5 = 5,
	CO2_STATE_BYTE_6 = 6,
	CO2_STATE_BYTE_7 = 7,
	CO2_STATE_BYTE_8 = 8,
	CO2_STATE_BYTE_9 = 9,
	CO2_STATE_BYTE_10 = 10,
	CO2_STATE_BYTE_11 = 11,
	CO2_STATE_BYTE_12 = 12,
	CO2_STATE_BYTE_13 = 13,
	CO2_STATE_BYTE_14 = 14,
	CO2_STATE_BYTE_15 = 15
};

//C02 SENSOR_CURRENT_STATE
uint8_t current_c02_state = 0;
uint8_t C02_HIGH_BYTE = 0;
uint8_t C02_LOW_BYTE = 0;

uint32_t PPM_VALUE;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart2, &r, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		HAL_Delay(100);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
		PROCESS_C02(r);
}
//PROCESS CO2 SENSOR DATA
void PROCESS_C02(uint8_t c) {
	switch (current_c02_state) {
	case CO2_STATE_BYTE_0:
		if (c == 0x42) {
			current_c02_state = CO2_STATE_BYTE_1;
		} else {
			current_c02_state = CO2_STATE_BYTE_0;
		}
		break;
	case CO2_STATE_BYTE_1:
		if (c == 0x4D) {
			current_c02_state = CO2_STATE_BYTE_2;
		} else {
			current_c02_state = CO2_STATE_BYTE_0;
		}
		break;
	case CO2_STATE_BYTE_2:
		current_c02_state = CO2_STATE_BYTE_3;
		break;
	case CO2_STATE_BYTE_3:
		current_c02_state = CO2_STATE_BYTE_4;
		break;
	case CO2_STATE_BYTE_4:
		current_c02_state = CO2_STATE_BYTE_5;
		break;
	case CO2_STATE_BYTE_5:
		current_c02_state = CO2_STATE_BYTE_6;
		break;
	case CO2_STATE_BYTE_6:
		current_c02_state = CO2_STATE_BYTE_7;
		C02_HIGH_BYTE = c;
		break;
	case CO2_STATE_BYTE_7:
		current_c02_state = CO2_STATE_BYTE_8;
		C02_LOW_BYTE = c;
		PPM_VALUE = (C02_HIGH_BYTE*256) + C02_LOW_BYTE;
		break;
	case CO2_STATE_BYTE_8:
		current_c02_state = CO2_STATE_BYTE_9;
		break;
	case CO2_STATE_BYTE_9:
		current_c02_state = CO2_STATE_BYTE_10;
		break;
	case CO2_STATE_BYTE_10:
		current_c02_state = CO2_STATE_BYTE_11;
		break;
	case CO2_STATE_BYTE_11:
		current_c02_state = CO2_STATE_BYTE_12;
		break;
	case CO2_STATE_BYTE_12:
		current_c02_state = CO2_STATE_BYTE_13;
		break;
	case CO2_STATE_BYTE_13:
		current_c02_state = CO2_STATE_BYTE_14;
		break;
	case CO2_STATE_BYTE_14:
		current_c02_state = CO2_STATE_BYTE_0;
		break;
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
