/**
 * @file        control_task.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-November-2020
 * @brief       Control Center
 */

/* Includes ------------------------------------------------------------------*/
#include "control_task.h"

#include "device.h"
#include "cmsis_os.h"
#include "chassis.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	控制任务
 */
void StartControlTask(void const * argument)
{
	chassis.init();
	
	for(;;)
	{
		CHASSIS_Ctrl();//只计算不输出
		if(sys.state == SYS_STATE_NORMAL) {
			CHASSIS_RCnormal_output();
			CHASSIS_FBinit();//只有一句if执行快
			//module.ctrl()
		} else if(sys.state ==SYS_STATE_RCLOST)
		{
			CHASSIS_RClost_output();
			//module.self_protect();
		}
		CHASSIS_output();
		osDelay(4);
	}
}
