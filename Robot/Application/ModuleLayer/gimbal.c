
/* Includes ------------------------------------------------------------------*/
#include "gimbal.h"

#include "rp_math.h"
#include "arm_math.h"
/* Private macro -------------------------------------------------------------*/
#define ARMOR_ANGLE 5766
#define WHEEL_ANGLE 6790

/* Private function prototypes -----------------------------------------------*/
void Gimbal_Init(void);
void Gimbal_Ctrl(void);
void Gimbal_SelfProtect(void);

void Gimbal_GetInfo(void);
void Gimbal_GetBaseInfo(void);
void Gimbal_YawResetCheck(void);
void Gimbal_YawTurn(void);
void Gimbal_GetRcInfo(void);
void Gimbal_GetKeyInfo(void);
void Gimbal_Reset(void);
uint8_t	Gimbal_ResetCheck(void);
void Gimbal_MotoReset(void);
void Gimbal_GyroReset(void);
void Gimbal_MotorCtrl(void);
void Gimbal_Yaw_Angle_PidCalc(void);
void Gimbal_Pitch_Angle_PidCalc(void);
void Gimbal_SendOut(void);
void Gimbal_Stop(void);

void Gimbal_Test(void);
void Get_GimbalInfo(int16_t *get_info, int16_t data_offset);
/* Private typedef -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//  0:yaw轴  1:pitch轴
int16_t  gim_out[2];

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/
float yaw_gyro_position[7] = {20.f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f};
float yaw_gyro_position_in[7] = {200.f, 2.f, 0.f, 0.f, 10000.f, 15000.f, 25000.f};
float pitch_gyro_position[7] = {60.f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f};
float pitch_gyro_position_in[7] = {80.f, 2.f, 0.f, 0.f, 10000.f, 15000.f, 25000.f};

//float yaw_mach_position[7] = {0.625f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f}; // 0.625f, 0.f
//float yaw_mach_position_in[7] = {200.f ,1.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f}; // 350.f, 1.f
//float pitch_mach_position[7] = {-1.f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f};  // -1.f, 0.f
//float pitch_mach_position_in[7] = {150.f, 1.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f}; // 200.f, 7.f

// 云台设备
gimbal_dev_t		gim_dev = {
	.yaw_motor = &RM_motor[GIM_Y],
	.pitch_motor = &RM_motor[GIM_P],
	.imu_sensor = &imu_sensor,
	.rc_sensor = &rc_sensor,
};

// 云台模块信息
gimbal_info_t 	gim_info = {
	.remote_mode = RC,
};

gimbal_conf_t   gim_conf = {
	.restart_yaw_imu_angle = 0.0f,
	.restart_pitch_imu_angle = 0.0f,
	.MID_VALUE = ARMOR_ANGLE,//  前 5766 后 1670 
	.restart_yaw_motor_angle = ARMOR_ANGLE,
	.restart_pitch_motor_angle = 1213,
	.rc_pitch_motor_offset = 110,
	.rc_yaw_imu_offset = 1100.0f,//3300
	.rc_pitch_imu_offset = 3300.0f,
	.key_pitch_motor_offset = 33,
	.key_yaw_imu_offset = -800.0f,//330
	.key_pitch_imu_offset = 1000.0f,
	.max_pitch_imu_angle = 23.0f,
	.min_pitch_imu_angle = -40.0f,
	.max_pitch_motor_angle = 2247,
	.min_pitch_motor_angle = 697,
};

/* Exported variables --------------------------------------------------------*/
extern int16_t can1_send_buf[8];

gimbal_t gimbal = {
	.dev = &gim_dev,
	.info = &gim_info,
	.conf = &gim_conf,
	.init = Gimbal_Init,
	.ctrl = Gimbal_Ctrl,
	.self_protect = Gimbal_SelfProtect,
};

/* Exported functions --------------------------------------------------------*/
void Gimbal_Init(void)
{
	
}


void Get_GimbalInfo(int16_t *get_info, int16_t data_case)
{
	switch (data_case)
	{
		case 0:
			*get_info = gimbal.conf->MID_VALUE;
			break;
		case 1:
			*get_info = gimbal.conf->restart_yaw_motor_angle;
			break;
		case 2:
			*get_info = gimbal.info->yaw_motor_angle_err;
			break;
		case 3:
			*get_info = gimbal.info->measure_yaw_motor_angle;
		default:
			break;
	}
}

