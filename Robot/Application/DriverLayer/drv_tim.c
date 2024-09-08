/* Includes ------------------------------------------------------------------*/
#include "drv_tim.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t val_open = 222;
uint16_t val_clse = 152;
uint16_t val_slep = 0;
/* Exported variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void TIM1_Init(void)
{
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void TIM4_Init(void)
{
	HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);
}

/* Servo functions */
void Magazine_Open(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_open);
}

void Magazine_Close(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_clse);
}

void Magazine_Sleep(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_slep);
}
