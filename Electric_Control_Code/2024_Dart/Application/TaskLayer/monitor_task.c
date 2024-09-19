#include "monitor_task.h"

/**
  * @brief  软件复位
  * @param  None
  * @retval None
  */
void Soft_Reset()
{
	if(rc_sensor.info->thumbwheel.value > 50 && IF_RC_SW1_DOWN && IF_RC_SW2_DOWN)
	{
		CAN_SendAllZero();
		HAL_Delay(500);
		__set_FAULTMASK(1);
		NVIC_SystemReset();	
	}
}

void StartMonitorTask(void const * argument)
{
	
  for(;;)
  {
	
	motor_list_heart_beat();
	rc_sensor.heart_beat(&rc_sensor);
	judge_sensor.heart_beat(&judge_sensor);
	vision_sensor.heart_beat(&vision_sensor);
    Soft_Reset();
	if(rc_sensor.work_state == DEV_ONLINE)
	{
		CAN_SendAll();
	}
	else
	{
		Trigger_Sleep();
		CAN_SendAllZero();
	}
	
    osDelay(1);
  }
}
