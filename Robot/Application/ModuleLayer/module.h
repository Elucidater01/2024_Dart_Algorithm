/**
 * @file        module.c/.h
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        5-January-2023
 * @brief       Module Status Control
 * @update
 */

#ifndef __MODULE_H
#define __MODULE_H

/* Includes ------------------------------------------------------------------*/
#include "device.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
	MODULE_STATE_RCLOST,	// ң��ʧ��
	MODULE_STATE_RCINIT,	// ң�س�ʼ��
	MODULE_STATE_IMUTMP,	// ������У׼�ٽ�״̬
	MODULE_STATE_IMUERR,	// �����Ǵ���
	MODULE_STATE_KILLED,	// �����ϵ�
	MODULE_STATE_NORMAL,	// �������
	MODULE_STATE_WRONG,	  // �������
} module_state_t;

typedef enum {
	MODULE_MODE_RESET,		// ģʽ��λ
	MODULE_MODE_MACHINE,	// ��еģʽ
	MODULE_MODE_GYRO,			// ������ģʽ
	MODULE_MODE_CNT,
} module_mode_t;

typedef enum {
	MODULE_DIR_ARMOR,		// ��̨����װ�װ�
	MODULE_DIR_WHEEL,		// ��̨��������
	MODULE_DIR_CNT,
} module_dir_t;

/*  �������״̬  */
typedef struct launcher_state_struct {
	fric_cmd_e		fric_state;
	magz_cmd_e		magz_state;
	dial_cmd_e		shoot_state;
} launcher_state_t;

/*  ����״̬  */
typedef enum
{
	chas_reset, 			// ���ܸ�λ
	gyro_on,        	// С���ݿ���
} chas_cmd_e;

/*  �����ٶ�״̬  */
typedef enum
{
	speed_reset, 			// ���ܸ�λ
	rapid_on,        	// ���ٿ���
	rapid_off,       	// ���ٹر�
} speed_cmd_e;

/*  ��������״̬  */
typedef enum
{
	heat_limit_on,     // ������������
	heat_limit_off,    // �ر���������
} heat_cmd_e;

/*  �����Զ���״̬  */
typedef enum
{
	auto_shoot_off,    // �ر��Զ���
	auto_shoot_on,     // �����Զ���
} auto_cmd_e;

/*  �����Զ����״̬  */
typedef enum
{
	auto_buff_off,		 // �ر��Զ����
	auto_buff_on,			 // �����Զ����
} autobuff_cmd_e;

/*  �����־λ  */
typedef enum
{
	buff_reset, 			// ���ܸ�λ
	small_buff_on,   	// С������
	big_buff_on,     	// �������
	debuff_on,				// ��������
} buff_cmd_e;

typedef struct {
	launcher_state_t	lch_state;
	gim_cmd_e					gim_cmd;
	gim_cmd_e					gim_state;
	chas_cmd_e				chas_cmd;
//	chas_cmd_e				chas_state;
	lch_cmd_t					lch_cmd;
	gim_mode_e				gim_mode;
	heat_cmd_e				heat_mode;
	speed_cmd_e				speed_cmd;
	auto_cmd_e				auto_cmd;
	autobuff_cmd_e		autobuff_cmd;
	buff_cmd_e				buff_cmd;
	uint8_t						tw_last_step[4];
} status_t;

typedef struct __module_t
{
	remote_mode_t		remote_mode;	// ���Ʒ�ʽ
	module_state_t	state;	// ���״̬
	module_mode_t		mode;	// ���ģʽ
	module_dir_t		dir;	// ��̨����
	symbal_t				*symbal;
	status_t				*status;
	
	void (*heartbeat)(struct __module_t *module);
} module_t;

extern status_t status;
extern module_t module;

/* Exported functions --------------------------------------------------------*/

#endif
