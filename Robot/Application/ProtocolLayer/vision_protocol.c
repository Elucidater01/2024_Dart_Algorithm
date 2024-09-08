
/* Includes ------------------------------------------------------------------*/
#include "vision_protocol.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t micros(void);

uint8_t vision_txBuf[40];

uint32_t t1, t2, tmp;
/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

vision_tx_info_t vision_tx_info = {
	.SOF = 0xA5,
	.blood_0 = 0,
	.blood_1 = 100,
	.blood_2 = 100,
	.blood_3 = 100,
	.blood_4 = 100,
	.blood_5 = 100,
	.blood_6 = 100,
	.blood_7 = 100,
	.blood_8 = 100,
	.size_3 = 0,
	.size_4 = 0,
	.size_5 = 0,
};
vision_rx_info_t vision_rx_info;

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
bool vision_send_data(void)
{
	memcpy(vision_txBuf, &vision_tx_info, sizeof(vision_tx_info_t));
	Append_CRC8_Check_Sum(vision_txBuf, 3);
	vision_tx_info.CRC8 = vision_txBuf[2];
	Append_CRC16_Check_Sum(vision_txBuf, sizeof(vision_tx_info_t));
	vision_tx_info.CRC16 = (uint16_t)((vision_txBuf[sizeof(vision_tx_info_t)-2] << 8)
                                   | vision_txBuf[sizeof(vision_tx_info_t)-1]);
	
	if(HAL_UART_Transmit_DMA(&huart1,vision_txBuf,sizeof(vision_tx_info_t)) == HAL_OK)
	{
			return true;
	}
	return false;
}

uint32_t rx_cnt = 0, rxr_cnt = 0;
void vision_update(vision_sensor_t *vis_sen, uint8_t *rxBuf)
{
	rx_cnt ++;
	if(rxBuf[0] == 0xA5)
	{
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(vision_rx_info_t)) == true)
			{
				memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
				vis_sen->info->rx_flag = 1;
				
				rxr_cnt++;
				t1 = t2;
				t2 = micros();
				tmp = t2 - t1;
				return;
			}
		}
	}
	vis_sen->info->rx_flag = 0;
}

void vision_check(vision_sensor_t *vis_sen)
{
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	vision_info_t *info = vis_sen->info;
	
	info->offline_cnt = 0;
	
	if (info->rx_flag == 1)
	{
		memcpy(&info->mode, &rx_info->mode, 1);
		memcpy(&info->target_pitch_angle, (void*)&rx_info->pitch_angle, 4);
		memcpy(&info->target_yaw_angle, (void*)&rx_info->yaw_angle, 4);
		memcpy(&info->is_find_target, &rx_info->is_find_target, 1);
		memcpy(&info->is_find_buff, &rx_info->is_find_buff, 1);
		memcpy(&info->is_hit_enable, &rx_info->is_hit_enable, 1);
		memcpy(&info->is_spin, &rx_info->is_spin, 1);
		memcpy(&info->shoot_type, &rx_info->shoot_type, 1);
		memcpy(&info->armor_num, &rx_info->armor_num, 1);
		memcpy(&info->armor_id, &rx_info->armor_id, 1);
	}
}

void USART1_rxDataHandler(uint8_t *rxBuf)
{
	vision_sensor.update(&vision_sensor, rxBuf);
	vision_sensor.check(&vision_sensor);
}
