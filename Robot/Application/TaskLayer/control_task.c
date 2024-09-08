#include "control_task.h"
#include "rp_user_config.h"
#include "motor.h"



void StartControlTask(void const * argument)
{
		rm_motor_init(&rm_motor[DIAL], motor_pid[DIAL]);
  for(;;)
  { 
		/*Motor_ToSpeed(&rm_motor[DIAL], 0);
	    Motor_ToSpeed(&rm_motor[CHAS_LF], 1000);
	    
	  
	    CAN1_CMD_200();
	    CAN2_CMD_200();
	    CAN1_CMD_1FF();
	    CAN2_CMD_1FF();*/
	 Magazine_Open();
		osDelay(1);
  }
}
