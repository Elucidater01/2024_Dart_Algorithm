/**
  ******************************************************************************
  * @file    motor.c
  * @brief   电机驱动
  ******************************************************************************
  * @attention
  * typedef struct
  * {
  *  	can_id_t    id;				// CAN1或CAN2
  * 	uint32_t	rx_id;  		// 反馈报文标识符
  * 	uint32_t	tx_id;  		// 上传报文标识符
  * 	uint8_t		data_idx;		// 数组索引
  * 	uint8_t		*CANx_XXX_DATA; // 发送的数组
  * } drv_can_t;
  * 3508最大电流16384
  * 6020最大电压25000
  * 2006最大电流10000
  * 最大电流不应超过额定值
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/* Private variables ---------------------------------------------------------*/
/**
 * @brief 电机驱动信息
 */
drv_can_t rm_motor_driver[] = {
	[BELT_R] = {
		.id = DRV_CAN1,
		.rx_id = 0x201,
		.tx_id = 0x200,
		.data_id = 0,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[WIND] = {
		.id = DRV_CAN1,
		.rx_id = 0x206,
		.tx_id = 0x1FF,
		.data_id = 2,
		.CANx_XXX_DATA = CAN1_1FF_DATA,
	},
	[BELT_L] = {
		.id = DRV_CAN1,
		.rx_id = 0x203,
		.tx_id = 0x200,
		.data_id = 4,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[DIAL] = {
		.id = DRV_CAN1,
		.rx_id = 0x204,
		.tx_id = 0x200,
		.data_id = 6,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[AXIS_X] = {
		.id = DRV_CAN2,
		.rx_id = 0x205,
		.tx_id = 0x1FF,
		.data_id = 0,
		.CANx_XXX_DATA = CAN2_1FF_DATA,
	},
    [AXIS_ZL] = {
		.id = DRV_CAN2,
		.rx_id = 0x206,
		.tx_id = 0x1FF,
		.data_id = 2,
		.CANx_XXX_DATA = CAN2_1FF_DATA,
	},
	[AXIS_ZR] = {
		.id = DRV_CAN2,
		.rx_id = 0x207,
		.tx_id = 0x1FF,
		.data_id = 4,
		.CANx_XXX_DATA = CAN2_1FF_DATA,
	},
	[AXIS_Y] = {
		.id = DRV_CAN2,
		.rx_id = 0x208,
		.tx_id = 0x1FF,
		.data_id = 6,
		.CANx_XXX_DATA = CAN2_1FF_DATA,
	},
};

/**
 * @brief 电机pid参数
 */
motor_pid_t motor_pid[] = {
	[AXIS_X] = {
		.speed.kp = 15,
		.speed.ki = 0.3,
		.speed.kd = 0,
		.speed.integral_max = 6000,
		.speed.out_max = 8000,
		.angle.kp = 0.3,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 4000,
	},
	[AXIS_ZL] = {
		.speed.kp = 11,
		.speed.ki = 0.1,
		.speed.kd = 0,
		.speed.integral_max = 6000,
		.speed.out_max = 3000,
		.angle.kp = 0.3,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 4000,
	},
	[AXIS_ZR] = {
		.speed.kp = 11,
		.speed.ki = 0.1,
		.speed.kd = 0,
		.speed.integral_max = 6000,
		.speed.out_max = 3000,
		.angle.kp = 0.3,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 4000,
	},
	[BELT_L] = {
		.speed.kp = 11,
		.speed.ki = 0.1,
		.speed.kd = 0,
		.speed.integral_max = 6000,
		.speed.out_max = 7000,
		.angle.kp = 0.3,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 7000,
	},
	[BELT_R] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 16000,
		.speed.out_max = 16000,
		.angle.kp = 0.17,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 7200,
	},
	[DIAL] = {
		.speed.kp = 11,
		.speed.ki = 0.02,
		.speed.kd = 0,
		.speed.integral_max = 5000,
		.speed.out_max = 8000,
		.angle.kp = 0.15,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 3800,
	},
	[WIND] = {
		.speed.kp = 350,
		.speed.ki = 0.25,
		.speed.kd = 0,
		.speed.integral_max = 15000,
		.speed.out_max = 24000,
		.angle.kp = 0.4,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 0,
		.angle.out_max = 50,
	},
	[AXIS_Y] = {
		.speed.kp = 11,
		.speed.ki = 0.1,
		.speed.kd = 0,
		.speed.integral_max = 6000,
		.speed.out_max = 9000,
		.angle.kp = 0.3,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 1000,
		.angle.out_max = 5000,
	},
};

/**
 * @brief 电机信息
 */
rm_motor_info_t rm_motor_info[] = {
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
	{
        .init_flag = false,
        .offline_max_cnt = 50,
    },
	{
        .init_flag = false,
        .offline_max_cnt = 50,
    },
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },

};

/**
 * @brief 电机结构体
 */
rm_motor_t rm_motor[] = {
	[AXIS_X] = {
        .info = &rm_motor_info[AXIS_X],
        .driver = &rm_motor_driver[AXIS_X],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[AXIS_ZL] = {
        .info = &rm_motor_info[AXIS_ZL],
        .driver = &rm_motor_driver[AXIS_ZL],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[AXIS_ZR] = {
        .info = &rm_motor_info[AXIS_ZR],
        .driver = &rm_motor_driver[AXIS_ZR],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[BELT_L] = {
        .info = &rm_motor_info[BELT_L],
        .driver = &rm_motor_driver[BELT_L],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
		.cnt = 0,
	},
	[BELT_R] = {
        .info = &rm_motor_info[BELT_R],
        .driver = &rm_motor_driver[BELT_R],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
		.cnt = 0,
	},
	[DIAL] = {
        .info = &rm_motor_info[DIAL],
        .driver = &rm_motor_driver[DIAL],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
		.cnt = 0,
	},
	[WIND] = {
        .info = &rm_motor_info[WIND],
        .driver = &rm_motor_driver[WIND],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
		.cnt = 0,
	},
	[AXIS_Y] = {
        .info = &rm_motor_info[AXIS_Y],
        .driver = &rm_motor_driver[AXIS_Y],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
		.cnt = 0,
	},
};

/* Exported functions --------------------------------------------------------*/
/**
 * @brief 电机初始化
 * @param  无
 * @retval 无
 */
void motor_list_init()
{
	rm_motor_init(&rm_motor[AXIS_X], motor_pid[AXIS_X]);
	rm_motor_init(&rm_motor[AXIS_ZL], motor_pid[AXIS_ZL]);
	rm_motor_init(&rm_motor[AXIS_ZR], motor_pid[AXIS_ZR]);
	rm_motor_init(&rm_motor[BELT_L], motor_pid[BELT_L]);
	rm_motor_init(&rm_motor[BELT_R], motor_pid[BELT_R]);
	rm_motor_init(&rm_motor[DIAL], motor_pid[DIAL]);
	rm_motor_init(&rm_motor[WIND], motor_pid[WIND]);
	rm_motor_init(&rm_motor[AXIS_Y], motor_pid[AXIS_Y]);
}

/**
 * @brief 电机心跳失联检测
 * @param  无
 * @retval 无
 */
void motor_list_heart_beat()
{
	rm_motor_heart_beat(&rm_motor[AXIS_X]);
	rm_motor_heart_beat(&rm_motor[AXIS_ZL]);
	rm_motor_heart_beat(&rm_motor[AXIS_ZR]);
	rm_motor_heart_beat(&rm_motor[BELT_L]);
	rm_motor_heart_beat(&rm_motor[BELT_R]);
	rm_motor_heart_beat(&rm_motor[DIAL]);
	rm_motor_heart_beat(&rm_motor[WIND]);
	rm_motor_heart_beat(&rm_motor[AXIS_Y]);
}

/**
 * @brief 电机工作状态
 * @param  无
 * @retval 全部在线返回1，否则返回0
 */
uint8_t motor_list_workstate()
{
	if(rm_motor[BELT_R].work_state == DEV_ONLINE
        && rm_motor[BELT_L].work_state == DEV_ONLINE
        && rm_motor[DIAL].work_state == DEV_ONLINE
        && rm_motor[WIND].work_state == DEV_ONLINE
        && rm_motor[AXIS_X].work_state == DEV_ONLINE
        && rm_motor[AXIS_Y].work_state == DEV_ONLINE
        && rm_motor[AXIS_ZL].work_state == DEV_ONLINE
        && rm_motor[AXIS_ZR].work_state == DEV_ONLINE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
