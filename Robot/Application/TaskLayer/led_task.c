#include "led_task.h"

void StartLedTask(void const * argument)
{
	//static	uint16_t i = 0;
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
  for(;;)
  {
		
    osDelay(1);
  }
}
