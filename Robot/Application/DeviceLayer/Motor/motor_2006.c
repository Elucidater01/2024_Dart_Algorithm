#include "motor_2006.h"


static void Motor_Init(motor_2006_t *motor);
static void UpdateMotorData(motor_2006_t *motor, uint8_t* data);
static void CheckMotorData(motor_2006_t *motor);
static void M2006_Heart_Beat(motor_2006_t *motor);

drv_can_t dial_motor_driver = {
	.hcan = &hcan2,
	.rx_id = RM2006_CAN_ID_203,
};

motor_2006_info_t dial_motor_info = {
	.offline_max_cnt = 50,
};
	
pid_t dial_pid = {
	.speed.Kp = DIAL_SP_KP,
	.speed.Ki = DIAL_SP_KI,
	.speed.Kd = DIAL_SP_KD,
	.speed.max_out = DIAL_SP_MAX_OUT,
	.speed.max_integral = DIAL_SP_MAX_INTEGRAL,
	.speed.max_iout = DIAL_SP_MAX_I_OUT,
	.angle.Kp = DIAL_AG_KP,
	.angle.Ki = DIAL_AG_KI,
	.angle.Kd = DIAL_AG_KD,
	.angle.max_out = DIAL_AG_MAX_OUT,
	.angle.max_integral = DIAL_AG_MAX_INTEGRAL,
	.angle.max_iout = DIAL_AG_MAX_I_OUT,
};

motor_2006_t dial_motor = {
	.info = &dial_motor_info,
	.driver = &dial_motor_driver,
	.pid = &dial_pid,
	.init = Motor_Init,
	.update = UpdateMotorData,
	.check = CheckMotorData,
	.heart_beat = M2006_Heart_Beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_DIAL,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void Motor_Init(motor_2006_t *motor)
{
		if (motor->info == NULL || motor == NULL || motor->driver == NULL)
		{
			motor->errno = DEV_INIT_ERR;
			return;
		}
		motor->info->angle = 0;
		motor->info->total_ecd = 0;
		motor->info->offline_cnt = 51;
		motor->errno = NONE_ERR;
		motor->work_state = DEV_OFFLINE;
		
		PID_Init(&motor->pid->speed);
		PID_Init(&motor->pid->angle);
}

static void UpdateMotorData(motor_2006_t *motor, uint8_t *data)
{
	  if (motor->info == NULL || motor == NULL)
		{
			motor->errno = DEV_INIT_ERR;
			return;
		}
		motor->info->ecd = (uint16_t)((data[0] << 8) | data[1]);
		motor->info->speed_rpm = (int16_t)((data[2] << 8) | data[3]);
		motor->info->given_current = (int16_t)((data[4] << 8) | data[5]);
}

static void CheckMotorData(motor_2006_t *motor)
{
		if (motor->info == NULL || motor == NULL)
		{
			motor->errno = DEV_INIT_ERR;
			return;
		}

		motor_2006_info_t *motor_info = motor->info;

		motor_info->delta_ecd = motor_info->ecd - motor_info->last_ecd;
		if (motor_info->delta_ecd > HALF_ECD_RANGE)
		{
				motor_info->delta_ecd -= ECD_RANGE;
		}
		else if (motor_info->delta_ecd < -HALF_ECD_RANGE)
		{
				motor_info->delta_ecd += ECD_RANGE;
		}
	
	  motor->info->last_ecd = motor->info->ecd;
		motor->info->total_ecd += motor->info->delta_ecd;
		motor->info->angle = (motor->info->total_ecd % 294876) * M2006_ECD_TO_ANGLE;
		motor->info->total_angle = motor->info->total_ecd * M2006_ECD_TO_ANGLE;
		
		motor->info->offline_cnt = 0;
}

static void M2006_Heart_Beat(motor_2006_t *motor)
{
	if (motor->info == NULL || motor == NULL)
	{
		motor->errno = DEV_INIT_ERR;
		return;
	}
	
	motor_2006_info_t *motor_info = motor->info;
	
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



