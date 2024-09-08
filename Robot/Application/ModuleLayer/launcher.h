/**
 * @file        launcher.c/.h
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        20-November-2022
 * @brief       Launcher Control Center
 */

#ifndef __LAUNCHER_H
#define __LAUNCHER_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rm_protocol.h"

#include "device.h"
#include "module.h"

/* Exported macro ------------------------------------------------------------*/
#define Fric_15_init 4400.0f
#define Fric_18_init 4870.0f
#define Fric_20_init 4950.0f
#define Fric_22_init 5200.0f
#define Fric_30_init 7020.0f

/* Exported types ------------------------------------------------------------*/
// �������ģʽö��
typedef enum
{
	lch_keep,      // ����
	lch_gyro,      // ������
	lch_gyro2,     // ������
	lch_machine,   // ��е
} launcher_mode_e;

// Ħ����״̬ö��
typedef enum
{
	WaitCommond_Fric, // �ȴ�ָ��
	On_Fric,        // ����
	Off_Fric,       // �ر�
} fric_status_e;

// ����״̬ö��
typedef enum
{
	WaitCommond_Dial,// �ȴ�ָ��
	Reload_Dial,     // ����
	Lock_Dial,    	 // ����
	SpeedKeep_Dial,  // �ٶȱ���
	SwiftKeep_Dial,  // ���䱣��
//	Unload_Dial,   // �˵�
//	B_Lock_Dial ,  // ���򿨵�
} dial_status_e;

/*  Ħ����ָ��ö��  */
typedef enum {
	Fric_Reset,  	  // ���ܸ�λ
	Fric_Open,   		// ��Ħ����
	Fric_Close,   	// ��Ħ����
	Fric_Toggle,		// ����Ħ����
} Fric_cmd_e;

/*  ����ָ��ö��  */
typedef enum {
	Magz_Reset,  	  // ���ܸ�λ
	Magz_Open,      // ������
	Magz_Close,     // �ص���
} Magz_cmd_e;

/*  ����ָ��ö��  */
typedef enum {
	Shoot_Reset,  	// ���ܸ�λ
	Single_Shoot,   // ����
	Keep_Shoot,     // ����
	Swift_Shoot,		// ����
} Dial_cmd_e;

// �������ָ��ö��
typedef struct
{
	Fric_cmd_e  Fric_cmd;
	Magz_cmd_e	Magz_cmd;
	Dial_cmd_e	Dial_cmd;
} launcher_commond_t;

typedef struct 
{
	/*RM_motor_t    *fricl_motor;
	RM_motor_t    *fricr_motor;
	RM_motor_t    *dial_motor;*/
	rc_sensor_t		*rc_sensor;
} launcher_dev_t;

typedef struct
{
	remote_mode_t	 	remote_mode;
	launcher_mode_e launcher_mode;
	
	int16_t  measure_left_speed;
	int16_t  measure_right_speed;
	int16_t  measure_dial_speed;
	float    measure_dial_angle;
	
	float    limit_speed;
	float    measure_launcher_speed;
	uint16_t limit_heat;
	uint16_t measure_launcher_heat;
	uint16_t remain_heat;
	
	float  target_left_speed;
	float  target_right_speed;
	float  target_dial_speed;
	float  target_dial_angle;
	float  target_heat_angle;
	
	uint8_t  vision_shoot_cnt;
	uint8_t  last_vision_shoot_cnt;
	uint8_t  shoot_cnt;
	
	uint8_t  last_s1;
	uint8_t  init_s2;
	uint8_t  last_s2;
	
	dev_work_state_t  rc_work_state;
} launcher_info_t;

typedef struct
{
 	launcher_commond_t  launcher_commond;
	fric_status_e       fric_status;
	dial_status_e       dial_status;
	uint16_t            shoot_cnt;
	uint8_t							swift_enable;
} launcher_work_info_t;

typedef struct
{
	float 	 Fric_15;
	float 	 Fric_18;
	float 	 Fric_20;
	float 	 Fric_22;
	float 	 Fric_30;
	float    fric_speed;
	uint8_t  fric_mode;
	float    dial_speed;
	float    dial_swiftspeed;
	float    dial_torque_limit;
	float    lock_angle_check;
	float    Back_Angle;
	float    Load_Angle;
	uint16_t wait_time;
} launcher_conf_t;

typedef struct 
{
	launcher_dev_t        *dev;
	launcher_info_t       *info;
	launcher_work_info_t  *work_info;
	launcher_conf_t       *conf;
	void			(*init)(void);
	void			(*ctrl)(void);
	void			(*self_protect)(void);
} launcher_t;

extern launcher_t launcher;

/* Exported functions --------------------------------------------------------*/
//void Launcher_GetInfo(void);
//void Launcher_Stop(void);
//void Launcher_RcCtrl(void);
//void Launcher_PidCtrl(void);

#endif
