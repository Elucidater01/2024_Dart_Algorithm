#ifndef __CHASSIS_H
#define __CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"
#include "rm_protocol.h"

#include "device.h"
#include "module.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
// ����ģʽö��
typedef enum
{
	CHASSIS_MODE_NORMAL 		= 0, // ����ģʽ
	CHASSIS_MODE_BUFF   		= 1, // ���ģʽ
	CHASSIS_MODE_RELOAD_BULLET	= 2, // ���̵��ٲ���ģʽ
	CHASSIS_MODE_GYRO       = 3, // С����ģʽ
} chassis_mode_e;

typedef enum
{
	CHASSIS_FORWARD,
	CHASSIS_BACKWARD,
}	chassis_dir_e;

typedef enum 
{
	CHAS_LOCK,
	CHAS_UNLOCK,
} chassis_lock_e;

typedef struct 
{
	//RM_motor_t    *chas_motor[4];
	imu_sensor_t	*imu_sensor;
	rc_sensor_t		*rc_sensor;
} chassis_dev_t;

typedef struct
{
	remote_mode_t		    remote_mode;
	chassis_mode_e      chassis_mode;
	chassis_dir_e				chassis_dir;
	chassis_lock_e			chassis_lock;
	
	int16_t  target_x_speed;  //Ŀ��ǰ���ٶ�
	int16_t  target_y_speed;  //Ŀ�������ٶ�
	int16_t  target_z_speed;  //Ŀ����ת�ٶ�
	
	int16_t  measure_x_speed; //��̨����ǰ���ٶ�
	int16_t  measure_y_speed; //��̨���������ٶ�
	int16_t  measure_z_speed; //��ǰ��ת�ٶ�
	
	int16_t  measure_chas_x_speed; //���̳���ǰ���ٶ�
	int16_t  measure_chas_y_speed; //���̳��������ٶ�
	
	float		 wheel_set[4];
	float		 wheel_real[4];
	
	float		 yaw_rad_err;
	int16_t  yaw_motor_angle_err;			 //yaw��Ƕ����
	int16_t  last_yaw_motor_angle_err; //��һ��yaw��Ƕ����
} chassis_info_t;

typedef struct
{
	float  limit_speed;				// ������ת��
	float  chassis_follow_kp; // ���̸�����̨���ٶ�ϵ��
	float  gyro_round_speed; 	// С�������ת��
	float	 key_z_offset;
} chassis_conf_t;

typedef struct 
{
	chassis_dev_t    *dev;
	chassis_info_t   *info;
	chassis_conf_t   *conf;
	void			(*init)(void);
	void			(*ctrl)(void);
	void			(*self_protect)(void);
} chassis_t;

extern chassis_t chassis;

/* Exported functions --------------------------------------------------------*/

#endif
