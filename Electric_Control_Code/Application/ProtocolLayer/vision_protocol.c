/* Includes ------------------------------------------------------------------*/
#include "vision_protocol.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t micros(void);

uint8_t vision_txBuf[40];

/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

vision_tx_info_t vision_tx_info = {
	.SOF = 0xA5,

};
vision_rx_info_t vision_rx_info;

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Send the vision sensor data
 * @param  vis_sen: Pointer to a vision_sensor_t structure
 * @retval None
 */
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
}

void vision_check(vision_sensor_t *vis_sen)
{
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	vision_info_t *info = vis_sen->info;
	
	info->offline_cnt = 0;
	
	if (info->rx_flag == 1)
	{
		memcpy(&info->yaw_error, (void*)&rx_info->yaw_error, 4);
		memcpy(&info->target_status, &rx_info->target_status, 1);
	}
}

void USART1_rxDataHandler(uint8_t *rxBuf)
{
	vision_sensor.update(&vision_sensor, rxBuf);
	vision_sensor.check(&vision_sensor);

}
