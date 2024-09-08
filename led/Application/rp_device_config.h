/**
 * @file        rp_device_config.h
 * @author      RobotPilots
 * @Version     v1.0
 * @brief       RobotPilots Robots' Device Configuration.
 * @update
 *              v1.0(7-November-2021)  
 */
#ifndef __RP_DEVICE_CONFIG_H
#define __RP_DEVICE_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "rp_driver_config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* �豸�� --------------------------------------------------------------------*/
/**
 *	@brief	�豸id�б�
 *	@class	device
 */
typedef enum {
	DEV_ID_RC = 0,
	DEV_ID_IMU = 1,
	DEV_ID_CHASSIS_LF = 2,
	DEV_ID_CHASSIS_RF = 3,
	DEV_ID_CHASSIS_LB = 4,
	DEV_ID_CHASSIS_RB = 5,
	DEV_ID_CNT = 6,
} dev_id_t;

/**
 *	@brief	���̵���豸����
 *	@class	device
 */
typedef enum {
	CHAS_LF,
	CHAS_RF,
	CHAS_LB,
	CHAS_RB,
	CHAS_MOTOR_CNT,
} chassis_motor_cnt_t;

/**
 *	@brief	�豸����״̬(ͨ��)
 *	@class	device
 */
typedef enum {
	DEV_ONLINE,
	DEV_OFFLINE,
} dev_work_state_t;

/**
 *	@brief	�������(ͨ��)
 *  @note   ���Զ����豸����������Ͳ��滻����errno�ı������ͣ���
 *          typedef enum {
 *              IMU_NONE_ERR,
 *              IMU_ID_ERR,
 *              IMU_COM_FAILED,
 *              IMU_DEV_NOT_FOUND,
 *              ...
 *          } imu_errno_t;
 *          
 *          typedef struct imu_sensor_struct {
 *              ...
 *	            imu_errno_t errno;
 *              ...	
 *          } imu_sensor_t;
 *	@class	device
 */
typedef enum {
	NONE_ERR,		// ����(�޴���)
	DEV_ID_ERR,		// �豸ID����
	DEV_INIT_ERR,	// �豸��ʼ������
	DEV_DATA_ERR,	// �豸���ݴ���
} dev_errno_t;

/**
 *	@brief	�豸�ṹ�嶨��ģ��
 *	@class	device
 */
typedef struct device {
	void				        *info;		// �Զ�������豸��Ϣ�ṹ��
	void				        *driver;	// �Զ�������豸�����ṹ��
	void				        (*init)(struct device *self);	// �豸��ʼ������
	void				        (*update)(struct device *self, uint8_t *rxBuf);	// �豸���ݸ��º���
	void				        (*check)(struct device *self);	// �豸���ݼ�麯��
	void				        (*heart_beat)(struct device *self);	// �豸������
	volatile dev_work_state_t	work_state;	// �豸����״̬
	volatile dev_errno_t		errno;		// ���Զ�������豸�������
	const    dev_id_t			id;			// �豸id
} device_t;

/**
 *	@brief	RM��׼�����Ϣ��ṹ��
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

typedef struct rm_motor_struct {
	rm_motor_info_t 	        *info;
	drv_can_t				    *driver;
	void					    (*init)(struct rm_motor_struct *self);
	void					    (*update)(struct rm_motor_struct *self, uint8_t *rxBuf);
	void					    (*check)(struct rm_motor_struct *self);	
	void					    (*heart_beat)(struct rm_motor_struct *self);
	volatile dev_work_state_t   work_state;
	volatile dev_errno_t	    errno;
	const    dev_id_t			id;
} rm_motor_t;

#endif
