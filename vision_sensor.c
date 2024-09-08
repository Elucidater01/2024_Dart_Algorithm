/**
 ******************************************************************************
 * @file    vision_sensor.c
 * @author  RobotPilots
 * @brief   Vision sensor driver
 ******************************************************************************
 * @attention
 * 
 * Copyright 2024 RobotPilots
 * 
 * @Version     V1.0
 * @date        August-2024
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "vision_sensor.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void vision_init(vision_sensor_t *vis_sen);
void vision_heart_beat(vision_sensor_t *vis_sen);
void vision_update(vision_sensor_t *vis_sen, uint8_t *rxBuf);
void vision_send(vision_sensor_t *vis_sen);	

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define serial port number */
drv_uart_t	vision_sensor_driver = {
	.id = DRV_UART1,
};

/* Define the vision sensor information */
vision_info_t 	vision_sensor_info = {
	.tx_info = &vision_tx_info,
	.rx_info = &vision_rx_info,
	.offline_max_cnt = 200,
};

/* Exported variables --------------------------------------------------------*/
/* Define the vision sensor structure*/
vision_sensor_t vision_sensor = {
	.info = &vision_sensor_info,
	.driver = &vision_sensor_driver,
	.init = vision_init,
	.update = vision_update,
	.send = vision_send,
	.heart_beat = vision_heart_beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_VISION,
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the vision sensor
 * @param  vis_sen: Pointer to a vision_sensor_t structure
 * @retval None
 */
void vision_init(vision_sensor_t *vis_sen)
{
	vision_tx_info_t *tx_info = vis_sen->info->tx_info;
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	
	vis_sen->info->rx_flag = 0;
	tx_info->mode = 0;     
	rx_info->yaw_error = 0;
}

/**
 * @brief  Vision sensor Sensor loss detection
 * @param  vis_sen: Pointer to a vision_sensor_t structure
 * @retval None
 */
void vision_heart_beat(vision_sensor_t *vis_sen)
{
	vision_info_t *vis_info = vis_sen->info;

	vis_info->offline_cnt++;
	if(vis_info->offline_cnt > vis_info->offline_max_cnt) 
    {
		vis_info->offline_cnt = vis_info->offline_max_cnt;
		vis_sen->work_state = DEV_OFFLINE;
	} 
	else {
		if(vis_sen->work_state == DEV_OFFLINE)
		{
			vis_sen->work_state = DEV_ONLINE;
		}
	}
}

/**
 * @brief  Vision sensor data processing
 * @param  vis_sen: Pointer to a vision_sensor_t structure
 * @param  rxBuf: Pointer to the received data
 * @retval None
 */
void vision_update(vision_sensor_t *vis_sen, uint8_t *rxBuf)
{

	if(rxBuf[0] == 0xA5)
	{
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(vision_rx_info_t)) == true)
			{
				memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
				vis_sen->info->rx_flag = 1;
				return;
			}
		}
	}
	vis_sen->info->rx_flag = 0;


    pid_ctrl_t *pid = &axis_pid;
    if(judge_sensor.info->game_status.game_progress == 4)
	{
		vision_sensor.info->tx_info->mode = 1;
        /* When the dart gate is open */
		if(referee_data.dart_launch_opening_status != 1)
		{
            /* When the vision sensor is online and the target is found */
			if(vision_sensor.work_state == DEV_ONLINE && vision_sensor.info->target_status == 1)
			{
                /* Record the error and double it */
				pid->err = abs(vision_sensor.info->rx_info->yaw_error) * vision_sensor.info->rx_info->yaw_error;
                /* Calculate the PID output */
				single_pid_ctrl(pid);
				AXISX_TOSPEED(pid->out);
                /* Record the now angle of axis_x */
				y_angle = rm_motor[AXIS_X].info->angle_sum;
			}
			else
			{
                /* When the vision sensor is offline or the target is lost, axis_x need to be protected */
				AXISX_TOANGLE(y_angle);
			}
		}
    }
}

/**
 * @brief  Send the vision sensor data
 * @param  vis_sen: Pointer to a vision_sensor_t structure
 * @retval None
 */
void vision_send(vision_sensor_t *vis_sen)
{
    vision_tx_info_t *tx_info = vision_sensor->info->tx_info;

	tx_info->number = module.launcher->times;
	tx_info->state = module.axis->state;
	tx_info->gate = referee_data.dart_launch_opening_status;
}
