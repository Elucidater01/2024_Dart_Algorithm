/**
 * @file        rp_user_config.h
 * @author      RobotPilots
 * @Version     v1.0
 * @brief       RobotPilots Robots' User Configuration.
 * @update
 *              v1.0(7-November-2021)  
 */
#ifndef __RP_USER_CONFIG_H
#define __RP_USER_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* Exported macro ------------------------------------------------------------*/
#define EVENT_SEND_CAN1_MAILBOX (0x1UL << 0)
#define EVENT_SEND_CAN2_MAILBOX (0x1UL << 1)
/* Exported types ------------------------------------------------------------*/
/* Ӧ�ò� --------------------------------------------------------------------*/
/**
 *	@brief	pid������
 *	@class	controller
 */
typedef struct pid_ctrl {
	float	target;
	float	measure;
	float 	err;
	float 	last_err;
	float	kp;
	float 	ki;
	float 	kd;
	float 	pout;
	float 	iout;
	float 	dout;
	float 	out;
	float	integral;
	float 	integral_max;
	float 	out_max;
} pid_ctrl_t;

/* Remote Mode Enum */
typedef enum {
	RC = 0,
	KEY = 1,
	REMOTE_MODE_CNT = 2,
} remote_mode_t;

typedef enum {
	SYS_STATE_NORMAL,	// ϵͳ����
	SYS_STATE_RCLOST,	// ң��ʧ��
	SYS_STATE_RCERR,	// ң�س���
	SYS_STATE_WRONG,	// ����ϵͳ����
} sys_state_t;

typedef enum {
	SYS_MODE_NORMAL,	// ����ģʽ
	SYS_MODE_CNT,
} sys_mode_t;

//typedef struct {
//	struct {
//		uint8_t reset_start;
//		uint8_t reset_ok;
//	}gimbal;
//} flag_t;

typedef struct {
	remote_mode_t		remote_mode;	// ���Ʒ�ʽ
	sys_state_t			state;			// ϵͳ״̬
	sys_mode_t			mode;			// ϵͳģʽ
} system_t;

//extern flag_t	flag;
extern system_t sys;

#endif
