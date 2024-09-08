#ifndef __GIMBAL_H
#define __GIMBAL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"
#include "rm_protocol.h"

#include "device.h"
#include "module.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
// ��̨ģʽö��
typedef enum
{
	gim_gyro,      // ������
	gim_machine,   // ��е
	gim_back,			 // ��ͷ
	gim_keep,      // ����
	gim_vision,    // ����
} gimbal_mode_e;

typedef struct 
{
	/*RM_motor_t    *yaw_motor;
	RM_motor_t    *pitch_motor;*/
	imu_sensor_t	*imu_sensor;
	rc_sensor_t		*rc_sensor;
} gimbal_dev_t;

typedef struct
{
	remote_mode_t		     remote_mode;
	gimbal_mode_e        gimbal_mode;
	
	float    measure_yaw_imu_speed;
	float    measure_yaw_imu_angle;
	int16_t  measure_yaw_motor_speed;
	int16_t  measure_yaw_motor_angle;
	float    measure_pitch_imu_speed;
	float    measure_pitch_imu_angle;
	int16_t  measure_pitch_motor_speed;
	int16_t  measure_pitch_motor_angle;
	float    measure_roll_imu_speed;
	float    measure_roll_imu_angle;
	
//	float    measure_yaw_global_speed;
//	float    measure_yaw_global_angle;
//	float    measure_pitch_global_speed;
//	float    measure_pitch_global_angle;
//	float    target_yaw_global_speed;
//	float    target_yaw_global_angle;
//	float    target_yaw_global_deltaangle;
//	float    target_pitch_global_speed;
//	float    target_pitch_global_angle;
//	float    target_pitch_global_deltaangle;
	
	float    target_yaw_imu_angle;
	float    target_yaw_imu_deltaangle;
	int16_t  target_yaw_motor_angle;
	int16_t  target_yaw_motor_deltaangle;
	float    target_pitch_imu_angle;
	float    target_pitch_imu_deltaangle;
	int16_t  target_pitch_motor_angle;
	int16_t  target_pitch_motor_deltaangle;
	
	int16_t  yaw_motor_angle_err; // ��λǰ��е�Ƕ����
	float    yaw_rad_err; // �������
} gimbal_info_t;

typedef struct
{
	int16_t  MID_VALUE;
	int16_t  restart_yaw_motor_angle;
	float    restart_yaw_imu_angle;
	int16_t  restart_pitch_motor_angle;
	float    restart_pitch_imu_angle;
	
	int16_t  rc_pitch_motor_offset;
	float    rc_yaw_imu_offset;
	float    rc_pitch_imu_offset;
	int16_t  key_pitch_motor_offset;
	float    key_yaw_imu_offset;
	float    key_pitch_imu_offset;
	
	float    max_pitch_imu_angle; // ��
	float    min_pitch_imu_angle; // ��
	int16_t  max_pitch_motor_angle; // ��
	int16_t  min_pitch_motor_angle; // ��
} gimbal_conf_t;

typedef struct 
{
	gimbal_dev_t    *dev;
	gimbal_info_t   *info;
	gimbal_conf_t   *conf;
	void			(*init)(void);
	void			(*ctrl)(void);
	void			(*self_protect)(void);
} gimbal_t;

extern gimbal_t gimbal;

/* Exported functions --------------------------------------------------------*/

#endif
