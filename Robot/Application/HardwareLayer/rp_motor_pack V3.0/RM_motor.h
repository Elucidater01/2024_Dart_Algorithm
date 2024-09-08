#ifndef __RM_motor_H
#define __RM_motor_H


#include "motor_def.h"


#define m_abs(x) 					((x)>0? (x):(-(x)))
#define m_constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define m_anti_constrain(x, min, max)	(((x<max) && (x>min))?(0):x)




typedef struct RM_motor_class_t
{
	RM_motor_rx_info_t        rx_info;
	RM_motor_state_info_t     state;
	RM_motor_id_info_t        id;
  RM_motor_mec_info_t       mec_info;
	RM_motor_pid_all_t        pid;
  RM_motor_base_info_t			base_info;
	
	void (*init)(struct RM_motor_class_t *motor);
	void (*heartbeat)(struct RM_motor_class_t *motor);
  void (*pid_init)(RM_motor_pid_t *pid, float *buff);	
	
	void (*tx)(struct RM_motor_class_t *motor, int16_t *buff,uint8_t len);
	void (*rx)(struct RM_motor_class_t *motor, uint8_t *buff);
	
	/*以下为控制函数*/
	float (*ctr_posit)(struct RM_motor_class_t *motor,float target);
	float (*ctr_angle)(struct RM_motor_class_t *motor,float target);	
	float (*ctr_speed)(struct RM_motor_class_t *motor,float target);	
	float (*ctr_pid2)(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode);	
	float (*ctr_pid1)(RM_motor_pid_t *out, float meas1, float tar);	
	
	void  (*ctr_judge_dir)(struct RM_motor_class_t *motor,uint16_t range);	
  void  (*ctr_offset)(struct RM_motor_class_t *motor, uint16_t range);	
	
  uint8_t (*ctr_stuck_flag)(struct RM_motor_class_t *motor, float torque_limit);	
	
}RM_motor_t;




void motor_class_init(struct RM_motor_class_t *motor);
void motor_class_heartbeat(struct RM_motor_class_t *motor);
void motor_class_pid_init(RM_motor_pid_t *pid, float *buff);
uint8_t motor_class_stucking_flag(struct RM_motor_class_t *motor, float torque_limit);

void get_rm_can_drvie(struct RM_motor_class_t *motor);
void get_rm_info(struct RM_motor_class_t *motor, uint8_t *rxBuf);


float motor_cycle(float tar,float cycle);
float motor_half_cycle(float angle,float max);
void  motor_judge_dir(struct RM_motor_class_t *motor,uint16_t range);
void  motor_offset(struct RM_motor_class_t *motor, uint16_t range);

float motor_pid_err(RM_motor_pid_t *pid,float measure);
float motor_pid_cal(RM_motor_pid_t *pid);
float motor_pid_ctrl(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, char err_cal_mode);

float motor_pid_position(struct RM_motor_class_t *motor,float target);
float motor_pid_angle(struct RM_motor_class_t *motor,float target);
float motor_pid_speed(struct RM_motor_class_t *motor,float target);

float motor_pid_double(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode);
float motor_pid_single(RM_motor_pid_t *out, float meas1, float tar);

void CAN1_Motor_RxCallback(uint32_t canId, uint8_t *rxBuf);

#endif



