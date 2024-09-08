#ifndef __KT_motor_H
#define __KT_motor_H

#include "stm32f4xx.h"                  // Device header

#include "driver.h"
#include "motor_def.h"
#include "algo.h"


typedef struct KT_motor_class_t
{
	KT_motor_info_t  KT_motor_info;
	uint8_t          tx_buff[8];
	
	void (*init)(struct KT_motor_class_t *motor);
	void (*heartbeat)(struct KT_motor_class_t *motor);
	void (*pid_init)(struct KT_motor_class_t *motor);

	
	//W表示写，R表示读，cmd是命令
	
	void (*get_info)(struct KT_motor_class_t *motor, uint8_t *rxBuf);
	void (*tx_W_cmd)(struct KT_motor_class_t *motor, uint8_t command);   //发送写命令
	void (*tx_R_cmd)(struct KT_motor_class_t *motor, uint8_t command);		//发送主动读取信息命令
	
	
	//下面是基本功能函数，负责写电机的发送结构体中的参数，内部有逻辑限制范围操作
	
	void (*W_pid)(struct KT_motor_class_t *motor, uint8_t *buff);
	void (*W_accel)(struct KT_motor_class_t *motor, int32_t accel);
	void (*W_encoderOffset)(struct KT_motor_class_t *motor, uint16_t encoderOffset);
	void (*W_powerControl)(struct KT_motor_class_t *motor, int16_t powerControl);//开环功率
	void (*W_iqControl)(struct KT_motor_class_t *motor, int16_t iqControl);//闭环扭矩/电流
	void (*W_speedControl)(struct KT_motor_class_t *motor, int32_t speedControl);//闭环速度
	void (*W_angle_sum_Control)(struct KT_motor_class_t      *motor, 
															int32_t                      angle_sum_Control,
	                            uint16_t                     angle_sum_Control_maxSpeed);//闭环多圈角度
	void (*W_angle_single_Control)(struct KT_motor_class_t   *motor, 
																 uint16_t                  angle_single_Control,
													       uint8_t   	               angle_single_Control_spinDirection,
																 uint16_t			             angle_single_Control_maxSpeed);//闭环单圈角度
	void (*W_angle_add_Control)(struct KT_motor_class_t      *motor, 
														  int32_t                      angle_add_Control,
															uint16_t                     angle_add_Control_maxSpeed);//闭环角度增量
	
	
}KT_motor_t;



void KT_motor_class_init(KT_motor_t *motor);
void kt_motor_multi_control(int16_t* iqControl, char kt_motor_num, motor_drive_e drive_type);


#endif
