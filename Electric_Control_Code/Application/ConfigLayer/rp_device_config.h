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
	DEV_ID_IMU = 0,
	DEV_ID_IMU_EX,
	DEV_ID_RC,
	DEV_ID_JUDGE,
	DEV_ID_VISION,
	DEV_ID_CNT,
} dev_id_t;

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

typedef enum {
	BELT_R,		//��Ƥ���������Ϊ���̶���Ƥ���ұߣ������������ڴ��浯������
	WIND,		//���ŷ��ڵķ糵
	BELT_L,		//��Ƥ�����������ѷ糵�ϵķ��ڲ��������
	DIAL,		//���Ʋ����ڵĵ���
	AXIS_X,		//X�ᳯ��
	AXIS_ZL,	//��Z�ᳯ���Է��ڴ��ȥ�ķ���Ϊǰ��
	AXIS_ZR,	//��Z�ᳯ��
	AXIS_Y,		//���ư���Ƥ��λ�õĵ������������
	MOTOR_LIST,
} dev_motor_list_e;

/**
 *	@brief	�豸�ṹ�嶨��ģ��
 *	@class	device
 */
typedef struct device {
	void				        *info;		                                    // �Զ�������豸��Ϣ�ṹ��
	void				        *driver;	                                    // �Զ�������豸�����ṹ��
	void				        (*init)(struct device *self);	                // �豸��ʼ������
	void				        (*update)(struct device *self, uint8_t *rxBuf);	// �豸���ݸ��º���
	void				        (*check)(struct device *self);	                // �豸���ݼ�麯��
	void				        (*heart_beat)(struct device *self);	            // �豸������
	volatile dev_work_state_t	work_state;	                                    // �豸����״̬
	volatile dev_errno_t	  	errno;		                                    // ���Զ�������豸�������
	const    dev_id_t		    	id;			                                // �豸id
} device_t;

#endif
