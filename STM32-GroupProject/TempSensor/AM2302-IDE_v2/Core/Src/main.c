/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/**
 * ready: new measurement can start
 * measuring: begins with the start signal and ends with the release after the 40th Bit
 * finished: the 40 Bits are buffered
 * processing: calculating the temperature and the humidity, tries to transmit the data to the slave
 * 				in case of being unable to transmit data (e.g. no slave available or sending no acknowledge):
 * 				ready-State
 * invalid: repeat the measurement, continue with the ready-state (e.g. in case of timeout, incorrect timings, etc.)
 * error: no transmission possible
 */
typedef enum {
	ready, measuring, finished, processing, invalid, error
} states_t;

/*
 * err1: no transmission to I2C slave possible
 * err2: no connection to the sensor
 * err3: dataset to often invalid
 */
typedef enum {
	err1, err2, err3
} errCode_t;

struct halfData {
	uint8_t firstByte;
	uint8_t secondByte;
};

//used to interpret one uint16_t as two uint8_t
typedef union {
	uint16_t Uint16_t_1x;
	struct halfData Uint8_t_2x;
} data_t;

typedef enum {
	humidity, temperature
} physicalQuantity_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define I2C_ADDRESS 0x20<<1 //I2C-slave address
#define DELAY 5000 //switching delay between humidity and temperature
#define I2C_TIMEOUT 80 //waiting time during I2C transmission

#define INVALID_LIMIT 5 //number of allowed "invalid" cases until an error occurs

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint64_t const uint32NumValues = 4294967296; //number of values in a uint32_t
uint32_t const unint32MaxValue = 4294967295; //the max value in a uint32_t

uint8_t *buf[50] = { 0 };
uint8_t bufIndex = 0;

uint8_t invalidCouter = 0;
states_t currentState = error;
errCode_t currentErrorCode = err1;

physicalQuantity_t nextPhysicalQuantity = humidity;

//blocking variables, storing a time stamp
uint32_t tick_blockMeasurement = unint32MaxValue - 3000; //Allows the measurement to start immediately without waiting 2 seconds
uint32_t tick_blockTransmission = unint32MaxValue - DELAY;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void resetVariables(void) {
	bufIndex = 0;
}

