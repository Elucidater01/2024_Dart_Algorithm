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
#include "cmsis_os.h"
#include "OLED.H"
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
osThreadId BlueHandle;
osThreadId OrangeHandle;
osThreadId GreenHandle;
osThreadId RedHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartBlueTask(void const * argument);
void StartOrangeTask(void const * argument);
void StartGreenTask(void const * argument);
void StartRedTask(void const * argument);

/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */
  OLED_Init();
//	OLED_ShowString(1, 1, "X");
//	OLED_ShowChar(1, 7, 'Y');
//	OLED_ShowChar(2, 1, 'Z');
//	OLED_ShowString(3, 1, "e");
	  
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Blue */
  osThreadDef(Blue, StartBlueTask, osPriorityNormal, 0, 128);
  BlueHandle = osThreadCreate(osThread(Blue), NULL);

  /* definition and creation of Orange */
  osThreadDef(Orange, StartOrangeTask, osPriorityBelowNormal, 0, 128);
  OrangeHandle = osThreadCreate(osThread(Orange), NULL);

  /* definition and creation of Green */
  osThreadDef(Green, StartGreenTask, osPriorityLow, 0, 128);
  GreenHandle = osThreadCreate(osThread(Green), NULL);

  /* definition and creation of Red */
  osThreadDef(Red, StartRedTask, osPriorityIdle, 0, 128);
  RedHandle = osThreadCreate(osThread(Red), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  HAL_GPIO_WritePin(GPIOC, Blue_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, Orange_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, Green_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_SET);
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
    /* USER CODE END WHILE */
	
	  HAL_Delay(1);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Blue_Pin|Orange_Pin|Red_Pin|Green_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Blue_Pin Orange_Pin Red_Pin Green_Pin */
  GPIO_InitStruct.Pin = Blue_Pin|Orange_Pin|Red_Pin|Green_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartBlueTask */
/**
  * @brief  Function implementing the Blue thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBlueTask */
void StartBlueTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	for(;;)
	{
		HAL_GPIO_WritePin(GPIOC, Blue_Pin, GPIO_PIN_RESET);
		osDelay(500);
        HAL_GPIO_WritePin(GPIOC, Blue_Pin, GPIO_PIN_SET);
        osDelay(1500);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartOrangeTask */
/**
* @brief Function implementing the Orange thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOrangeTask */
void StartOrangeTask(void const * argument)
{
  /* USER CODE BEGIN StartOrangeTask */
  /* Infinite loop */
	for(;;)
	{
        osDelay(500);
        HAL_GPIO_WritePin(GPIOC, Orange_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(GPIOC, Orange_Pin, GPIO_PIN_SET);
        osDelay(1000);
	}
  /* USER CODE END StartOrangeTask */
}

/* USER CODE BEGIN Header_StartGreenTask */
/**
* @brief Function implementing the Green thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGreenTask */
void StartGreenTask(void const * argument)
{
  /* USER CODE BEGIN StartGreenTask */
  /* Infinite loop */
	for(;;)
    {
	    osDelay(1000);
        HAL_GPIO_WritePin(GPIOC, Green_Pin, GPIO_PIN_RESET);
		osDelay(500);
        HAL_GPIO_WritePin(GPIOC, Green_Pin, GPIO_PIN_SET);
        osDelay(500);
   }
  /* USER CODE END StartGreenTask */
}

/* USER CODE BEGIN Header_StartRedTask */
/**
* @brief Function implementing the Red thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRedTask */
void StartRedTask(void const * argument)
{
  /* USER CODE BEGIN StartRedTask */
  /* Infinite loop */
    for(;;)
    {
        osDelay(1500);
        HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(GPIOC, Red_Pin, GPIO_PIN_SET);
    }
  /* USER CODE END StartRedTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