/**
  * @brief  云台控制
  * @param  
  * @retval 
  */
void Gimbal_Ctrl(void)
{
	Gimbal_GetInfo();
	
	Gimbal_Reset();
	
//	Gimbal_Test();
	
	Gimbal_MotorCtrl();
	
	Gimbal_SendOut();
}


/**
  * @brief  云台控制离线保护
  * @param  
  * @retval 
  */
void Gimbal_SelfProtect(void)
{
	Gimbal_Stop();
	Gimbal_GetInfo();
}

/* Private functions ---------------------------------------------------------*/
void Gimbal_Test(void)
{
	static uint16_t cnt = 0;
	gimbal.info->target_pitch_imu_angle = 0.f;
	if (abs(gimbal.info->target_yaw_imu_angle - gimbal.info->measure_yaw_imu_angle) < 0.5f)
	{
		cnt++;
		if (cnt == 500)
		{
			if (gimbal.info->target_yaw_imu_angle > 0)
			{
				gimbal.info->target_yaw_imu_angle = -30.f;
			}
			else
			{
				gimbal.info->target_yaw_imu_angle = 30.f;
			}
		}
	}
	else
	{
		cnt = 0;
	}
}


/**
  * @brief  云台数据读取
  * @param  
  * @retval 
  */
void Gimbal_GetInfo(void)
{
	Gimbal_GetBaseInfo();
	
	if (symbal.gim_sym.reset_ok == 0)
	{
		if (abs(gim_conf.restart_yaw_motor_angle - gimbal.info->measure_yaw_motor_angle) > 25)
		{
			gimbal.info->target_pitch_imu_angle = gim_conf.restart_pitch_imu_angle;
			gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle \
																				+ (float)gimbal.info->yaw_motor_angle_err * ECD_TO_ANGLE;
		}
		else if (symbal.gim_sym.reset_start == 0)
		{
			symbal.gim_sym.reset_ok = Gimbal_ResetCheck();
		}
	}
	else
	{
		if(gim_info.remote_mode == RC) {
			Gimbal_GetRcInfo();
		}
		else if(gim_info.remote_mode == KEY) {
			Gimbal_GetKeyInfo();
		}
	}
}

void Gimbal_GetBaseInfo(void)
{
	int16_t	err_motor_max = 0, err_motor_min = 0;
	// 电机数据更新
	gimbal.info->measure_yaw_motor_angle = RM_motor[GIM_Y].rx_info.angle;
	gimbal.info->measure_yaw_motor_speed = RM_motor[GIM_Y].rx_info.speed;
	gimbal.info->measure_pitch_motor_angle = RM_motor[GIM_P].rx_info.angle;
	gimbal.info->measure_pitch_motor_speed = RM_motor[GIM_P].rx_info.speed;
	
	// imu数据更新
	gimbal.info->measure_yaw_imu_speed = imu_sensor.info->base_info.ave_rate_yaw;
	gimbal.info->measure_yaw_imu_angle = imu_sensor.info->base_info.yaw;
	gimbal.info->measure_pitch_imu_speed = imu_sensor.info->base_info.ave_rate_pitch;
	gimbal.info->measure_pitch_imu_angle = imu_sensor.info->base_info.pitch;
	gimbal.info->measure_roll_imu_speed = imu_sensor.info->base_info.ave_rate_roll;
	gimbal.info->measure_roll_imu_angle = imu_sensor.info->base_info.roll;
	
	// pitch轴电机imu动态限位
	err_motor_max = gimbal.info->measure_pitch_motor_angle - gimbal.conf->min_pitch_motor_angle;
	err_motor_min = gimbal.info->measure_pitch_motor_angle - gimbal.conf->max_pitch_motor_angle;
	if (abs(err_motor_max) > HALF_ECD_RANGE)
		err_motor_max -= one(err_motor_max) * ECD_RANGE;
	if (abs(err_motor_min) > HALF_ECD_RANGE)
		err_motor_min -= one(err_motor_min) * ECD_RANGE;
	gimbal.conf->max_pitch_imu_angle = gimbal.info->measure_pitch_imu_angle + ECD_TO_ANGLE * err_motor_max;
	gimbal.conf->min_pitch_imu_angle = gimbal.info->measure_pitch_imu_angle + ECD_TO_ANGLE * err_motor_min;
	
	if (gimbal.conf->max_pitch_imu_angle > 90.f)
		gimbal.conf->max_pitch_imu_angle = 90.f;
	if (gimbal.conf->min_pitch_imu_angle < -90.f)
		gimbal.conf->min_pitch_imu_angle = -90.f;
	
	
	/* 云台朝向 */
	if (module.dir == MODULE_DIR_ARMOR)
	{
		gimbal.conf->MID_VALUE = ARMOR_ANGLE;
		gimbal.conf->restart_yaw_motor_angle = ARMOR_ANGLE;
	}
	else
	{
		gimbal.conf->MID_VALUE = WHEEL_ANGLE;
		gimbal.conf->restart_yaw_motor_angle = WHEEL_ANGLE;
	}
	// yaw轴电机就近归中
	Gimbal_YawResetCheck();
	
	// 世界坐标系数据更新
//	gimbal.info->measure_pitch_global_speed = gimbal.info->measure_pitch_imu_speed * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																			  	- gimbal.info->measure_yaw_imu_speed * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
//	gimbal.info->measure_yaw_global_speed = gimbal.info->measure_pitch_imu_speed * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																				+ gimbal.info->measure_yaw_imu_speed * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
//	gimbal.info->measure_pitch_global_angle = gimbal.info->measure_pitch_imu_angle * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																			  	- gimbal.info->measure_yaw_imu_angle * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
//	gimbal.info->measure_yaw_global_angle = gimbal.info->measure_yaw_imu_angle * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																				- gimbal.info->measure_pitch_imu_angle * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
	
}