void statusReady(void) {
	currentState = measuring;
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//Send start signal: T_{be}
	HAL_NVIC_DisableIRQ(TIM7_IRQn); //disable: avoid unwanted interruptions
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); //disable: avoid unwanted interruptions
	GPIO_InitStruct.Pin = sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(sensor_GPIO_Port, &GPIO_InitStruct); //pulls down the level

	HAL_Delay(10);

	//release the wire: T_{go}
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
	__HAL_TIM_SET_COUNTER(&htim7, 0);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	GPIO_InitStruct.Pin = sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(sensor_GPIO_Port, &GPIO_InitStruct); //change to high impedance
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void statusFinished(void) {
	currentState = processing;

	HAL_StatusTypeDef transmitReturnValue;

	uint8_t sum = 0; //sum of all 4 data bytes: hum1+hum2+temp1+temp2
	uint8_t parity = 0;

	data_t dataset[2]; //includes the humidity([0]) and the temperature([1])

	//check for T_{reh}
	if (buf[1] >= 72 && buf[1] <= 82) {
	} else {
		currentState = invalid;
		return;
	}

	//Determine the bit value:

	//normal data-bits (bit 0 until bit 7)
	//runs from high index to low index, because the LSB in the buffer is actually the MSB
	for (int element = 41; element >= 34; --element) {
		uint8_t value = buf[element]; //current value
		//data-bits == 0:
		if (value >= 20 && value <= 30) {
			parity = parity & ~(1 << (41 - element));
		}
		//data-bits == 1:
		else if (value >= 68 && value <= 75) {
			parity = parity | 1 << (41 - element);
		} else {
			currentState = invalid;
			return;
		}
	}

	//normal data-bits (bit 8 until bit 23)
	for (int element = 33; element >= 18; --element) {
		uint8_t value = buf[element]; //current value
		//data-bits == 0:
		if (value >= 20 && value <= 30) {
			/*
			 * 											  element = 33 or bit 8
			 *                                                         |
			 * e.g.                                                    v
			 * element = 33, dataset= 0bxxxxxxxxxxxxxxxx0000000011111111xxxxxxxx
			 * But the first Bit in die second Byte should be 0.
			 *
			 *  dataset[temperature].Uint16_t_1x & ~(1 << (33 - element))
			 *                                             (33 - 33)
			 *                                      (1 <<      0)
			 *                                     ~0b0000000000000001
			 *                0b0000000011111111 &  0b1111111111111110
			 * result ==>                0b0000000011111110
			 *
			 *  the 0 is inserted
			 */
			dataset[temperature].Uint16_t_1x = dataset[temperature].Uint16_t_1x & ~(1 << (33 - element));
		}
		//data-bits == 1:
		else if (value >= 68 && value <= 75) {
			/*                             element = 18 or bit 23
			 *                                          |
			 * e.g.                                     v
			 * element = 18, dataset= 0bxxxxxxxxxxxxxxxx0000000011111111xxxxxxxx
			 * But the last Bit in die second Byte should be 1.
			 *
			 *  dataset[temperature].Uint16_t_1x | 1 << (33 - element)
			 *                                          (33 - 18)
			 *                                     (1 <<   15)
			 *                0b0000000011111111 & 0b1000000000000000
			 * result ==>                0b1000000011111111
			 *
			 *  the 1 is inserted
			 */
			dataset[temperature].Uint16_t_1x = dataset[temperature].Uint16_t_1x | 1 << (33 - element);
		} else {
			currentState = invalid;
			return;
		}
	}

	//normal data-bits (bit 24 until bit 39)
	for (int element = 17; element >= 2; --element) {
		uint8_t value = buf[element]; //current value
		//data-bits == 0:
		if (value >= 20 && value <= 30) {
			dataset[humidity].Uint16_t_1x = dataset[humidity].Uint16_t_1x & ~(1 << (17 - element));
		}
		//data-bits == 1:
		else if (value >= 68 && value <= 75) {
			dataset[humidity].Uint16_t_1x = dataset[humidity].Uint16_t_1x | 1 << (17 - element);
		} else {
			currentState = invalid;
			return;
		}
	}

	//validate the parity: {1st humidity byte}+{2nd humidity byte}+{1st temperature byte}+{2nd temperature byte} == parity byte
	sum = dataset[humidity].Uint8_t_2x.firstByte;
	sum += dataset[humidity].Uint8_t_2x.secondByte;
	sum += dataset[temperature].Uint8_t_2x.firstByte;
	sum += dataset[temperature].Uint8_t_2x.secondByte;
	if (parity != sum) {
		currentState = invalid;
		return;
	}

	dataset[humidity].Uint16_t_1x += 0b0100000000000000; //to distinguish from the temperature

	//block until switch delay expires, e.g. 5 seconds
	int64_t difference = HAL_GetTick() - tick_blockTransmission;
	while (difference < DELAY) {
		//check for a systick overflow
		if (difference < 0) {
			difference = uint32NumValues - difference;
		}
		HAL_Delay(100); //could be improved by putting the CPU to sleep and waking it up again
		difference = HAL_GetTick() - tick_blockTransmission;
	}
	tick_blockTransmission = HAL_GetTick();

	//transmit the "next physical quantity"
	transmitReturnValue = HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, &(dataset[nextPhysicalQuantity].Uint16_t_1x), 2, I2C_TIMEOUT);
	if (transmitReturnValue != HAL_OK) {
		currentState = error;
		currentErrorCode = err1;
		return;
	}

	//toggle between humidity and temperature
	if (nextPhysicalQuantity == humidity) {
		nextPhysicalQuantity = temperature;
	} else if (nextPhysicalQuantity == temperature) {
		nextPhysicalQuantity = humidity;
	}

	resetVariables();
	invalidCouter = 0; //when the CPU comes to this point, everything has worked -> set to zero
	currentState = ready;

}

