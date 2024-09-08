/**
 * @file        imu_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       Imu Protocol.
 * @update      
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.修改imu_potocol.c/.h->imu_protocol.c/.h 
 */
 
/* Includes ------------------------------------------------------------------*/
#include "imu_protocol.h"

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
    
	imu_info->rate_pitch = gyrox;
	imu_info->rate_roll = gyroy;
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
        // 如果初始化失败则重新初始化
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
        如果上电的时候云台运动，会导致计算出来的静态偏差数值出错。如果每次上电的时候，静态偏差均
        差别不大的话，可以直接给定一个固定值。或者，另外对计算出来的偏差值做判断等。
    */
	imu_info->rate_pitch_offset /= 250.f;
	imu_info->rate_yaw_offset /= 250.f;

	if(imu_sen->id != DEV_ID_IMU)
		imu_sen->errno = DEV_ID_ERR;
}