/**
  * @brief  yaw轴电机就近归中
  * @param  
  * @retval 
  */
void Gimbal_YawResetCheck(void)
{
	/*  yaw轴电机error过零点处理  */
	gimbal.info->yaw_motor_angle_err = gim_conf.restart_yaw_motor_angle - gimbal.info->measure_yaw_motor_angle;
	
	if (gimbal.info->yaw_motor_angle_err > HALF_ECD_RANGE)
	{
		gimbal.info->yaw_motor_angle_err -= ECD_RANGE;
	}
	else if (gimbal.info->yaw_motor_angle_err < -HALF_ECD_RANGE)
	{
		gimbal.info->yaw_motor_angle_err += ECD_RANGE;
	}
	/*  yaw轴电机error调整  */
	if (abs(gimbal.info->yaw_motor_angle_err) > (HALF_ECD_RANGE / 2))
	{
		if (gimbal.conf->restart_yaw_motor_angle > HALF_ECD_RANGE)
		{
			gimbal.conf->restart_yaw_motor_angle = gimbal.conf->MID_VALUE - HALF_ECD_RANGE;
		}
		else
		{
			gimbal.conf->restart_yaw_motor_angle = gimbal.conf->MID_VALUE;
		}
		gimbal.info->yaw_motor_angle_err = gim_conf.restart_yaw_motor_angle - gimbal.info->measure_yaw_motor_angle;
	}
		
}

/**
  * @brief  云台复位
  * @param  
  * @retval 
  */
void Gimbal_Reset(void)
{
	if (symbal.gim_sym.reset_start == 1 && symbal.gim_sym.reset_ok == 0)
	{
		if (gimbal.info->gimbal_mode == gim_machine)
		{
			Gimbal_MotoReset();
		}
		else if (gimbal.info->gimbal_mode == gim_gyro)
		{
			Gimbal_GyroReset();
		}
	}
}

void Gimbal_MotoReset(void)
{
	gimbal.info->target_pitch_motor_angle = gim_conf.restart_pitch_motor_angle;
	gimbal.info->target_yaw_motor_angle = gim_conf.restart_yaw_motor_angle;
	
	symbal.gim_sym.reset_start = 0;
}

void Gimbal_GyroReset(void)
{
	
	gimbal.info->target_pitch_imu_angle = gim_conf.restart_pitch_imu_angle;
	gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle \
																		+ (float)gimbal.info->yaw_motor_angle_err * ECD_TO_ANGLE;
	
	symbal.gim_sym.reset_start = 0;
}


/**
  * @brief  云台复位检测
  * @param  
  * @retval 
  */
