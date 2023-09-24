#ifndef __CHASSIS_H
#define __CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "chassis_motor.h"
//#include "gimbal_motor.h"
#include "imu_sensor.h"
#include "rc_sensor.h"

#define CHAS_MECH_ANGLE_POS_MID CO_MECH_ANGLE_POS_MID
#define CHAS_MECH_ANGLE_NEG_MID CO_MECH_ANGLE_NEG_MID

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/**
  * @brief   ���̱���ģʽ���� 
  * @note    ����ϵͳģʽ�Ĳ�ͬ���������ò�ͬ�ĵ��̱���ģʽ
  * @author  RobotPilots
  */
typedef enum {
	CHASSIS_MODE_NORMAL 		= 0, // ����ģʽ
	CHASSIS_MODE_LONGSHOOT   		= 1, // Զ��ģʽ
	CHASSIS_MODE_RELOAD_BULLET	= 2, // ���̵��ٲ���ģʽ
	CHASSIS_MODE_SZUPUP			= 3, // SZU����ģʽ
} chassis_mode_t;


/**
  * @brief ���̵��pid�е�target����
  * @note 
  * @author  RobotPilots  
*/
typedef struct {
	pid_ctrl_t	speed;
	pid_ctrl_t	angle;
	
	pid_ctrl_t  stand_anglepid;
	pid_ctrl_t  stand_speedpid;
	pid_ctrl_t  locat_pid;
	pid_ctrl_t  TurnR_pid;
	float		out;
} chassis_motor_pid_t;

typedef struct {
	pid_ctrl_t	angle;
	float 		out;
} chassis_z_pid_t;

/**
  * @brief 		chassis_ctrl_t ���̿�������
  * @note 		�ṹ������ָ����̵��������PID��ָ��
							�Լ������˶�ʱz������ת��PIDָ��
	* @program	chassis_motor_pid_t  	ָ�����ٶȻ��ͽǶȻ��Լ�����������ָ��
							chassis_z_pid_t				ָ��z����ǶȺ�����ṹ���ָ��
  * @author	  RobotPilots  
*/
typedef struct {
	chassis_motor_pid_t		(*motor)[CHAS_MOTOR_CNT];
	chassis_z_pid_t			*z_atti;	// z������̬
} chassis_ctrl_t;

/**
  * @brief 		chassis_dev_t ������������
  * @note 		���Ի�ÿ���ʱ����Ҫ�ĸ��ഫ����Ϣ
	* @program	chas_motor  	ָ�򵥸�������ݵ�ָ��
							yaw_motor			ָ����̨������ݵ�ָ��
							imu_sensor		ָ��IMU��������ָ��
							rc_sensor			ָ��ң������������ָ��	
  * @author 	RobotPilots  
*/
typedef struct {
	chassis_motor_t	*chas_motor[CHAS_MOTOR_CNT];
	//gimbal_motor_t	*yaw_motor;
	imu_sensor_t	*imu_sensor;
	rc_sensor_t		*rc_sensor;
} chassis_dev_t;


/**
  * @brief		chassis_info_t  ������Ϣ����
  * @note			��¼�˵�ǰ���̵Ĺ���ģʽ�Լ��߼���λ
  * @program	remote_mode			ң��ģʽ������ȷ������������Դ
							co_mode					����ģʽ������ȷ������̨���ϵ�ģʽ
							local_mode			����ģʽ������ȷ�������˶�ģʽ
							co_angle_logic	��̨�������־
							top_gyro				С����ģʽ��־λ
  * @author  RobotPilots  
*/
typedef struct {
	remote_mode_t		remote_mode;
	co_mode_t		co_mode;
	chassis_mode_t		local_mode;
	co_angle_logic_t	co_angle_logic; //����ͷ�����λ��
	bool				top_gyro;
}chassis_info_t;


/**
  * @brief		chassis_t  ��������
  * @note			���˴�����Ϣ�Ϳ�����Ϣ�⣬�������˺���ָ������ִ����Ӧ����
  * @program	�����������֣���Ϣ���ֿ����棩
							.init 										��ʼ��
							.update 									���̸��º���
							.test											���Ժ���������test_open����ʱ����
							.ctrl											���ƺ���
							.output										����������
							.self_protect							ʧ������
							.if_back_to_mid_angle			�Ƿ���в�ѯ
							.if_top_gyro_open					�Ƿ��������ǲ�ѯ

	* @author  RobotPilots  
*/
typedef struct chassis{
	chassis_ctrl_t	*controller;		//ָ��chassis_ctrl_t ���̿��������� ���͵�ָ��
	chassis_dev_t	*dev;							//���̿�����ص�������Ϣ
	chassis_info_t	*info;					//��ϵͳģʽ���������ı���ģʽ
	bool			test_open;						//�����ú���
	void			(*init)(void);
	void			(*update)(void);
	void			(*test)(void);
	void			(*ctrl)(void);
	void			(*output)(void);
	void			(*self_protect)(void);
	bool			(*if_back_to_mid_angle)(void);
	bool			(*if_top_gyro_open)(void);
}chassis_t;

extern chassis_t chassis;
extern chassis_motor_pid_t 	chas_motor_pid[CHAS_MOTOR_CNT];
//��ң�����йغ���
void CHASSIS_RCnormal_output(void);
void CHASSIS_RClost_output(void);
void CHASSIS_output(void);
/* Exported functions --------------------------------------------------------*/
/* ��Ϣ�� --------------------------------------------------------------------*/
bool CHASSIS_IfBackToMiddleAngle(void);
void CHASSIS_Ctrl(void);

void CHASSIS_FBinit(void);
void FBwheel_PidangleCtrl(float F_angle_targ,float B_angle_targ);
#endif
