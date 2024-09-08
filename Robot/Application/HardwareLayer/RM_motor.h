#ifndef __RM_MOTOR_H
#define __RM_MOTOR_H
#include "rp_config.h"
#include "pid.h"
/**
 *	@brief	RM标准电机信息与结构体
 *	@class	device
 */
typedef struct rm_motor_info_struct {
	volatile uint16_t	angle;
	volatile int16_t    speed;
	volatile int16_t	current;
    volatile uint8_t    temperature;
	volatile uint16_t	angle_prev;
	volatile int32_t	angle_sum;
	volatile uint8_t	init_flag;
	volatile uint8_t	offline_cnt;
	const    uint8_t	offline_max_cnt;	
} rm_motor_info_t;

typedef struct {
	pid_ctrl_t	speed;
	pid_ctrl_t	angle;
} motor_pid_t;

typedef struct rm_motor_struct {
	rm_motor_info_t 	        *info;
	drv_can_t				    *driver;
	motor_pid_t                 motor_pid;
	int16_t                     cnt;
	bool                        flag;
	void					    (*init)(struct rm_motor_struct *self, motor_pid_t motor_pid);
	void					    (*update)(struct rm_motor_struct *self, uint8_t *rxBuf);
	void					    (*check)(struct rm_motor_struct *self);	
	void					    (*heart_beat)(struct rm_motor_struct *self);
	volatile dev_work_state_t   work_state;
	volatile dev_errno_t	    errno;
} rm_motor_t;

void Motor_ToSpeed(rm_motor_t *motor, int16_t speed);
void Motor_ToAngle(rm_motor_t *motor, int16_t angle);
void Motor_SendData(drv_can_t *motor, int16_t data);

void rm_motor_init(rm_motor_t *motor, motor_pid_t motor_pid);
void rm_motor_heart_beat(rm_motor_t *motor);

#endif

