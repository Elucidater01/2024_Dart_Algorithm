#include "control_task.h"
#include "rp_config.h"
#include "module.h"
int16_t test = 0;
float target1, measear1;
void StartControlTask(void const * argument)
{
	
  for(;;)
  {
    if(rc_sensor.work_state == DEV_ONLINE)
	{
		mode_select(&module);
		launcher_ctrl(&launcher);
		axis_ctrl(&axis);
//		WIND_TOANGLE(test);
//		target1 = rm_motor[4].motor_pid.speed.target;
//		measear1 = rm_motor[4].motor_pid.speed.measure;
	}
	
	osDelay(1);
  }
}
