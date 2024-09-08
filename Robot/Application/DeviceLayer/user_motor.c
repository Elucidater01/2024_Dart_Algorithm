/**
 * @file        user_motor.c
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        18-November-2022
 * @brief       User Motor Devives Config
 * @update
 */

/* Includes ------------------------------------------------------------------*/
#include "user_motor.h"

/*
* 使用电机前必须配置好电机的类型、id号、驱动方式
*/

//发送ID会在调用init函数后自动获取
RM_motor_t RM_motor[MOTOR_LIST] = 
{
	[FRIC_R] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id = 0x201,
		
		.init = motor_class_init,
	},
	[FRIC_L] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id = 0x202,
		
		.init = motor_class_init,
	},
	[DIAL] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id = 0x203,
		
		.init = motor_class_init,
	},
	[CHAS_1] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = 0x201,
		
		.init = motor_class_init,
	},
	[CHAS_2] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = 0x202,
		
		.init = motor_class_init,
	},
	[CHAS_3] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = 0x203,
		
		.init = motor_class_init,
	},
	[CHAS_4] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = 0x204,
		
		.init = motor_class_init,
	},
	[GIM_P] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = GM6020,
		.id.rx_id = 0x205,
		
		.init = motor_class_init,
	},
	[GIM_Y] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = GM6020,
		.id.rx_id = 0x206,
		
		.init = motor_class_init,
	},
};

pid_type_t chas_pid = {
	.Kp = 2.3f,
	.Ki = 0.01f,
	.Kd = 0.f,
	.max_integral = 10000.f,
  .max_out = 8000.f,
  .max_iout = 8000.f,
};

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/
float fric_r_speed_pid_param[7] = {15.f, 0.5f, 0.5f, 0.f, 6000.f, 6000.f, 12000.f};
float fric_l_speed_pid_param[7] = {15.f, 0.5f, 0.5f, 0.f, 6000.f, 6000.f, 12000.f};
float chassis_speed_pid_param[7] = {15.f, 0.6f, 0.f, 0.f, 10000.f, 6000.f, 8000.f};
float dial_position_in_pid_param[7] = {17.f, 1.2f, 0.f, 0.f, 6000.f, 6000.f, 12000.f};
float dial_position_pid_param[7] = {0.24414f, 0.f, 0.f, 0.f, 0.f, 0.f, 10000.f};
float gim_p_position_in_pid_param[7] = {300.f, 1.5f, 0.f, 0.f, 10000.f, 15000.f, 29000.f};
float gim_p_position_pid_param[7] = {30.f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 30000.f};
//float gim_p_position_in_pid_param[7] = {200.f, 0.f, 120.f, 0.f, 10000.f, 15000.f, 25000.f};
//float gim_p_position_pid_param[7] = {40.f, 0.01f, 5.f, 0.f, 10000.f, 10000.f, 25000.f};

float gim_y_position_in_pid_param[7] = {200.f, 2.f, 0.f, 0.f, 10000.f, 15000.f, 29000.f};
float gim_y_position_pid_param[7] = {21.f, 0.f, 30.f, 0.f, 10000.f, 10000.f, 30000.f};

float yaw_mach_position[7] = {0.92285f, 0.f, 1.318f, 0.f, 10000.f, 10000.f, 25000.f};
float yaw_mach_position_in[7] = {200.f, 2.f, 0.f, 0.f, 10000.f, 15000.f, 29000.f};
float pitch_mach_position[7] = {-1.f, 0.f, 0.f, 0.f, 10000.f, 10000.f, 25000.f};
float pitch_mach_position_in[7] = {300.f, 1.5f, 0.f, 0.f, 10000.f, 15000.f, 29000.f};
/* Exported variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 *	@brief	RM电机总初始化
 */
