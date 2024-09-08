
/* Includes ------------------------------------------------------------------*/
#include "chassis.h"
#include "power_limit.h"

#include "rp_math.h"
#include "arm_math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NOMAL_ROUND_SPEED 5000.f
#define RAPID_ROUND_SPEED 8000.f

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Chassis_Init(void);
static void Chassis_Ctrl(void);
static void Chassis_SelfProtect(void);

void Chassis_GetInfo(void);
void Chassis_GetRcInfo(void);
void Chassis_GetKeyInfo(void);
chassis_dir_e Chassis_GetDirection(void);
int16_t	Chassis_GetYawErr(void);
float	Chassis_GetYawRadErr(void);

void Chassis_Reset(void);
void Chassis_MotorCtrl(void);
void Chassis_SendOut(void);
void Chassis_Stop(void);

void Chassis_Test(void);

extern void Get_GimbalInfo(int16_t *get_info, int16_t data_offset);
/* Private variables ---------------------------------------------------------*/

int16_t chas_out[4];

chassis_dev_t		chas_dev = {
	.chas_motor[0] = &RM_motor[CHAS_1],
	.chas_motor[1] = &RM_motor[CHAS_2],
	.chas_motor[2] = &RM_motor[CHAS_3],
	.chas_motor[3] = &RM_motor[CHAS_4],
	.imu_sensor = &imu_sensor,
	.rc_sensor = &rc_sensor,
};

chassis_info_t	chas_info = {
	.remote_mode = RC,
};

chassis_conf_t	chas_conf = {
	.limit_speed = 8000.f,
	.chassis_follow_kp = 4.f,
	.gyro_round_speed = 5000.f,
	.key_z_offset = -10.f,
};
/* Exported variables --------------------------------------------------------*/
extern int16_t can1_send_buff[8];

chassis_t chassis = {
	.dev = &chas_dev,
	.info = &chas_info,
	.conf = &chas_conf,
	.init = Chassis_Init,
	.ctrl = Chassis_Ctrl,
	.self_protect = Chassis_SelfProtect,
};

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  底盘初始化
  * @param  
  * @retval 
  */
void Chassis_Init(void)
{
	
}

/**
  * @brief  底盘控制
  * @param  
  * @retval 
  */
void Chassis_Ctrl(void)
{
	Chassis_GetInfo();
	
	Chassis_Reset();
	
//	Chassis_Test();
	
	Chassis_MotorCtrl();
	
	Chassis_SendOut();
}


/**
  * @brief  底盘控制离线保护
  * @param  
  * @retval 
  */
