#ifndef __SHOOT_H
#define __SHOOT_H

#include "rp_config.h"
#include "rp_math.h"

#include "gimbal.h"
#include "rc_sensor.h"
//#include "key.h"
#include "judge.h"

#define Fric_15 4400.0f
#define Fric_18 4700.0f
#define Fric_20 4950.0f
#define Fric_22 5200.0f
#define Fric_30 7020.0f

// Ħ����״̬ö��
typedef enum
{
	On_Fric,        // ����
	Off_Fric,       // �ر�
	WaitCommond_Fric // �ȴ�ָ��
} fric_status_e;

// ����״̬ö��
typedef enum
{
	Reload_Dial,   // ����
	F_Lock_Dial,   // ���򿨵�
//	Unload_Dial,   // �˵�
//	B_Lock_Dial ,  // ���򿨵�
	WaitCommond_Dial // �ȴ�ָ��
} dial_status_e;

// �������ָ��ö��
typedef enum
{
	Fric_Toggle,    // ����Ħ����
	Magz_Open,      // ������
	Magz_Close,     // �ص���
	Func_Reset,     // ���ܸ�λ
	Keep_Shoot,     // ����
	Single_Shoot,   // ����
	WaitCommond_L // �ȴ�ָ��
} launcher_commond_e;

typedef struct 
{
//	motor_3508_t  *fric_left;
//	motor_3508_t  *fric_right;
//	motor_2006_t  *dial_motor;
	rc_sensor_t		*rc_sensor;
} shoot_dev_t;

typedef struct
{
	remote_mode_t	 remote_mode;
	int16_t  measure_left_speed;
	int16_t  measure_right_speed;
	int16_t  measure_dial_speed;
	float    measure_dial_angle;
	
	float    limit_speed;
	float    measure_shoot_speed;
	uint16_t limit_heat;
	uint16_t measure_shoot_heat;
	
	float  target_left_speed;
	float  target_right_speed;
	float  target_dial_speed;
	float  target_dial_angle;
	
	uint8_t  init_s2;
	uint8_t  last_s2;
	
	dev_work_state_t  rc_work_state;
} shoot_info_t;

typedef struct
{
 	launcher_commond_e  launcher_commond;
	fric_status_e       fric_status;
	dial_status_e       dial_status;
	uint16_t            lock_cnt;
} shoot_work_info_t;

typedef struct
{
	float    fric_speed;
	float    dial_speed;
	float    lock_angle_check;
	int16_t  lock_cnt;
	float    Back_Angle;
	float    Load_Angle;
} shoot_conf_t;

typedef struct 
{
	shoot_dev_t        *dev;
	shoot_info_t       *info;
	shoot_work_info_t  *work_info;
	shoot_conf_t       *conf;
	void			(*init)(void);
	void			(*ctrl)(void);
	void			(*self_protect)(void);
} launcher_t;

extern launcher_t launcher;

//void Shoot_GetInfo(void);
//void Shoot_Stop(void);
//void Shoot_RcCtrl(void);
//void Shoot_PidCtrl(void);

#endif