void RM_motor_init(void)
{
	//电机初始化
	RM_motor[FRIC_R].init(&RM_motor[FRIC_R]);
	RM_motor[FRIC_L].init(&RM_motor[FRIC_L]);
	RM_motor[DIAL].init(&RM_motor[DIAL]);
	RM_motor[CHAS_1].init(&RM_motor[CHAS_1]);
	RM_motor[CHAS_2].init(&RM_motor[CHAS_2]);
	RM_motor[CHAS_3].init(&RM_motor[CHAS_3]);
	RM_motor[CHAS_4].init(&RM_motor[CHAS_4]);
	RM_motor[GIM_P].init(&RM_motor[GIM_P]);
	RM_motor[GIM_Y].init(&RM_motor[GIM_Y]);
	//电机速度pid初始化
	RM_motor[FRIC_R].pid_init(&RM_motor[FRIC_R].pid.speed, fric_r_speed_pid_param);
	RM_motor[FRIC_L].pid_init(&RM_motor[FRIC_L].pid.speed, fric_l_speed_pid_param);
	RM_motor[DIAL].pid_init(&RM_motor[DIAL].pid.position_in, dial_position_in_pid_param);
	RM_motor[DIAL].pid_init(&RM_motor[DIAL].pid.position, dial_position_pid_param);
	RM_motor[CHAS_1].pid_init(&RM_motor[CHAS_1].pid.speed, chassis_speed_pid_param);
	RM_motor[CHAS_2].pid_init(&RM_motor[CHAS_2].pid.speed, chassis_speed_pid_param);
	RM_motor[CHAS_3].pid_init(&RM_motor[CHAS_3].pid.speed, chassis_speed_pid_param);
	RM_motor[CHAS_4].pid_init(&RM_motor[CHAS_4].pid.speed, chassis_speed_pid_param);
	RM_motor[GIM_P].pid_init(&RM_motor[GIM_P].pid.position_in, gim_p_position_in_pid_param);
	RM_motor[GIM_P].pid_init(&RM_motor[GIM_P].pid.position, gim_p_position_pid_param);
	RM_motor[GIM_Y].pid_init(&RM_motor[GIM_Y].pid.position_in, gim_y_position_in_pid_param);
	RM_motor[GIM_Y].pid_init(&RM_motor[GIM_Y].pid.position, gim_y_position_pid_param);
	RM_motor[GIM_P].pid_init(&RM_motor[GIM_P].pid.angle_in, pitch_mach_position_in);
	RM_motor[GIM_P].pid_init(&RM_motor[GIM_P].pid.angle, pitch_mach_position);
	RM_motor[GIM_Y].pid_init(&RM_motor[GIM_Y].pid.angle_in, yaw_mach_position_in);
	RM_motor[GIM_Y].pid_init(&RM_motor[GIM_Y].pid.angle, yaw_mach_position);
//	RM_motor[0].pid.speed.info.integral
}


/**
 *	@brief	单个RM电机离线检测
 */
uint8_t single_motor_offline_check(void)
{
	if ((RM_motor[FRIC_R].state.work_state == M_OFFLINE) || \
			(RM_motor[FRIC_L].state.work_state == M_OFFLINE) || \
			(RM_motor[DIAL].state.work_state == M_OFFLINE) || \
			(RM_motor[CHAS_1].state.work_state == M_OFFLINE) || \
			(RM_motor[CHAS_2].state.work_state == M_OFFLINE) || \
			(RM_motor[CHAS_3].state.work_state == M_OFFLINE) || \
			(RM_motor[CHAS_4].state.work_state == M_OFFLINE) || \
			(RM_motor[GIM_P].state.work_state == M_OFFLINE) || \
			(RM_motor[GIM_Y].state.work_state == M_OFFLINE))
	{
		return 1;
	}
	return 0;
}

/**
 *	@brief	RM电机离线检测
 */
uint8_t motor_offline_check(void)
{
	if ((RM_motor[FRIC_R].state.work_state == M_OFFLINE) && \
			(RM_motor[FRIC_L].state.work_state == M_OFFLINE) && \
			(RM_motor[DIAL].state.work_state == M_OFFLINE) && \
			(RM_motor[CHAS_1].state.work_state == M_OFFLINE) && \
			(RM_motor[CHAS_2].state.work_state == M_OFFLINE) && \
			(RM_motor[CHAS_3].state.work_state == M_OFFLINE) && \
			(RM_motor[CHAS_4].state.work_state == M_OFFLINE) && \
			(RM_motor[GIM_P].state.work_state == M_OFFLINE) && \
			(RM_motor[GIM_Y].state.work_state == M_OFFLINE))
	{
		return 1;
	}
	return 0;
}
