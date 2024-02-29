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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NOTE_C4 262
#define NOTE_D4	293
#define NOTE_E4	329
#define NOTE_G4	391
#define NOTE_F4	349
#define NOTE_A4	440
#define NOTE_H4	493
#define NOTE_D5	587
#define NOTE_C5	523
#define NOTE_PAUSE 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void tone_set(int frequency);
static void tone_start_stop(int command);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct note
{
	int frequency;
	int value;
};

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
const struct note song1[]=
  {
		  {NOTE_D4,8},
		  {NOTE_D4,8},
		  {NOTE_E4,4},
		  {NOTE_D4,4},
		  {NOTE_G4,4},
		  {NOTE_F4,2},
		  {NOTE_D4,8},
		  {NOTE_D4,8},
		  {NOTE_E4,4},
		  {NOTE_D4,4},
		  {NOTE_A4,4},
		  {NOTE_G4,2},
		  {NOTE_D4,8},
		  {NOTE_D4,8},
		  {NOTE_D5,4},
		  {NOTE_H4,4},
		  {NOTE_G4,8},
		  {NOTE_G4,8},
		  {NOTE_F4,4},
		  {NOTE_E4,4},
		  {NOTE_C5,8},
		  {NOTE_C5,8},
		  {NOTE_H4,4},
		  {NOTE_G4,4},
		  {NOTE_A4,4},
		  {NOTE_G4,2},

  };

const struct note song2[]=
{
	{NOTE_C4,4},
        {NOTE_D4,4},
        {NOTE_E4,4},
        {NOTE_F4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_C4,4},
        {NOTE_D4,4},
        {NOTE_E4,4},
        {NOTE_F4,4},
        {NOTE_G4,2},
        {NOTE_G4,2},
        {NOTE_C4,4},
        {NOTE_D4,4},
        {NOTE_E4,4},
        {NOTE_F4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_F4,4},
        {NOTE_E4,4},
        {NOTE_D4,4},
        {NOTE_C4,2},
        {NOTE_C4,2},
};

const struct note song3[]=
{
        {NOTE_C4,4},
        {NOTE_D4,4},
        {NOTE_E4,4},
        {NOTE_F4,4},
        {NOTE_G4,2},
        {NOTE_G4,2},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_G4,2},
        {NOTE_PAUSE,2},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_G4,2},
        {NOTE_PAUSE,2},
        {NOTE_F4,4},
        {NOTE_F4,4},
        {NOTE_F4,4},
        {NOTE_F4,4},
        {NOTE_E4,2},
        {NOTE_E4,2},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_C4,2},

};

const struct note song4[]=
{
        {NOTE_C4,2},
        {NOTE_E4,2},
        {NOTE_G4,2},
        {NOTE_G4,4},
        {NOTE_F4,4},
        {NOTE_E4,4},
        {NOTE_D4,4},
        {NOTE_C4,2},
        {NOTE_G4,4},
        {NOTE_G4,4},
        {NOTE_E4,4},
        {NOTE_C4,4},
        {NOTE_A4,4},
        {NOTE_A4,4},
        {NOTE_G4,2},
        {NOTE_G4,4},
        {NOTE_F4,4},
        {NOTE_E4,4},
        {NOTE_D4,4},
        {NOTE_C4,2},

};

  int note_duration(int value)
  {
	  return 2400/value;
  };

  const size_t song1_len = sizeof(song1)/sizeof(song1[0]);
  const size_t song2_len = sizeof(song2)/sizeof(song2[0]);
  const size_t song3_len = sizeof(song3)/sizeof(song3[0]);
  const size_t song4_len = sizeof(song4)/sizeof(song4[0]);


int number=4;

void switch_songs(int number)
{
	switch(number)
	{
		case 1:
			for(int i=0;i<song1_len;i++)
			{
				tone_set(song1[i].frequency);
				tone_start_stop(0);
				HAL_Delay(note_duration(song1[i].value));
				tone_start_stop(1);
				HAL_Delay(50);
			}
			break;

		case 2:
                        for(int i=0;i<song2_len;i++)
                        {
                                tone_set(song2[i].frequency);
                                tone_start_stop(0);
                                HAL_Delay(note_duration(song2[i].value));
                                tone_start_stop(1);
                                HAL_Delay(50);
                        }
			break;

		case 3:
                        for(int i=0;i<song3_len;i++)
                        {
                                tone_set(song3[i].frequency);
                                tone_start_stop(0);
                                HAL_Delay(note_duration(song3[i].value));
                                tone_start_stop(1);
                                HAL_Delay(50);
                        }
			break;

		case 4:
                        for(int i=0;i<song4_len;i++)
                        {
                                tone_set(song4[i].frequency);
                                tone_start_stop(0);
                                HAL_Delay(note_duration(song4[i].value));
                                tone_start_stop(1);
                                HAL_Delay(50);
                        }
			break;
	}
}


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	 switch_songs(number);
	 HAL_Delay(1000);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/**
 * @brief Start or Stop the timer
 * @param command: if 0 it starts the timer, if 1 it stops the timer
 * @retval None
 */
static void tone_start_stop(int command)
{

	switch(command)
	{
		case 0:
			HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
			break;
		case 1:
			HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
			break;
	}
}
/**
 *@brief Change the tone
 *@pram frequency: set arr and ccr for new tone to be played
 *@retval None
 */

static void tone_set(int frequency)
{
	  __HAL_TIM_SET_COUNTER(&htim2, 0);
	  __HAL_TIM_SET_AUTORELOAD(&htim2, (32000000/frequency));
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (32000000/frequency)/2);
}





/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 109215;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 109215;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
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
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
