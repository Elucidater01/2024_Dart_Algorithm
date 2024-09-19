#ifndef __DEVICE_H
#define __DEVICE_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rc_sensor.h"
#include "vision_sensor.h"
#include "judge_sensor.h"
#include "motor.h"
#include "RM_motor.h"
//#include "oled.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct dev_list_struct 
{
	//imu_sensor_t    *imu_sen;
	rc_sensor_t 	*rc_sen;
} dev_list_t;

extern dev_list_t dev_list;

/* Exported functions --------------------------------------------------------*/
void DEVICE_Init(void);

#endif