void Chassis_SelfProtect(void)
{
	Chassis_Stop();
	Chassis_GetInfo();
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  底盘数据读取
  * @param  
  * @retval 
  */
void Chassis_GetInfo(void)
{
	static uint16_t hurt_time = 0, sign = 1.f;
	
	chassis.info->last_yaw_motor_angle_err = chassis.info->yaw_motor_angle_err;
	chassis.info->chassis_dir = Chassis_GetDirection();
	chassis.info->yaw_motor_angle_err = Chassis_GetYawErr();
	chassis.info->yaw_rad_err = Chassis_GetYawRadErr();
	
	for(uint8_t i = 0; i < 4; i++)
	{
		chassis.info->wheel_real[i] = RM_motor[i + 3].rx_info.speed;
	}
	
	chassis.info->measure_chas_x_speed = (float)(-RM_motor[CHAS_1].rx_info.speed - \
																			 RM_motor[CHAS_2].rx_info.speed + \
																			 RM_motor[CHAS_3].rx_info.speed + \
																			 RM_motor[CHAS_4].rx_info.speed) / 4.f;
	chassis.info->measure_chas_y_speed = (float)(-RM_motor[CHAS_1].rx_info.speed + \
																			 RM_motor[CHAS_2].rx_info.speed - \
																			 RM_motor[CHAS_3].rx_info.speed + \
																			 RM_motor[CHAS_4].rx_info.speed) / 4.f;
	chassis.info->measure_z_speed = (float)(-RM_motor[CHAS_1].rx_info.speed - \
																	 RM_motor[CHAS_2].rx_info.speed - \
																	 RM_motor[CHAS_3].rx_info.speed - \
																	 RM_motor[CHAS_4].rx_info.speed) / 4.f;
	
	chassis.info->measure_x_speed = chassis.info->measure_chas_x_speed * arm_cos_f32(chassis.info->yaw_rad_err) \
																- chassis.info->measure_chas_y_speed * arm_sin_f32(chassis.info->yaw_rad_err);
	chassis.info->measure_y_speed =-chassis.info->measure_chas_x_speed * arm_sin_f32(chassis.info->yaw_rad_err) \
																+ chassis.info->measure_chas_y_speed * arm_cos_f32(chassis.info->yaw_rad_err);
	
	chas_conf.limit_speed = 8000.f;
	chas_conf.gyro_round_speed = sign * NOMAL_ROUND_SPEED;
	if (status.speed_cmd == rapid_on)
	{
		chas_conf.limit_speed = 12000.f;
		chas_conf.gyro_round_speed = sign * RAPID_ROUND_SPEED;
	}
	
	if (status.lch_state.magz_state == magz_open)
	{
		chas_conf.limit_speed = 800.f;
	}
	
	/* 变速变向陀螺 */
//	switch (judge.info->robot_hurt.armor_id)
//	{
//		case 0:
//			if (judge.info->robot_hurt.hurt_type == 1)
//			{
//				hurt_time = HAL_GetTick();
//				chas_conf.gyro_round_speed = -abs(chas_conf.gyro_round_speed);
//			}
//			if (HAL_GetTick() - hurt_time >= 1000)
//			{
//				sign = -sign;
//				judge.info->robot_hurt.hurt_type = 0;
//			}
//			break;
//		case 1:
//			if (judge.info->robot_hurt.hurt_type == 1)
//			{
//				hurt_time = HAL_GetTick();
//				chas_conf.gyro_round_speed -= 3000.f;
//			}
//			if (HAL_GetTick() - hurt_time >= 1000)
//			{
//				judge.info->robot_hurt.hurt_type = 0;
//			}
//			break;
//		case 2:
//			if (judge.info->robot_hurt.hurt_type == 1)
//			{
//				hurt_time = HAL_GetTick();
//				chas_conf.gyro_round_speed += 3000.f;
//			}
//			if (HAL_GetTick() - hurt_time >= 1000)
//			{
//				judge.info->robot_hurt.hurt_type = 0;
//			}
//			break;
//		case 3:
//			if (judge.info->robot_hurt.hurt_type == 1)
//			{
//				hurt_time = HAL_GetTick();
//				chas_conf.gyro_round_speed = -abs(chas_conf.gyro_round_speed);
//			}
//			if (HAL_GetTick() - hurt_time >= 1000)
//			{
//				judge.info->robot_hurt.hurt_type = 0;
//			}
//			break;
//		default:
//			break;
//			
//	}
	
	// 电机数据更新
	if (chassis.info->chassis_lock == CHAS_UNLOCK)
	{
		if (module.remote_mode == RC)
		{
			chassis.info->remote_mode = RC;
			Chassis_GetRcInfo();
		}
		else if (module.remote_mode == KEY)
		{
			chassis.info->remote_mode = KEY;
			Chassis_GetKeyInfo();
		}
	}
	
	if (chassis.info->chassis_dir == CHASSIS_BACKWARD)
	{
		chassis.info->target_x_speed = -chassis.info->target_x_speed;
		chassis.info->target_y_speed = -chassis.info->target_y_speed;
		chassis.info->measure_x_speed = -chassis.info->measure_x_speed;
		chassis.info->measure_y_speed = -chassis.info->measure_y_speed;
		chassis.info->measure_chas_x_speed = -chassis.info->measure_chas_x_speed;
		chassis.info->measure_chas_y_speed = -chassis.info->measure_chas_y_speed;
	}
}

uint8_t err = 100;
float	k1 = 0.4f, k2 = 0.4f, k3 = 1.f, k4 = 1.f;
float kkp, kpset;
float ki = 0;
float ang_err, logerr;
//float max_speed, max_err;
void Chassis_GetRcInfo(void)
{
//	if (chassis.info->chassis_mode == CHASSIS_MODE_NORMAL)
	{
		if (module.mode == MODULE_MODE_GYRO)
		{
			float yaw_rad_err = 0.f;
			float	x_speed_temp, y_speed_temp;
				
			
			chassis.info->target_x_speed = (float)rc_sensor.info->ch3 * chas_conf.limit_speed / 660.0f;
			chassis.info->target_y_speed =-(float)rc_sensor.info->ch2 * chas_conf.limit_speed / 660.0f;
			
			/* 变kp */
			ang_err = k2 * abs(chassis.info->yaw_motor_angle_err) + k3;
			arm_vlog_f32(&ang_err, &logerr, 1);
			kkp = k1 * logerr;
			kpset = kkp;
			if (kkp < k4)
				kkp = k4;
			chas_pid.Kp = kkp;
			
			/* 积分限幅 */
			chas_pid.Ki = 0.f;
			if (abs(chassis.info->yaw_motor_angle_err) < err)
				chas_pid.Ki = ki;
			
			/* 死区处理 */
			chassis.info->target_z_speed = 0;
			if (abs(chassis.info->yaw_motor_angle_err) > 5)
				chassis.info->target_z_speed = PID_Plc_Calc(&chas_pid, (float)chassis.info->yaw_motor_angle_err, 0.f);
			
			x_speed_temp = chassis.info->target_x_speed;
			y_speed_temp = chassis.info->target_y_speed;
			yaw_rad_err = (float)chassis.info->yaw_motor_angle_err * GM6020_ECD_TO_ANGLE * ANGLE_TO_RAD;
			
			chassis.info->target_x_speed = x_speed_temp * arm_cos_f32(yaw_rad_err) \
																	 + y_speed_temp * arm_sin_f32(yaw_rad_err);
			chassis.info->target_y_speed =-x_speed_temp * arm_sin_f32(yaw_rad_err) \
																	 + y_speed_temp * arm_cos_f32(yaw_rad_err);
//			chassis.info->target_z_speed = 0;
			
			if (status.chas_cmd == gyro_on)
			{
				chassis.info->target_z_speed = chas_conf.gyro_round_speed;
			}
		}
		else if (module.mode == MODULE_MODE_MACHINE)
		{
			
			chassis.info->target_x_speed = (float)rc_sensor.info->ch3 * chas_conf.limit_speed / 660.0f;
			chassis.info->target_y_speed =-(float)rc_sensor.info->ch2 * chas_conf.limit_speed / 660.0f;
			chassis.info->target_z_speed =-(float)rc_sensor.info->ch0 * chas_conf.limit_speed / 660.0f;
		if (status.lch_state.magz_state == magz_open)
		{
			chassis.info->target_z_speed =-(float)rc_sensor.info->ch0 * chas_conf.limit_speed / 660.0f / 10.f;
		}
		}
	}
	
	/* 换挡加速 上坡效果不好 */
//	max_speed = max(max(abs(chassis.info->target_x_speed), abs(chassis.info->target_y_speed)), abs(chassis.info->target_z_speed));
//	max_err = max(max(abs(chassis.info->target_x_speed - chassis.info->measure_x_speed), abs(chassis.info->target_y_speed - chassis.info->measure_y_speed)), \
//						abs(chassis.info->target_z_speed - chassis.info->measure_z_speed));
//	if (max_err > (max_speed * 0.2f))
//	{
//		chassis.info->target_x_speed *= 0.4f;//(1.f- (max_err - max_speed * 0.4f));
//		chassis.info->target_y_speed *= 0.4f;//(1.f- (max_err - max_speed * 0.4f));
//		chassis.info->target_z_speed *= 0.4f;//(1.f- (max_err - max_speed * 0.4f));
//	}
//	else if (max_err > (max_speed * 0.4f))
//	{
//		chassis.info->target_x_speed *= 0.8f;//(1.f- (max_err - max_speed * 0.8f));
//		chassis.info->target_y_speed *= 0.8f;//(1.f- (max_err - max_speed * 0.8f));
//		chassis.info->target_z_speed *= 0.8f;//(1.f- (max_err - max_speed * 0.8f));
//	}
}	

void Chassis_GetKeyInfo(void)
{
	float front = 0.f, left = 0.f;
	front += (float)rc_sensor.info->W.cnt / (float)KEY_W_CNT_MAX * 660.f;
	front -= (float)rc_sensor.info->S.cnt / (float)KEY_S_CNT_MAX * 660.f;
	left  += (float)rc_sensor.info->A.cnt / (float)KEY_D_CNT_MAX * 660.f;
	left  -= (float)rc_sensor.info->D.cnt / (float)KEY_A_CNT_MAX * 660.f;
	
	if (module.mode == MODULE_MODE_GYRO) 	
	{
			float yaw_rad_err = 0.f;
			float	x_speed_temp, y_speed_temp;
		
			chassis.info->target_x_speed = front * chas_conf.limit_speed / 660.0f;
			chassis.info->target_y_speed = left  * chas_conf.limit_speed / 660.0f;
//			chassis.info->target_z_speed =-(float)chassis.info->yaw_motor_angle_err * chas_conf.chassis_follow_kp;
//			chassis.info->target_z_speed = 0;
//			if (abs(chassis.info->yaw_motor_angle_err) > err)
//				chassis.info->target_z_speed = PID_Plc_Calc(&chas_pid, (float)chassis.info->yaw_motor_angle_err, 0.f);
		
			/* 变kp */
			ang_err = k2 * abs(chassis.info->yaw_motor_angle_err) + k3;
			arm_vlog_f32(&ang_err, &logerr, 1);
			kkp = k1 * logerr;
			kpset = kkp;
			if (kkp < k4)
				kkp = k4;
			chas_pid.Kp = kkp;
			
			/* 积分限幅 */
			chas_pid.Ki = 0.f;
			if (abs(chassis.info->yaw_motor_angle_err) < err)
				chas_pid.Ki = ki;
			
			/* 死区处理 */
			chassis.info->target_z_speed = 0;
			if (abs(chassis.info->yaw_motor_angle_err) > 5)
				chassis.info->target_z_speed = PID_Plc_Calc(&chas_pid, (float)chassis.info->yaw_motor_angle_err, 0.f);
			
			x_speed_temp = chassis.info->target_x_speed;
			y_speed_temp = chassis.info->target_y_speed;
			yaw_rad_err = (float)chassis.info->yaw_motor_angle_err * GM6020_ECD_TO_ANGLE * ANGLE_TO_RAD;
			
			chassis.info->target_x_speed = x_speed_temp * arm_cos_f32(yaw_rad_err) \
																	 + y_speed_temp * arm_sin_f32(yaw_rad_err);
			chassis.info->target_y_speed =-x_speed_temp * arm_sin_f32(yaw_rad_err) \
																	 + y_speed_temp * arm_cos_f32(yaw_rad_err);
			
		
		if (status.chas_cmd == gyro_on)
		{
			chassis.info->target_z_speed = chas_conf.gyro_round_speed;
		}
	}
	else if (module.mode == MODULE_MODE_MACHINE)
	{
		
		chassis.info->target_x_speed = front * chas_conf.limit_speed / 660.0f;
		chassis.info->target_y_speed = left  * chas_conf.limit_speed / 660.0f;
		chassis.info->target_z_speed = rc_sensor.info->mouse_x * chas_conf.key_z_offset;
		if (status.lch_state.magz_state == magz_open)
		{
			chassis.info->target_z_speed = rc_sensor.info->mouse_x * chas_conf.key_z_offset / 10.f;
		}
	}
}

chassis_dir_e Chassis_GetDirection(void)
{
	int16_t gim_info[2];
	
	for (uint8_t i = 0; i < 2; i++)
	{
		Get_GimbalInfo(&gim_info[i], i);
	}
	if (gim_info[0] == gim_info[1])
	{
		return CHASSIS_FORWARD;
	}
	return CHASSIS_BACKWARD;
}

int16_t	Chassis_GetYawErr(void)
{
	int16_t	yaw_err;
	
	Get_GimbalInfo(&yaw_err, 2);
	
	return yaw_err;
}

float	Chassis_GetYawRadErr(void)
{
	int16_t	mid_yaw, mea_yaw, err_yaw;
	
	Get_GimbalInfo(&mid_yaw, 0);
	Get_GimbalInfo(&mea_yaw, 3);
	
	/*  error过零点处理  */
	err_yaw = mid_yaw - mea_yaw;
	
	if (err_yaw > HALF_ECD_RANGE)
	{
		err_yaw -= ECD_RANGE;
	}
	else if (err_yaw < -HALF_ECD_RANGE)
	{
		err_yaw += ECD_RANGE;
	}
	
	return (float)err_yaw * GM6020_ECD_TO_ANGLE * ANGLE_TO_RAD;
}

/**
  * @brief  底盘复位
  * @param  
  * @retval 
  */
void Chassis_Reset(void)
{
	static uint32_t reset_start_time = 0;
	
	if (symbal.gim_sym.reset_start == 1 && symbal.gim_sym.reset_ok == 0)
	{
		reset_start_time = HAL_GetTick();
	}
	
	if (symbal.gim_sym.reset_ok == 1)
	{
		chassis.info->chassis_lock = CHAS_UNLOCK;
	}
	else if (symbal.gim_sym.reset_ok == 0)
	{
		if (HAL_GetTick() - reset_start_time > 1000)
			chassis.info->chassis_lock = CHAS_UNLOCK;
	}
}


/**
  * @brief  底盘电机控制
  * @param  
  * @retval 
  */
	int16_t x_err, y_err, z_err;
void Chassis_MotorCtrl(void)
{
	float velocity[4], sum;
	
	x_err = sgn(chassis.info->target_x_speed - chassis.info->measure_x_speed);
	y_err = sgn(chassis.info->target_y_speed - chassis.info->measure_y_speed);
	z_err = sgn(chassis.info->target_z_speed - chassis.info->measure_z_speed);
	
	if (status.chas_cmd == gyro_on)
	{
		sum = abs(chassis.info->target_x_speed) + abs(chassis.info->target_y_speed) + abs(chassis.info->target_z_speed);
		
		if (module.dir == MODULE_DIR_ARMOR)
		{
			velocity[0] =-chassis.info->target_x_speed - chassis.info->target_y_speed - chassis.info->target_z_speed;
			velocity[1] =-chassis.info->target_x_speed + chassis.info->target_y_speed - chassis.info->target_z_speed;
			velocity[2] = chassis.info->target_x_speed - chassis.info->target_y_speed - chassis.info->target_z_speed;
			velocity[3] = chassis.info->target_x_speed + chassis.info->target_y_speed - chassis.info->target_z_speed;
		}
		else
		{
			velocity[0] =-chassis.info->target_x_speed - chassis.info->target_z_speed;
			velocity[1] =+chassis.info->target_y_speed - chassis.info->target_z_speed;
			velocity[2] =-chassis.info->target_y_speed - chassis.info->target_z_speed;
			velocity[3] =+chassis.info->target_x_speed - chassis.info->target_z_speed;
		}
		
		/* 直线校正begin */
//		velocity[0] += abs(velocity[0]) * (float)(-x_err - y_err - z_err) * 0.1f;
//		velocity[1] += abs(velocity[1]) * (float)(-x_err + y_err - z_err) * 0.1f;
//		velocity[2] += abs(velocity[2]) * (float)( x_err - y_err - z_err) * 0.1f;
//		velocity[3] += abs(velocity[3]) * (float)( x_err + y_err - z_err) * 0.1f;
		/* 直线校正end */
		
		if (sum > chassis.conf->limit_speed)
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				chassis.info->wheel_set[i] = velocity[i] / sum * chassis.conf->limit_speed;
			}
		}
		else
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				chassis.info->wheel_set[i] = velocity[i];
			}
		}
	}
	else
	{
		sum = abs(chassis.info->target_x_speed) + abs(chassis.info->target_y_speed);
		if (module.dir == MODULE_DIR_ARMOR)
		{
			velocity[0] =-chassis.info->target_x_speed - chassis.info->target_y_speed;
			velocity[1] =-chassis.info->target_x_speed + chassis.info->target_y_speed;
			velocity[2] = chassis.info->target_x_speed - chassis.info->target_y_speed;
			velocity[3] = chassis.info->target_x_speed + chassis.info->target_y_speed;
		}
		else
		{
			velocity[0] =-chassis.info->target_x_speed;
			velocity[1] =+chassis.info->target_y_speed;
			velocity[2] =-chassis.info->target_y_speed;
			velocity[3] =+chassis.info->target_x_speed;
		}
		
		/* 直线校正begin */
		velocity[0] += abs(velocity[0]) * (float)(-x_err - y_err - z_err) * 0.1f;
		velocity[1] += abs(velocity[1]) * (float)(-x_err + y_err - z_err) * 0.1f;
		velocity[2] += abs(velocity[2]) * (float)( x_err - y_err - z_err) * 0.1f;
		velocity[3] += abs(velocity[3]) * (float)( x_err + y_err - z_err) * 0.1f;
		/* 直线校正end */
		
		if ((sum + abs(chassis.info->target_z_speed)) > chassis.conf->limit_speed)
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				chassis.info->wheel_set[i] = velocity[i] / (sum + abs(chassis.info->target_z_speed)) * chassis.conf->limit_speed - chassis.info->target_z_speed;
			}
		}
		else
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				chassis.info->wheel_set[i] = velocity[i] - chassis.info->target_z_speed;
			}
		}
	}
		
	RM_motor[CHAS_1].base_info.motor_out = RM_motor[CHAS_1].ctr_speed(&RM_motor[CHAS_1], \
																																		chassis.info->wheel_set[0]);
	RM_motor[CHAS_2].base_info.motor_out = RM_motor[CHAS_2].ctr_speed(&RM_motor[CHAS_2], \
																																		chassis.info->wheel_set[1]);
	RM_motor[CHAS_3].base_info.motor_out = RM_motor[CHAS_3].ctr_speed(&RM_motor[CHAS_3], \
																																		chassis.info->wheel_set[2]);
	RM_motor[CHAS_4].base_info.motor_out = RM_motor[CHAS_4].ctr_speed(&RM_motor[CHAS_4], \
																																		chassis.info->wheel_set[3]);
	
	chas_out[RM_motor[CHAS_1].id.buff_p] = RM_motor[CHAS_1].base_info.motor_out;
	chas_out[RM_motor[CHAS_2].id.buff_p] = RM_motor[CHAS_2].base_info.motor_out;
	chas_out[RM_motor[CHAS_3].id.buff_p] = RM_motor[CHAS_3].base_info.motor_out;
	chas_out[RM_motor[CHAS_4].id.buff_p] = RM_motor[CHAS_4].base_info.motor_out;
	
		/* 直线校正begin */
