/**
 * @file        device.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        15-September-2020
 * @brief       Devices' Manager.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "device.h"

#include "drv_haltick.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
dev_list_t dev_list = {
	.rc_sen = &rc_sensor,
	.imu_sen = &imu_sensor,
	.chas_mtr[CHAS_L] = &chassis_motor[CHAS_L],
	.chas_mtr[CHAS_R] = &chassis_motor[CHAS_R],
	.chas_mtr[CHAS_F] = &chassis_motor[CHAS_F],
	.chas_mtr[CHAS_B] = &chassis_motor[CHAS_B],
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void DEV_Init(void)
{
	dev_list.rc_sen->init(dev_list.rc_sen);
//	dev_list.imu_sen->init(dev_list.imu_sen);
	dev_list.chas_mtr[CHAS_L]->init(dev_list.chas_mtr[CHAS_L]);
	dev_list.chas_mtr[CHAS_R]->init(dev_list.chas_mtr[CHAS_R]);
	dev_list.chas_mtr[CHAS_F]->init(dev_list.chas_mtr[CHAS_F]);
	dev_list.chas_mtr[CHAS_B]->init(dev_list.chas_mtr[CHAS_B]);
}
