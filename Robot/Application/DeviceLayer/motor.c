#include "motor.h"
/*
typedef struct drv_can {
    can_id_t    id;				// CAN1��CAN2
	uint32_t	rx_id;  		// �������ı�ʶ��
	uint32_t	tx_id;  		// �ϴ����ı�ʶ��
	uint8_t		data_idx;		// ��������
    uint16_t    tx_period;  	// ��ʱ���ͼ��(ms)
	uint8_t		*CANx_XXX_DATA; // ���͵�����
} drv_can_t;
*/
/*
	3508������ת��Ϊ482*19���ٶ����ٱ�19��= 9158,������16384
	6020������ת��Ϊ320������ѹ30000
	2006������ת��Ϊ500*36 = 18000��������10000
	���ת�ٲ�Ӧ�����ֵ
*/

drv_can_t motor_driver[] = {
	[CHAS_LF] = {
		.id = DRV_CAN1,
		.rx_id = 0x201,
		.tx_id = 0x200,
		.data_id = 0,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
    [CHAS_RF] = {
		.id = DRV_CAN1,
		.rx_id = 0x202,
		.tx_id = 0x200,
		.data_id = 2,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[CHAS_RB] = {
		.id = DRV_CAN1,
		.rx_id = 0x203,
		.tx_id = 0x200,
		.data_id = 4,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[CHAS_LB] = {
		.id = DRV_CAN1,
		.rx_id = 0x204,
		.tx_id = 0x200,
		.data_id = 6,
		.CANx_XXX_DATA = CAN1_200_DATA,
	},
	[GIMB_Y] = {
		.id = DRV_CAN1,
		.rx_id = 0x205,
		.tx_id = 0x1FF,
		.data_id = 0,
		.CANx_XXX_DATA = CAN1_1FF_DATA,
	},
	[GIMB_P] = {
		.id = DRV_CAN1,
		.rx_id = 0x206,
		.tx_id = 0x1FF,
		.data_id = 2,
		.CANx_XXX_DATA = CAN1_1FF_DATA,
	},
	[FRIC_L] = {
		.id = DRV_CAN2,
		.rx_id = 0x201,
		.tx_id = 0x200,
		.data_id = 0,
		.CANx_XXX_DATA = CAN2_200_DATA,
	},
	[FRIC_R] = {
		.id = DRV_CAN2,
		.rx_id = 0x202,
		.tx_id = 0x200,
		.data_id = 2,
		.CANx_XXX_DATA = CAN2_200_DATA,
	},
	[DIAL] = {
		.id = DRV_CAN2,
		.rx_id = 0x203,
		.tx_id = 0x200,
		.data_id = 4,
		.CANx_XXX_DATA = CAN2_200_DATA,
	},
};

motor_pid_t motor_pid[] = {
	[CHAS_LF] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[CHAS_RF] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[CHAS_RB] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[CHAS_LB] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[GIMB_Y] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[GIMB_P] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[FRIC_L] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[FRIC_R] = {
		.speed.kp = 11,
		.speed.ki = 0.4,
		.speed.kd = 0,
		.speed.integral_max = 10000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0.008,
		.angle.integral_max = 0,
		.angle.out_max = 8000,
	},
	[DIAL] = {
		.speed.kp = 18,
		.speed.ki = 0,
		.speed.kd = 0,
		.speed.integral_max = 1000,
		.speed.out_max = 8000,
		.angle.kp = 0.55,
		.angle.ki = 0,
		.angle.kd = 0,
		.angle.integral_max = 1000,
		.angle.out_max = 15000,
	},
};

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
    {
        .init_flag = false,
        .offline_max_cnt = 50,
    },
};

rm_motor_t rm_motor[] = {
	[CHAS_LF] = {
        .info = &rm_motor_info[CHAS_LF],
        .driver = &motor_driver[CHAS_LF],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[CHAS_RF] = {
        .info = &rm_motor_info[CHAS_RF],
        .driver = &motor_driver[CHAS_RF],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[CHAS_RB] = {
        .info = &rm_motor_info[CHAS_RB],
        .driver = &motor_driver[CHAS_RB],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[CHAS_LB] = {
        .info = &rm_motor_info[CHAS_LB],
        .driver = &motor_driver[CHAS_LB],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[GIMB_Y] = {
        .info = &rm_motor_info[GIMB_Y],
        .driver = &motor_driver[GIMB_Y],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[GIMB_P] = {
        .info = &rm_motor_info[GIMB_P],
        .driver = &motor_driver[GIMB_P],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[FRIC_L] = {
        .info = &rm_motor_info[FRIC_L],
        .driver = &motor_driver[FRIC_L],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[FRIC_R] = {
        .info = &rm_motor_info[FRIC_R],
        .driver = &motor_driver[FRIC_R],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,
	},
	[DIAL] = {
        .info = &rm_motor_info[DIAL],
        .driver = &motor_driver[DIAL],
        .init = rm_motor_init,
        .update = rm_motor_update,
        .check = rm_motor_check,
        .heart_beat = rm_motor_heart_beat,
        .work_state = DEV_OFFLINE,

	},
};