//	Chassis_Motor_Power_Limit(chas_out);
//	
//		chas_out[0] += abs(chas_out[0]) * (float)(-x_err - y_err - z_err) * 0.05f;
//		chas_out[1] += abs(chas_out[1]) * (float)(-x_err + y_err - z_err) * 0.05f;
//		chas_out[2] += abs(chas_out[2]) * (float)( x_err - y_err - z_err) * 0.05f;
//		chas_out[3] += abs(chas_out[3]) * (float)( x_err + y_err - z_err) * 0.05f;
		/* 直线校正end */
	
}


/**
  * @brief  底盘控制输出
  * @param  
  * @retval 
  */
void Chassis_SendOut(void)
{
	Chassis_Motor_Power_Limit(chas_out);
	
	if (RM_motor[CHAS_1].state.work_state == M_ONLINE)
		can1_send_buff[RM_motor[CHAS_1].id.buff_p] = chas_out[RM_motor[CHAS_1].id.buff_p];
	else
		can1_send_buff[RM_motor[CHAS_1].id.buff_p] = 0;
	
	if (RM_motor[CHAS_2].state.work_state == M_ONLINE)
		can1_send_buff[RM_motor[CHAS_2].id.buff_p] = chas_out[RM_motor[CHAS_2].id.buff_p];
	else
		can1_send_buff[RM_motor[CHAS_2].id.buff_p] = 0;
	
	if (RM_motor[CHAS_3].state.work_state == M_ONLINE)
		can1_send_buff[RM_motor[CHAS_3].id.buff_p] = chas_out[RM_motor[CHAS_3].id.buff_p];
	else
		can1_send_buff[RM_motor[CHAS_3].id.buff_p] = 0;
	
	if (RM_motor[CHAS_4].state.work_state == M_ONLINE)
		can1_send_buff[RM_motor[CHAS_4].id.buff_p] = chas_out[RM_motor[CHAS_4].id.buff_p];
	else
		can1_send_buff[RM_motor[CHAS_4].id.buff_p] = 0;
	
}


/**
  * @brief  底盘保护
  * @param  
  * @retval 
  */
void Chassis_Stop(void)
{
	RM_motor[CHAS_1].pid.speed.info.out = 0.0f;
	RM_motor[CHAS_2].pid.speed.info.out = 0.0f;
	RM_motor[CHAS_3].pid.speed.info.out = 0.0f;
	RM_motor[CHAS_4].pid.speed.info.out = 0.0f;
	chas_out[0] = (int16_t)RM_motor[CHAS_1].pid.speed.info.out;
	chas_out[1] = (int16_t)RM_motor[CHAS_2].pid.speed.info.out;
	chas_out[2] = (int16_t)RM_motor[CHAS_3].pid.speed.info.out;
	chas_out[3] = (int16_t)RM_motor[CHAS_4].pid.speed.info.out;
	
	can1_send_buff[0] = chas_out[0];
	can1_send_buff[1] = chas_out[1];
	can1_send_buff[2] = chas_out[2];
	can1_send_buff[3] = chas_out[3];
	
}



