#ifndef __DEVICE_H
#define __DEVICE_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

//#include "cap.h"
#include "imu_sensor.h"
//#include "rc_sensor.h"
//#include "vision_sensor.h"
#include "keyboard.h"
//#include "judge.h"
#include "motor.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct dev_list_struct 
{
	imu_sensor_t    *imu_sen;
	rc_sensor_t 	*rc_sen;
	
	
} dev_list_t;

extern dev_list_t dev_list;

/* Exported functions --------------------------------------------------------*/
void DEVICE_Init(void);

/* Servo functions */
extern void Magazine_Open(void);
extern void Magazine_Close(void);
extern void Magazine_Sleep(void);

#endif
