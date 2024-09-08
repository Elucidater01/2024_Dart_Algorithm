#include "rm_motor.h"
#include "pid.h"

/*
	电机将CAN数据存到CANX_XXX_DATA数组（如CAN1_200_DATA）中，
	后由CANX_CMD_XXX函数发送
*/
void Motor_SendData(drv_can_t *motor, int16_t data)
{
    *(motor->CANx_XXX_DATA + motor->data_id) = data>>8;
	*(motor->CANx_XXX_DATA + motor->data_id + 1) = data;
}

/*
	电机单环pid控制速度
*/
void Motor_ToSpeed(rm_motor_t *motor, int16_t speed)
{
	motor->motor_pid.speed.measure = motor->info->speed;
	motor->motor_pid.speed.err = speed - motor->motor_pid.speed.measure;
	single_pid_ctrl(&motor->motor_pid.speed);
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/*
	电机双环pid控制角度
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
	//角度环
	single_pid_ctrl(&motor->motor_pid.angle);
	motor->motor_pid.speed.err = motor->motor_pid.angle.out - motor->info->speed;
	//速度环
	single_pid_ctrl(&motor->motor_pid.speed);
	//发送数据到数组
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/*
	堵转处理
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
