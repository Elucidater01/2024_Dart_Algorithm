/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SPI2_Init(void)
{
	HAL_SPI_Init(&hspi2);
	HAL_GPIO_WritePin(BMI_CS_GPIO_Port, BMI_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
}

//void SPI1_Init(void)
//{
//	HAL_SPI_Init(&hspi1);
//	HAL_GPIO_WritePin(EX_BMI_CS_GPIO_Port, EX_BMI_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(1);
//}

