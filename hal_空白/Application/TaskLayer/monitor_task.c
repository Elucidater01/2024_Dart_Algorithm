/**
 * @file        monitor_task.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-November-2020
 * @brief       Monitor&Test Center
 */

/* Includes ------------------------------------------------------------------*/
#include "monitor_task.h"

#include "drv_io.h"
#include "device.h"
#include "rp_math.h"
#include "cmsis_os.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void device_heart_beat(void)
{
	rc_sensor.heart_beat(&rc_sensor);
	imu_sensor.heart_beat(&imu_sensor);
	chassis_motor[CHAS_L].heart_beat(&chassis_motor[CHAS_L]);
	chassis_motor[CHAS_R].heart_beat(&chassis_motor[CHAS_R]);
	chassis_motor[CHAS_F].heart_beat(&chassis_motor[CHAS_F]);
	chassis_motor[CHAS_B].heart_beat(&chassis_motor[CHAS_B]);
}

static void system_led_flash(void)
{
	static uint16_t led_blue_flash = 0;
	
	led_blue_flash++;
	if(led_blue_flash > 500) 
	{
		led_blue_flash = 0;
		LED_BLUE_TOGGLE();
	}
}

/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	系统监控任务
 */
void StartMonitorTask(void const * argument)
{
	//LED_RED_ON();
	for(;;)
	{
		system_led_flash();
		device_heart_beat();		//掌管心跳大权
		imu_sensor.update(&imu_sensor);
		
		if(sys.co_mode == CO_GYRO)
		{
			LED_BLUE_ON();
			LED_GREEN_OFF();
		}
		else if(sys.co_mode == CO_MECH)
		{
			LED_BLUE_OFF();
			LED_GREEN_ON();
		}
		
		if(sys.remote_mode == KEY)
		{
			LED_ORANGE_ON();
			LED_RED_OFF();
		}
		else if(sys.remote_mode == RC)
		{
			LED_ORANGE_OFF();
			LED_RED_ON();
		}
		
		osDelay(1);
	}
}
