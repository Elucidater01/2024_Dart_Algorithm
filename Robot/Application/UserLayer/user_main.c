/**
 * @file        user_main.c
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        18-November-2022
 * @brief       User Main
 * @update
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "tim.h"

#include "user_main.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void user_main(void);
/* Exported functions --------------------------------------------------------*/

/**
 *	@brief	�û��豸��ʼ��(Init)
 */
void USER_Init(void)
{
	motor_all_init();
	HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);
	launcher.init();
}


/**
 *	@brief	�û�Ӧ�ò㣬1msִ��һ��(Loop)
 */
void user_main(void)
{
	//��������������ж��Ƿ�ʧ��
	motor[FRIC_R].heartbeat(&motor[FRIC_R]);
	motor[FRIC_L].heartbeat(&motor[FRIC_L]);
	motor[DIAL].heartbeat(&motor[DIAL]);
	rc_sensor.heart_beat(&rc_sensor);
	 
	if (launcher.info->rc_work_state == DEV_ONLINE)
	{
		launcher.ctrl();
	}
	else
	{
		launcher.self_protect();
	}
}



/**
 *	@brief	��ʱ���жϻص���1ms����һ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == TIM4)
 {
	 static uint16_t i = 0;
	 
	 if (++i == 60000)
	 {
		 i = 0;
	 }
	 
	 user_main();
	
 }
}

 
 
 
 
 
 
 
 
 
 
 
 
 
 
///*	
//	��������
//*/
//int16_t send_buff[4];
//float tar;
//void StartControlTask(void const * argument)
//{
//	
//	//�����ʼ��
//	motor[GIMB_Y].init(&motor[GIMB_Y]);
//	//����ٶ�pid��ʼ��
//	motor[GIMB_Y].pid_init(&motor[GIMB_Y].pid.speed,gimb_y_speed_pid_param);
//	
//  for(;;)
//  {
//		//��������������ж��Ƿ�ʧ��
//		motor[GIMB_Y].heartbeat(&motor[GIMB_Y]);

////		//pid����
//		send_buff[motor[GIMB_Y].id.buff_p] = motor[GIMB_Y].c_speed(&motor[GIMB_Y],tar);


////		//�������ݵķ���
//		CAN1_Send_With_int16_to_uint8(motor[GIMB_Y].id.tx_id,send_buff);
////		
////		
//		
//		
//    osDelay(1);
//  }

//}




