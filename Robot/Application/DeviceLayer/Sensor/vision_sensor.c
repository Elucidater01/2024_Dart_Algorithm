
/* Includes ------------------------------------------------------------------*/
#include "vision_sensor.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void vision_init(vision_sensor_t *vis_sen);
void vision_heart_beat(vision_sensor_t *vis_sen);
extern void vision_update(vision_sensor_t *vis_sen, uint8_t *rxBuf);
extern void vision_check(vision_sensor_t *vis_sen);	

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
drv_uart_t	vision_sensor_driver = {
	.id = DRV_UART6,
};

vision_info_t 	vision_sensor_info = {
	.tx_info = &vision_tx_info,
	.rx_info = &vision_rx_info,
	.offline_max_cnt = 200,
};

/* Exported variables --------------------------------------------------------*/
vision_sensor_t vision_sensor = {
	.info = &vision_sensor_info,
	.driver = &vision_sensor_driver,
	.init = vision_init,
	.update = vision_update,
	.check = vision_check,
	.heart_beat = vision_heart_beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_VISION,
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void vision_init(vision_sensor_t *vis_sen)
{
	vision_tx_info_t *tx_info = vis_sen->info->tx_info;
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	
	vis_sen->info->rx_flag = 0;
	
	tx_info->pitch_angle = 0.0f; // dataf_1
	tx_info->yaw_angle = 0.0f;   // dataf_2
	tx_info->mode = AIM_OFF;     // datau8_1
	tx_info->shoot_speed = 0;    // datau8_2
	tx_info->my_color = 0;       // datau8_3
	tx_info->is_change_target = 0;// datau8_4
	
	rx_info->pitch_angle = 0.0f; // dataf_1
	rx_info->yaw_angle = 0.0f;   // dataf_2
	rx_info->mode = AIM_OFF;     // datau8_1
	rx_info->is_find_target = 0; // datau8_2
	rx_info->is_find_buff = 0;	 // datau8_3
	rx_info->is_hit_enable = 0;  // datau8_4
}

void vision_heart_beat(vision_sensor_t *vis_sen)
{
	vision_info_t *vis_info = vis_sen->info;

	vis_info->offline_cnt++;
	if(vis_info->offline_cnt > vis_info->offline_max_cnt) {
		vis_info->offline_cnt = vis_info->offline_max_cnt;
		vis_sen->work_state = DEV_OFFLINE;
	} 
	else {
		/* ÀëÏß->ÔÚÏß */
		if(vis_sen->work_state == DEV_OFFLINE)
		{
			vis_sen->work_state = DEV_ONLINE;
		}
	}
}


