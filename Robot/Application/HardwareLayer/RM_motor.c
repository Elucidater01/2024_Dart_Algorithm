#include "rm_motor.h"
#include "pid.h"

/*
	�����CAN���ݴ浽CANX_XXX_DATA���飨��CAN1_200_DATA���У�
	����CANX_CMD_XXX��������
*/
void Motor_SendData(drv_can_t *motor, int16_t data)
{
    *(motor->CANx_XXX_DATA + motor->data_id) = data>>8;
	*(motor->CANx_XXX_DATA + motor->data_id + 1) = data;
}

/*
	�������pid�����ٶ�
*/
void Motor_ToSpeed(rm_motor_t *motor, int16_t speed)
{
	motor->motor_pid.speed.measure = motor->info->speed;
	motor->motor_pid.speed.err = speed - motor->motor_pid.speed.measure;
	single_pid_ctrl(&motor->motor_pid.speed);
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/*
	���˫��pid���ƽǶ�
*/
void Motor_ToAngle(rm_motor_t *motor, int16_t angle)
{
	motor->motor_pid.angle.measure = motor->info->angle;
	motor->motor_pid.angle.err = angle - motor->motor_pid.angle.measure;

	if(motor->motor_pid.angle.err < 0)
	{
		motor->motor_pid.angle.err += 8192;
	}
	if(motor->motor_pid.angle.err > 4096)
	{
		motor->motor_pid.angle.err = motor->motor_pid.angle.err - 8192;
	}
	//�ǶȻ�
	single_pid_ctrl(&motor->motor_pid.angle);
	motor->motor_pid.speed.err = motor->motor_pid.angle.out - motor->info->speed;
	//�ٶȻ�
	single_pid_ctrl(&motor->motor_pid.speed);
	//�������ݵ�����
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/*
	��ת����
*/
void Motor_DetectStuck(rm_motor_t motor)
{
	if(motor.info->speed == 0)
	{
		motor.cnt++;
		if(motor.flag == 0)
		{
			Motor_SendData(motor.driver, -500);
			//osDelay(200);
			motor.cnt = 0;
			motor.flag = 1;
		}
		else if(motor.flag == 1)
		{
			Motor_SendData(motor.driver, 1000);
			//osDelay(200);
			motor.cnt = 0;
			motor.flag = 0;
		}
	}			
}

void rm_motor_init(rm_motor_t *motor, motor_pid_t motor_pid)
{ 
	
    motor->motor_pid = motor_pid;

}

void rm_motor_heart_beat(rm_motor_t *motor)
{
    rm_motor_info_t *motor_info = motor->info;
    
    motor_info->offline_cnt++;
    if(motor_info->offline_cnt > motor_info->offline_max_cnt) {
        motor_info->offline_cnt = motor_info->offline_max_cnt;
        motor->work_state = DEV_OFFLINE;
    }
    else {
        if(motor->work_state == DEV_OFFLINE)
            motor->work_state = DEV_ONLINE;
    }
}
