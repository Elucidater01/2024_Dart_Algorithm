#include "led_task.h"

void StartLedTask(void const * argument)
{

  for(;;)
  {

	  /*OLED_ShowString(1, 1, "FR");
	  OLED_ShowSignedNum(1, 4, Force[0], 5);
	  OLED_ShowString(1, 11, "g");
	  
	  OLED_ShowString(2, 1, "FL");
	  OLED_ShowSignedNum(2, 4, Force[1], 5);
	  OLED_ShowString(2, 11, "g");*/
	  
	  if(judge_sensor.work_state == DEV_ONLINE)
	  {
		  HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_SET);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_RESET);
	  }
		if(motor_list_workstate())
		{
			HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_RESET);
		}
      osDelay(500);
  }
}
