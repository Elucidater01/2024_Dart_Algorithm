/* Includes ------------------------------------------------------------------*/
#include "drv_tim.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*舵机角度取值范围：val_open 50到250对应舵机0到300度*/
uint16_t val_open = 165;//100;
uint16_t val_clse = 90;//190;
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
void Trigger_Open(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_open);
}

void Trigger_Close(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_clse);
}

void Trigger_Sleep(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, val_slep);
}
