/**
  ******************************************************************************
  * @file    RM_motor.c
  * @brief   �������
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
 * @brief  �����CAN���ݴ浽CANX_XXX_DATA���飨��CAN1_200_DATA���У�
 *		   ����CANX_CMD_XXX��������
 * @param  motor: ����ṹ��
 * @param  data: �������
 * @retval None
 */
void Motor_SendData(drv_can_t *motor, int16_t data)
{
    *(motor->CANx_XXX_DATA + motor->data_id) = data>>8;
	*(motor->CANx_XXX_DATA + motor->data_id + 1) = data;
}

/**
 * @brief  �������pid�����ٶ�
 * @param  motor: ����ṹ��
 * @param  speed: �ٶ�Ŀ��ֵ
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
 * @brief  ���˫��pid���ƽǶ�
 * @param  motor: ����ṹ��
 * @param  angle: �Ƕ�Ŀ��ֵ
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
	//�ǶȻ�
	single_pid_ctrl(&motor->motor_pid.angle);
	motor->motor_pid.speed.err = motor->motor_pid.angle.out - motor->info->speed;
	//�ٶȻ�
	single_pid_ctrl(&motor->motor_pid.speed);
	//�������ݵ�����
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/**
 * @brief  ����˼��ٱȺ���еĽǶ�
 * @param  motor: ����ṹ��
 * @param  angle: �Ƕ�Ŀ��ֵ
 * @retval None
 */
void Motor_ToAxleAngle(rm_motor_t *motor, int32_t angle)
{
	motor->motor_pid.angle.measure = motor->info->angle_sum;
	motor->motor_pid.angle.err = angle - motor->motor_pid.angle.measure;

	//�ǶȻ�
	single_pid_ctrl(&motor->motor_pid.angle);
	motor->motor_pid.speed.err = motor->motor_pid.angle.out - motor->info->speed;
	//�ٶȻ�
	single_pid_ctrl(&motor->motor_pid.speed);
	//�������ݵ�����
	Motor_SendData(motor->driver, motor->motor_pid.speed.out);
}

/**
 * @brief  �������Ƿ��ת
 * @param  motor: ����ṹ��
 * @retval 1: ��ת 0: δ��ת
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
 * @brief  ��ת���+����
 * @param  motor: ����ṹ��
 * @retval ��
 */
void Motor_HandleStuck(rm_motor_t *motor)
{
	if((motor->info->speed == 0 && abs(motor->info->current) > 1000)|| motor->cnt > 100)
	{
		motor->cnt++;
		if(motor->cnt > 100)
		{	
			/*��ס��ת*/
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
 * @brief  �����ʼ��
 * @param  motor: ����ṹ��
 * @retval ��
 */
void rm_motor_init(rm_motor_t *motor, motor_pid_t motor_pid)
{ 
	motor->work_state = DEV_OFFLINE;
    motor->motor_pid = motor_pid;
}

/**
 * @brief  �������ʧ�����
 * @param  motor: ����ṹ��
 * @retval ��
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
