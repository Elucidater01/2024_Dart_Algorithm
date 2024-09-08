#include "monitor_task.h"

void StartMonitorTask(void const * argument)
{
	
  for(;;)
  {
	imu_sensor.heart_beat(&imu_sensor.work_state);
	imu_sensor.update(&imu_sensor);
	rc_sensor.heart_beat(&rc_sensor);
	
    osDelay(1);
  }
}