void statusInvalid(void) {
	//signalizes the invalid state
	HAL_GPIO_WritePin(status_GPIO_Port, status_Pin, GPIO_PIN_SET);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	HAL_GPIO_WritePin(status_GPIO_Port, status_Pin, GPIO_PIN_SET);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	HAL_Delay(250);
	HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);

	++invalidCouter;
	uint16_t errorMask = 0b1100000000000000; // to distinguish between humidity, (negative) temperature and error

	if (invalidCouter >= INVALID_LIMIT) {
		errorMask += err3;

		//if too many invalids have occurred, send error 3 to the slave:
		if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, &(errorMask), 2, I2C_TIMEOUT) != HAL_OK) {
			//when the slave was not reachable:
			currentState = error;
			currentErrorCode = err1;
		}
	}

	resetVariables();
	currentState = ready; //try again
}

void statusError(void) {
	//signalizes the error 2 state
	HAL_GPIO_WritePin(status_GPIO_Port, status_Pin, GPIO_PIN_SET);

	uint16_t errorMask = 0xC000;

	if (currentErrorCode == err2) {
		errorMask += err2;

		if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, &(errorMask), 2, I2C_TIMEOUT) != HAL_OK) {
			currentState = error;
			currentErrorCode = err1;
		}

	} else {
		//signalizes the error 1 state
		HAL_Delay(500);
		HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);
	}
	HAL_Delay(3000);
	HAL_NVIC_SystemReset(); //restart the system
}

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
	MX_I2C1_Init();
	MX_TIM7_Init();
	/* USER CODE BEGIN 2 */

	//startUp:
	//check for a connection
	uint16_t test = 0xcafe;
	uint8_t check[] = { 0 };

	if (HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, &test, 2, HAL_MAX_DELAY) != HAL_OK) {
		currentState = error;
		currentErrorCode = err1;
	} else {
		if (HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS, check, 1, HAL_MAX_DELAY) != HAL_OK) {
			currentState = error;
			currentErrorCode = err1;
		} else {
			if (check[0] == 85)
				currentState = ready;
		}
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		//cycle through the four states:
		if (currentState == ready) {
			int64_t difference = HAL_GetTick() - tick_blockMeasurement;
			//check for a systick overflow:
			if (difference < 0) {
				difference = uint32NumValues - difference;
			}
			if (difference >= 3000) {
				//if more than 2 seconds have passed
				tick_blockMeasurement = HAL_GetTick();
				statusReady();
			}

		} else if (currentState == finished) {
			statusFinished();
		} else if (currentState == invalid) {
			statusInvalid();

		} else if (currentState == error) {
			statusError();
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 16;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}

	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00707CBB;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void) {

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 32; //the timer runs with 1MHz = count up every 1µs
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 200; //after 200µs owerflow -> timeout
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(status_GPIO_Port, status_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : sensor_Pin */
	GPIO_InitStruct.Pin = sensor_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(sensor_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : status_Pin */
	GPIO_InitStruct.Pin = status_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(status_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//the timer runs with 1MHz = count up every 1µs
	//after 200µs overflow -> timeout
	if (htim == &htim7 && currentState == measuring) {
		//if the CPU reaches this line, a timeout has occurred during the sensor transmission
		currentState = error;
		currentErrorCode = err2;

		HAL_TIM_Base_Stop_IT(&htim7);
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == sensor_Pin && currentState == measuring) {
		uint8_t currentTime = (uint8_t) __HAL_TIM_GET_COUNTER(&htim7); //uint8_t is sufficient, because TIM7 never exceeds the 200

		//check for rising edge, this is the start time for a high period
		if (HAL_GPIO_ReadPin(sensor_GPIO_Port, sensor_Pin) == GPIO_PIN_SET) {
			__HAL_TIM_SET_COUNTER(&htim7, 0); //set TIM7 to 0

		}
		//check for falling edge, this is the end time for a high period
		else if (HAL_GPIO_ReadPin(sensor_GPIO_Port, sensor_Pin) == GPIO_PIN_RESET) {
			buf[bufIndex] = currentTime; //read TIM7

			//a sigle data transmission consists of 41 "high periods"
			if (bufIndex < 41) {
				bufIndex++;

			} else if (bufIndex == 41) {
				//after the 41st "high period" the dataset is complete
				HAL_TIM_Base_Stop_IT(&htim7);
				currentState = finished;
			} else {
				//this could accually not happen, but in case there are more than 41 "high periods" -> invalid state
				HAL_TIM_Base_Stop_IT(&htim7);
				currentState = invalid;
			}
		}
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
		HAL_NVIC_SystemReset();
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
