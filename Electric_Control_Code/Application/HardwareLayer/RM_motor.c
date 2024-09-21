/**
  ******************************************************************************
  * @file    RM_motor.c
  * @brief   电机控制
  * @version 
  * @date    
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rm_motor.h"
#include "pid.h"
#include "rp_math.h"

/**
 * @brief  将电机CAN数据存到CANX_XXX_DATA数组（如CAN1_200_DATA）中，
 *		   后由CANX_CMD_XXX函数发送
 * @param  motor: 电机结构体
 * @param  data: 电机数据
 * @retval None
 */
void Motor_SendData(drv_can_t *motor, int16_t data)
{
    *(motor->CANx_XXX_DATA + motor->data_id) = data>>8;
	*(motor->CANx_XXX_DATA + motor->data_id + 1) = data;
}

/**
 * @brief  电机单环pid控制速度
 * @param  motor: 电机结构体
 * @param  speed: 速度目标值
 * @retval None
 */
void Motor_ToSpeed(rm_motor_t *motor, int16_t speed)
{
	motor->motor_pid.speed.target = speed;
	motor->motor_pid.speed.measure = motor->info->speed;
	motor->motor_pid.speed.err = speed - motor->motor_pid.speed.measure;
	single_pid_ctrl(&motor->motor_pid.speed);
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/**
 * @brief  电机双环pid控制角度
 * @param  motor: 电机结构体
 * @param  angle: 角度目标值
 * @retval None
 */
void Motor_ToAngle(rm_motor_t *motor, int16_t angle)
{
	angle = angle % 8192;
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

/**
 * @brief  电机乘减速比后轴承的角度
 * @param  motor: 电机结构体
 * @param  angle: 角度目标值
 * @retval None
 */
void Motor_ToAxleAngle(rm_motor_t *motor, int32_t angle)
{
	motor->motor_pid.angle.measure = motor->info->angle_sum;
	motor->motor_pid.angle.err = angle - motor->motor_pid.angle.measure;

	//角度环
	single_pid_ctrl(&motor->motor_pid.angle);
	motor->motor_pid.speed.err = motor->motor_pid.angle.out - motor->info->speed;
	//速度环
	single_pid_ctrl(&motor->motor_pid.speed);
	//发送数据到数组
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/**
 * @brief  电机检测是否堵转
 * @param  motor: 电机结构体
 * @retval 1: 堵转 0: 未堵转
 */
int8_t Motor_DetectStuck(rm_motor_t *motor)
{
	if(motor->info->speed == 0 && abs(motor->info->current) > 100)
	{
		motor->cnt++;
		if(motor->cnt > 10)
		{
			motor->cnt = 0;
			return 1;
		}
		return 0;
	}
	else
	{
		motor->cnt = 0;
		return 0;
	}
}
/**
 * @brief  堵转检测+处理
 * @param  motor: 电机结构体
 * @retval 无
 */
void Motor_HandleStuck(rm_motor_t *motor)
{
	if((motor->info->speed == 0 && abs(motor->info->current) > 1000)|| motor->cnt > 100)
	{
		motor->cnt++;
		if(motor->cnt > 100)
		{	
			/*卡住后反转*/
			Motor_ToSpeed(motor, 4000);
			if(motor->cnt > 300)
			{	
				motor->cnt = 0;
			}
		}
	}
	else
	{
		motor->cnt = 0;
	}
}

/**
 * @brief  电机初始化
 * @param  motor: 电机结构体
 * @retval 无
 */
void rm_motor_init(rm_motor_t *motor, motor_pid_t motor_pid)
{ 
	motor->work_state = DEV_OFFLINE;
    motor->motor_pid = motor_pid;
}

/**
 * @brief  电机心跳失联检测
 * @param  motor: 电机结构体
 * @retval 无
 */
void rm_motor_heart_beat(rm_motor_t *motor)
{
    rm_motor_info_t *motor_info = motor->info;
    motor_info->offline_cnt++;
    if(motor_info->offline_cnt > motor_info->offline_max_cnt) 
	{
        motor_info->offline_cnt = motor_info->offline_max_cnt;
        motor->work_state = DEV_OFFLINE;
    }
    else 
	{
        if(motor->work_state == DEV_OFFLINE)
            motor->work_state = DEV_ONLINE;
    }
}
