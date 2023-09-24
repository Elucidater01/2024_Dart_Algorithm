/**
 * @file        imu_potocol.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-September-2020
 * @brief       Imu Potocol.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "imu_potocol.h"

#include "bmi.h"

#include "imu_sensor.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
short gyrox, gyroy, gyroz;
short accx, accy, accz;
void imu_sensor_update(imu_sensor_t *imu_sen)
{
    imu_sensor_info_t *imu_info = imu_sen->info;
	
    BMI_Get_RawData(&gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
    BMI_Get_EulerAngle(&imu_info->pitch, &imu_info->roll, &imu_info->yaw, &gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
    
	imu_info->rate_roll = gyrox;
	imu_info->rate_pitch = gyroy;
	imu_info->rate_yaw = gyroz;
	
	imu_sen->check(imu_sen);
}

//int8_t imu_init_errno;
void imu_sensor_init(imu_sensor_t *imu_sen)
{
    int8_t rslt;
	uint32_t tickstart = HAL_GetTick();
	imu_sensor_info_t *imu_info = imu_sen->info;
	
	imu_sen->errno = NONE_ERR;

    rslt = BMI_Init();
	while(rslt) {
        // �����ʼ��ʧ�������³�ʼ��
        imu_sen->errno = DEV_INIT_ERR;
        rslt = BMI_Init();
    }
    //imu_init_errno = rslt;
    
	for(uint16_t i=0; i<250; i++) {
		BMI_Get_GRO(&imu_info->rate_pitch, &imu_info->rate_roll, &imu_info->rate_yaw);
		imu_info->rate_pitch_offset += imu_info->rate_pitch;
		imu_info->rate_yaw_offset += imu_info->rate_yaw;
	}
    /**
        @note
        ����ϵ��ʱ����̨�˶����ᵼ�¼�������ľ�̬ƫ����ֵ�������ÿ���ϵ��ʱ�򣬾�̬ƫ���
        ��𲻴�Ļ�������ֱ�Ӹ���һ���̶�ֵ�����ߣ�����Լ��������ƫ��ֵ���жϵȡ�
    */
	imu_info->rate_pitch_offset /= 250.f;
	imu_info->rate_yaw_offset /= 250.f;

	if(imu_sen->id != DEV_ID_IMU)
		imu_sen->errno = DEV_ID_ERR;
}
