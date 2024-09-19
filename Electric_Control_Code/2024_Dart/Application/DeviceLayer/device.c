/**
 * @file        device.c
 */
 
/* Includes ------------------------------------------------------------------*/
#include "device.h"


/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
dev_list_t dev_list = {
	//.imu_sen    = &imu_sensor,
	.rc_sen     = &rc_sensor,
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void DEVICE_Init(void)
{
	dev_list.rc_sen->init(dev_list.rc_sen);
	//dev_list.imu_sen->init(dev_list.imu_sen);
//	OLED_Init();
	motor_list_init();
}
