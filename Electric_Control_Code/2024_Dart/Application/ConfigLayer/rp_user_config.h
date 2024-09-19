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

#define IS_MASTER (1)
/* Exported types ------------------------------------------------------------*/

/* Ӧ�ò� --------------------------------------------------------------------*/
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

typedef struct {
	uint8_t reset_start;
	uint8_t reset_ok;
	uint8_t turn_start;
	uint8_t turn_ok;
	uint8_t forward; // 1:ͷΪ�� 0:βΪ��
	uint8_t turn_right;
	uint8_t turn_left;
} gimbal_symbal_t;

typedef struct __symbal_struct
{
	gimbal_symbal_t   gim_sym;
	uint8_t 					rc_update;
	uint8_t						slave_reset;
} symbal_t;

typedef struct {
	remote_mode_t		remote_mode;	// ���Ʒ�ʽ
	sys_state_t			state;			// ϵͳ״̬
	sys_mode_t			mode;			// ϵͳģʽ
} system_t;

extern symbal_t	symbal;
extern system_t sys;

#endif