uint8_t	Gimbal_ResetCheck(void)
{
	if (symbal.gim_sym.reset_ok == 0)
	{
		if (gimbal.info->gimbal_mode == gim_machine)
		{
			int16_t pitch_err, yaw_err;
			
			pitch_err = gimbal.info->measure_pitch_motor_angle - gimbal.info->target_pitch_motor_angle;
			yaw_err = gimbal.info->measure_yaw_motor_angle - gimbal.info->target_yaw_motor_angle;
			if ((abs(pitch_err) < 25) && (abs(yaw_err < 25)))
				return	1;
		}
		else if (gimbal.info->gimbal_mode == gim_gyro)
		{
			float pitch_err, yaw_err;
			
			pitch_err = gimbal.info->measure_pitch_imu_angle - gimbal.info->target_pitch_imu_angle;
			yaw_err = gimbal.info->measure_yaw_imu_angle - gimbal.info->target_yaw_imu_angle;
			if ((abs(pitch_err) < 1.f) && (abs(yaw_err) < 1.f))
				return 1;
		}
	}
	return 0;
}


void Gimbal_GetRcInfo(void)
{
	if (gimbal.info->gimbal_mode == gim_machine)
	{
		gimbal.info->target_pitch_motor_deltaangle = -(float)rc_sensor.info->ch1 / (float)gim_conf.rc_pitch_motor_offset;
		gimbal.info->target_pitch_imu_angle = gimbal.info->measure_pitch_imu_angle;
		gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle;
	}
	else if (gimbal.info->gimbal_mode == gim_gyro)
	{
		gimbal.info->target_pitch_motor_angle = gimbal.info->measure_pitch_motor_angle;
		gimbal.info->target_pitch_imu_deltaangle = (float)rc_sensor.info->ch1 / gim_conf.rc_pitch_imu_offset;
		gimbal.info->target_yaw_imu_deltaangle = -(float)rc_sensor.info->ch0 / gim_conf.rc_yaw_imu_offset;
	}
	else if (gimbal.info->gimbal_mode == gim_vision)
	{
		gimbal.info->target_pitch_motor_angle = gimbal.info->measure_pitch_motor_angle;
		gimbal.info->target_pitch_imu_deltaangle = (float)rc_sensor.info->ch1 / gim_conf.rc_pitch_imu_offset;
		gimbal.info->target_yaw_imu_deltaangle = -(float)rc_sensor.info->ch0 / gim_conf.rc_yaw_imu_offset;
		if (vision_sensor.work_state == DEV_ONLINE)
		{
			if ((vision_sensor.info->is_find_target == 1) || (vision_sensor.info->is_find_buff == 1))
			{
				gimbal.info->target_pitch_imu_deltaangle = 0.f;
				gimbal.info->target_yaw_imu_deltaangle = 0.f;
				gimbal.info->target_pitch_motor_angle = gimbal.info->measure_pitch_motor_angle;
				gimbal.info->target_pitch_imu_angle = (vision_sensor.info->target_pitch_angle - (float)HALF_ECD_RANGE) * ECD_TO_ANGLE;
				gimbal.info->target_yaw_imu_angle = vision_sensor.info->target_yaw_angle * ECD_TO_ANGLE - 180.f;
			}
		}
	}
	else
	{
		gimbal.info->target_pitch_imu_angle = gimbal.info->measure_pitch_imu_angle;
		gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle;
	}
	
	if (status.gim_cmd == gim_turn)
	{
		Gimbal_YawTurn();
	}
	
	if (status.lch_state.magz_state == magz_open)
	{
		gimbal.info->target_pitch_imu_angle = 0.f;
		gimbal.info->target_pitch_motor_angle = gimbal.conf->restart_pitch_motor_angle;
		gimbal.info->target_yaw_imu_deltaangle /= 10.f;
	}
}

