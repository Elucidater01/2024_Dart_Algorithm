/**
 * @file        driver.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-September-2020
 * @brief       Drivers' Manager.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "driver.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void DRIVER_Init(void)
{
	USART1_Init();
	USART2_Init();
	USART3_Init();
	USART5_Init();
	USART6_Init();
	TIM1_Init();
	TIM4_Init();
	CAN_Filter_Init();
}
