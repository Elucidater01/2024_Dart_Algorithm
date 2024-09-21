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
 *	@brief	用户设备初始化(Init)
 */
void USER_Init(void)
{
	motor_all_init();
	HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);
	launcher.init();
}


/**
 *	@brief	用户应用层，1ms执行一次(Loop)
 */
void user_main(void)
{
	//电机心跳，用于判断是否失联
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
 *	@brief	定时器中断回调，1ms进入一次
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
//	发送数组
//*/
//int16_t send_buff[4];
//float tar;
//void StartControlTask(void const * argument)
//{
//	
//	//电机初始化
//	motor[GIMB_Y].init(&motor[GIMB_Y]);
//	//电机速度pid初始化
//	motor[GIMB_Y].pid_init(&motor[GIMB_Y].pid.speed,gimb_y_speed_pid_param);
//	
//  for(;;)
//  {
//		//电机心跳，用于判断是否失联
//		motor[GIMB_Y].heartbeat(&motor[GIMB_Y]);

////		//pid计算
//		send_buff[motor[GIMB_Y].id.buff_p] = motor[GIMB_Y].c_speed(&motor[GIMB_Y],tar);


////		//控制数据的发送
//		CAN1_Send_With_int16_to_uint8(motor[GIMB_Y].id.tx_id,send_buff);
////		
////		
//		
//		
//    osDelay(1);
//  }

//}




