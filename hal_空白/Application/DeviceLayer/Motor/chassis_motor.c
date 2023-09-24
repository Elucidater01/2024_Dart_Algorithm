/**
 * @file        chassis_motor.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        11-September-2020
 * @brief       Chassis Motor(RM3508).
 */
 
/* Includes ------------------------------------------------------------------*/
#include "chassis_motor.h"

#include "can_potocol.h"
#include "rp_math.h"

extern void chassis_motor_update(chassis_motor_t *motor, uint8_t *rxBuf);
extern void chassis_motor_init(chassis_motor_t *motor);

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void chassis_motor_check(chassis_motor_t *motor);
static void chassis_motor_heart_beat(chassis_motor_t *motor);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// 底盘电机驱动
drv_can_t		chassis_motor_driver[] = {
	[CHAS_L] = {
		.type = DRV_CAN1,
		.can_id = CHASSIS_CAN_ID_L,
//		.std_id = RM3508_GetStdId,
//		.drv_id = RM3508_GetDrvId,
		.tx_data = CAN_SendSingleData,
	},
	[CHAS_R] = {
		.type = DRV_CAN1,
		.can_id = CHASSIS_CAN_ID_R,
//		.std_id = RM3508_GetStdId,
//		.drv_id = RM3508_GetDrvId,
		.tx_data = CAN_SendSingleData,
	},
	[CHAS_F] = {
		.type = DRV_CAN1,
		.can_id = CHASSIS_CAN_ID_F,
//		.std_id = RM3508_GetStdId,
//		.drv_id = RM3508_GetDrvId,
		.tx_data = CAN_SendSingleData,
	},
	[CHAS_B] = {
		.type = DRV_CAN1,
		.can_id = CHASSIS_CAN_ID_B,
//		.std_id = RM3508_GetStdId,
//		.drv_id = RM3508_GetDrvId,
		.tx_data = CAN_SendSingleData,
	},
};

// 底盘电机信息
chassis_motor_info_t 	chassis_motor_info[] = {
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
	{
		.offline_max_cnt = 50,
	},
};

// 底盘电机传感器
chassis_motor_t		chassis_motor[] = {
	[CHAS_L] = {
		.info = &chassis_motor_info[CHAS_L],
		.driver = &chassis_motor_driver[CHAS_L],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.heart_beat = chassis_motor_heart_beat,
		.work_state = DEV_OFFLINE,
		.id = DEV_ID_CHASSIS_L,
	},
	[CHAS_R] = {
		.info = &chassis_motor_info[CHAS_R],
		.driver = &chassis_motor_driver[CHAS_R],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.heart_beat = chassis_motor_heart_beat,
		.work_state = DEV_OFFLINE,
		.id = DEV_ID_CHASSIS_R,
	},
	[CHAS_F] = {
		.info = &chassis_motor_info[CHAS_F],
		.driver = &chassis_motor_driver[CHAS_F],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.heart_beat = chassis_motor_heart_beat,
		.work_state = DEV_OFFLINE,
		.id = DEV_ID_CHASSIS_F,
	},
	[CHAS_B] = {
		.info = &chassis_motor_info[CHAS_B],
		.driver = &chassis_motor_driver[CHAS_B],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.heart_beat = chassis_motor_heart_beat,
		.work_state = DEV_OFFLINE,
		.id = DEV_ID_CHASSIS_B,
	},
};

/* Private functions ---------------------------------------------------------*/
static void chassis_motor_check(chassis_motor_t *motor)
{
	int16_t err;
	chassis_motor_info_t *motor_info = motor->info;
	
	/* 未初始化 */
	if( !motor_info->init_flag )
	{
		motor_info->init_flag = true;
		motor_info->angle_prev = motor_info->angle;
		motor_info->angle_sum = 0;
	}
	
	err = motor_info->angle - motor_info->angle_prev;
	
	/* 过零点 */
	if(abs(err) > 4095)
	{
		/* 0↓ -> 8191 */
		if(err >= 0)
			motor_info->angle_sum += -8191 + err;
		/* 8191↑ -> 0 */
		else
			motor_info->angle_sum += 8191 + err;
	}
	/* 未过零点 */
	else
	{
		motor_info->angle_sum += err;
	}
	
	motor_info->angle_prev = motor_info->angle;		
}

static void chassis_motor_heart_beat(chassis_motor_t *motor)
{
	chassis_motor_info_t *motor_info = motor->info;
	
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

/* Exported functions --------------------------------------------------------*/

