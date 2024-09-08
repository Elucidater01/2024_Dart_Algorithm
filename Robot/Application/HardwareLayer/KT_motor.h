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

	
	//W��ʾд��R��ʾ����cmd������
	
	void (*get_info)(struct KT_motor_class_t *motor, uint8_t *rxBuf);
	void (*tx_W_cmd)(struct KT_motor_class_t *motor, uint8_t command);   //����д����
	void (*tx_R_cmd)(struct KT_motor_class_t *motor, uint8_t command);		//����������ȡ��Ϣ����
	
	
	//�����ǻ������ܺ���������д����ķ��ͽṹ���еĲ������ڲ����߼����Ʒ�Χ����
	
	void (*W_pid)(struct KT_motor_class_t *motor, uint8_t *buff);
	void (*W_accel)(struct KT_motor_class_t *motor, int32_t accel);
	void (*W_encoderOffset)(struct KT_motor_class_t *motor, uint16_t encoderOffset);
	void (*W_powerControl)(struct KT_motor_class_t *motor, int16_t powerControl);//��������
	void (*W_iqControl)(struct KT_motor_class_t *motor, int16_t iqControl);//�ջ�Ť��/����
	void (*W_speedControl)(struct KT_motor_class_t *motor, int32_t speedControl);//�ջ��ٶ�
	void (*W_angle_sum_Control)(struct KT_motor_class_t      *motor, 
															int32_t                      angle_sum_Control,
	                            uint16_t                     angle_sum_Control_maxSpeed);//�ջ���Ȧ�Ƕ�
	void (*W_angle_single_Control)(struct KT_motor_class_t   *motor, 
																 uint16_t                  angle_single_Control,
													       uint8_t   	               angle_single_Control_spinDirection,
																 uint16_t			             angle_single_Control_maxSpeed);//�ջ���Ȧ�Ƕ�
	void (*W_angle_add_Control)(struct KT_motor_class_t      *motor, 
														  int32_t                      angle_add_Control,
															uint16_t                     angle_add_Control_maxSpeed);//�ջ��Ƕ�����
	
	
}KT_motor_t;



void KT_motor_class_init(KT_motor_t *motor);
void kt_motor_multi_control(int16_t* iqControl, char kt_motor_num, motor_drive_e drive_type);


#endif