void Gimbal_GetKeyInfo(void)
{
	
	if (gimbal.info->gimbal_mode == gim_machine)
	{
		gimbal.info->target_pitch_motor_deltaangle = -rc_sensor.info->mouse_y / (float)gim_conf.key_pitch_motor_offset;
		gimbal.info->target_pitch_imu_angle = gimbal.info->measure_pitch_imu_angle;
		gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle;
	}
	else if (gimbal.info->gimbal_mode == gim_gyro)
	{
		gimbal.info->target_pitch_imu_deltaangle = rc_sensor.info->mouse_y / gim_conf.key_pitch_imu_offset;
		gimbal.info->target_yaw_imu_deltaangle = rc_sensor.info->mouse_x / gim_conf.key_yaw_imu_offset;
	}
	else if (gimbal.info->gimbal_mode == gim_vision)
	{
		gimbal.info->target_pitch_imu_deltaangle = rc_sensor.info->mouse_y / gim_conf.key_pitch_imu_offset;
		gimbal.info->target_yaw_imu_deltaangle = rc_sensor.info->mouse_x / gim_conf.key_yaw_imu_offset;
		if (vision_sensor.work_state == DEV_ONLINE)
		{
			if ((vision_sensor.info->is_find_target == 1) || (vision_sensor.info->is_find_buff == 1))
			{
				gimbal.info->target_pitch_imu_deltaangle = 0.f;
				gimbal.info->target_yaw_imu_deltaangle = 0.f;
				gimbal.info->target_pitch_motor_angle = gimbal.info->measure_pitch_motor_angle;
				gimbal.info->target_pitch_imu_angle = (vision_sensor.info->target_pitch_angle - (float)HALF_ECD_RANGE) * ECD_TO_ANGLE;
				gimbal.info->target_yaw_imu_angle = vision_sensor.info->target_yaw_angle * ECD_TO_ANGLE - 180.f;
			}
		}
	}
	
	if (status.gim_cmd == gim_turn)
	{
		Gimbal_YawTurn();
	}
	
	if (status.lch_state.magz_state == magz_open)
	{
		gimbal.info->target_pitch_imu_angle = 0.f;
		gimbal.info->target_pitch_motor_angle = gimbal.conf->restart_pitch_motor_angle;
		gimbal.info->target_yaw_imu_deltaangle /= 10.f;
	}
	
	if (symbal.gim_sym.turn_left == 1)
	{
		gimbal.info->target_yaw_imu_angle += 90.f;
		symbal.gim_sym.turn_left = 0;
	}
	if (symbal.gim_sym.turn_right == 1)
	{
		gimbal.info->target_yaw_imu_angle -= 90.f;
		symbal.gim_sym.turn_right = 0;
	}
}


/**
  * @brief  云台掉头
  * @param  
  * @retval 
  */
void Gimbal_YawTurn(void)
{
	if (symbal.gim_sym.turn_start == 1)
	{
		symbal.gim_sym.turn_start = 0;
		if (gimbal.info->measure_yaw_imu_angle > 0.f)
		{
			gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle - 179.5f;
		}
		else
		{
			gimbal.info->target_yaw_imu_angle = gimbal.info->measure_yaw_imu_angle + 179.5f;
		}
	}
}

/**
  * @brief  云台电机控制
  * @param  
  * @retval 
  */
