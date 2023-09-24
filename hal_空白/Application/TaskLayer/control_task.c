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
 *	@brief	��������
 */
void StartControlTask(void const * argument)
{
	chassis.init();
	
	for(;;)
	{
		CHASSIS_Ctrl();//ֻ���㲻���
		if(sys.state == SYS_STATE_NORMAL) {
			CHASSIS_RCnormal_output();
			CHASSIS_FBinit();//ֻ��һ��ifִ�п�
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