void Gimbal_MotorCtrl(void)
{
	// 机械模式
	if (gimbal.info->gimbal_mode == gim_machine)
	{
			gimbal.info->target_pitch_motor_angle += gimbal.info->target_pitch_motor_deltaangle;
			
			// pitch轴电机限位
			if (gimbal.info->target_pitch_motor_angle > gim_conf.max_pitch_motor_angle)
			{
				gimbal.info->target_pitch_motor_angle = gim_conf.max_pitch_motor_angle;
			}
			else if (gimbal.info->target_pitch_motor_angle < gim_conf.min_pitch_motor_angle)
			{
				gimbal.info->target_pitch_motor_angle = gim_conf.min_pitch_motor_angle;
			}
		
			
			// 云台朝向检查
			symbal.gim_sym.forward = 0;
			if (gimbal.conf->restart_yaw_motor_angle == gimbal.conf->MID_VALUE)
				symbal.gim_sym.forward = 1;
			
			gim_info.target_yaw_motor_angle = gim_conf.restart_yaw_motor_angle;
			
	}
	// 陀螺仪模式
	else if (gimbal.info->gimbal_mode == gim_gyro)
	{
			gimbal.info->target_pitch_imu_angle += gimbal.info->target_pitch_imu_deltaangle;
			gimbal.info->target_yaw_imu_angle += gimbal.info->target_yaw_imu_deltaangle;
		
			///////////////////////////
//			gimbal.info->target_pitch_imu_angle = gimbal.info->target_pitch_global_angle * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																					+ gimbal.info->target_yaw_global_angle * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
//			gimbal.info->target_yaw_imu_angle = - gimbal.info->target_pitch_global_angle * arm_sin_f32(angle2rad(gimbal.info->measure_roll_imu_angle)) \
//																					+ gimbal.info->target_yaw_global_angle * arm_cos_f32(angle2rad(gimbal.info->measure_roll_imu_angle));
			
			
			// pitch轴电机限位
			if (gimbal.info->target_pitch_imu_angle > gim_conf.max_pitch_imu_angle)
			{
				gimbal.info->target_pitch_imu_angle = gim_conf.max_pitch_imu_angle;
			}
			else if (gimbal.info->target_pitch_imu_angle < gim_conf.min_pitch_imu_angle)
			{
				gimbal.info->target_pitch_imu_angle = gim_conf.min_pitch_imu_angle;
			}
			
			
			// yaw轴电机target过零点处理
			if (gimbal.info->target_yaw_imu_angle > 180.0f)
			{
				gimbal.info->target_yaw_imu_angle -= 360.0f;
			}
			else if (gimbal.info->target_yaw_imu_angle < -180.0f)
			{
				gimbal.info->target_yaw_imu_angle += 360.0f;
			}
	}
	else if (gimbal.info->gimbal_mode == gim_vision)
	{
			gimbal.info->target_pitch_imu_angle += gimbal.info->target_pitch_imu_deltaangle;
			gimbal.info->target_yaw_imu_angle += gimbal.info->target_yaw_imu_deltaangle;
		
			// pitch轴电机限位
			if (gimbal.info->target_pitch_imu_angle > gim_conf.max_pitch_imu_angle)
			{
				gimbal.info->target_pitch_imu_angle = gim_conf.max_pitch_imu_angle;
			}
			else if (gimbal.info->target_pitch_imu_angle < gim_conf.min_pitch_imu_angle)
			{
				gimbal.info->target_pitch_imu_angle = gim_conf.min_pitch_imu_angle;
			}
			
			
			// yaw轴电机target过零点处理
			if (gimbal.info->target_yaw_imu_angle > 180.0f)
			{
				gimbal.info->target_yaw_imu_angle -= 360.0f;
			}
			else if (gimbal.info->target_yaw_imu_angle < -180.0f)
			{
				gimbal.info->target_yaw_imu_angle += 360.0f;
			}
	}
	else
	{
			Gimbal_Stop();
	}
	
	
	// 云台换头
	if (symbal.gim_sym.turn_ok == 0)
	{
		if (abs(gimbal.info->target_yaw_imu_angle - gimbal.info->measure_yaw_imu_angle) < 0.5f)
		{
			symbal.gim_sym.turn_ok = 1;
		}
	}
	
	Gimbal_Yaw_Angle_PidCalc();
	Gimbal_Pitch_Angle_PidCalc();
}


float yaw_err;
void Gimbal_Yaw_Angle_PidCalc(void)
{
	if (gimbal.info->gimbal_mode == gim_machine)
	{
		// yaw轴电机error过零点处理
		if (gimbal.info->target_yaw_motor_angle - gimbal.info->measure_yaw_motor_angle > HALF_ECD_RANGE)
		{
			gimbal.info->measure_yaw_motor_angle += ECD_RANGE;
		}
		else if (gimbal.info->target_yaw_motor_angle - gimbal.info->measure_yaw_motor_angle < -HALF_ECD_RANGE)
		{
			gimbal.info->measure_yaw_motor_angle -= ECD_RANGE;
		}
		
		RM_motor[GIM_Y].base_info.motor_out = RM_motor[GIM_Y].ctr_pid2(&RM_motor[GIM_Y].pid.angle, 
																																	&RM_motor[GIM_Y].pid.angle_in,
																																	(float)gimbal.info->measure_yaw_motor_angle,
																																	gimbal.info->measure_yaw_imu_speed,
																																	(float)gimbal.info->target_yaw_motor_angle,
																																	0);
		gim_out[RM_motor[GIM_Y].id.buff_p] = RM_motor[GIM_Y].base_info.motor_out;
	}
	else if ((gimbal.info->gimbal_mode == gim_gyro) || (gimbal.info->gimbal_mode == gim_vision))
	{
		// yaw轴电机error过零点处理
		if (gimbal.info->target_yaw_imu_angle - gimbal.info->measure_yaw_imu_angle > 180.0f)
		{
			gimbal.info->measure_yaw_imu_angle += 360.0f;
		}
		else if (gimbal.info->target_yaw_imu_angle - gimbal.info->measure_yaw_imu_angle < -180.0f)
		{
			gimbal.info->measure_yaw_imu_angle -= 360.0f;
		}
		
		yaw_err = gimbal.info->target_yaw_imu_angle - gimbal.info->measure_yaw_imu_angle;
		RM_motor[GIM_Y].base_info.motor_out = RM_motor[GIM_Y].ctr_pid2(&RM_motor[GIM_Y].pid.position, 
																																	&RM_motor[GIM_Y].pid.position_in,
																																	gimbal.info->measure_yaw_imu_angle,
																																	gimbal.info->measure_yaw_imu_speed,
																																	gimbal.info->target_yaw_imu_angle,
																																	0);
		gim_out[RM_motor[GIM_Y].id.buff_p] = RM_motor[GIM_Y].base_info.motor_out;
	}
}


void Gimbal_Pitch_Angle_PidCalc(void)
{
//	float forward_back = 0.f;
//	forward_back = - 47.f * gimbal.info->target_pitch_imu_angle - 6200.f;//前馈
	
	if (gimbal.info->gimbal_mode == gim_machine)
	{
		RM_motor[GIM_P].base_info.motor_out = -RM_motor[GIM_P].ctr_pid2(&RM_motor[GIM_P].pid.angle, 
																																	&RM_motor[GIM_P].pid.angle_in,
																																	(float)gimbal.info->measure_pitch_motor_angle,
																																	gimbal.info->measure_pitch_imu_speed,
																																	(float)gimbal.info->target_pitch_motor_angle,
																																	0);
		gim_out[RM_motor[GIM_P].id.buff_p] = RM_motor[GIM_P].base_info.motor_out;
	}
	else if ((gimbal.info->gimbal_mode == gim_gyro) || (gimbal.info->gimbal_mode == gim_vision))
	{
		// pitch轴陀螺仪增量与电机增量方向不同
		RM_motor[GIM_P].base_info.motor_out = -RM_motor[GIM_P].ctr_pid2(&RM_motor[GIM_P].pid.position, 
																																	&RM_motor[GIM_P].pid.position_in,
																																	gimbal.info->measure_pitch_imu_angle,
																																	gimbal.info->measure_pitch_imu_speed,
																																	gimbal.info->target_pitch_imu_angle,
																																	0);
		gim_out[RM_motor[GIM_P].id.buff_p] = RM_motor[GIM_P].base_info.motor_out;// + forward_back;
	}
	
//	
//	//输出限幅
//	if (abs(gim_out[RM_motor[GIM_P].id.buff_p]) > 25000)
//	{
//		gim_out[RM_motor[GIM_P].id.buff_p] = one(gim_out[RM_motor[GIM_P].id.buff_p]) * 25000;
//	}
//	
}


/**
  * @brief  云台控制输出
  * @param  
  * @retval 
  */
void Gimbal_SendOut(void)
{
	// yaw轴电机离线保护
	if (RM_motor[GIM_Y].state.work_state == M_ONLINE)
	{
		can1_send_buf[RM_motor[GIM_Y].id.buff_p] = gim_out[RM_motor[GIM_Y].id.buff_p];
	}
	else
	{
		can1_send_buf[RM_motor[GIM_Y].id.buff_p] = 0;
	}
	
	// pitch轴电机离线保护
	if (RM_motor[GIM_P].state.work_state == M_ONLINE)
	{
		can1_send_buf[RM_motor[GIM_P].id.buff_p] = gim_out[RM_motor[GIM_P].id.buff_p];
	}
	else
	{
		can1_send_buf[RM_motor[GIM_P].id.buff_p] = 0;
	}
}



void Gimbal_Stop()
{
	RM_motor[GIM_P].pid.position_in.info.out = 0.0f;
	RM_motor[GIM_Y].pid.position_in.info.out = 0.0f;
	gim_out[0] = (int16_t)RM_motor[GIM_P].pid.position_in.info.out;
	gim_out[1] = (int16_t)RM_motor[GIM_Y].pid.position_in.info.out;
	
	can1_send_buf[0] = gim_out[0];
	can1_send_buf[1] = gim_out[1];
}


