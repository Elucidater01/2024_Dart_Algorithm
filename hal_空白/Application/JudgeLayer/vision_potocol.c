/**
 * @file        vision_potocol.c
 * @author      Sentry@2021
 * @Version     V1.0
 * @date        19-February-2021
 * @brief       .
 */
 
/* Includes ------------------------------------------------------------------*/
#include  "vision_potocol.h"
#include  "vision_sensor.h"
#include  "crc.h"
#include "string.h"
#include "judge_infantrypotocol.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void vision_init(vision_sensor_t *vision)
{
	// 初始化为离线状态
	vision->info->State.offline_cnt = vision->info->State.offline_max_cnt + 1;
	vision->work_state = DEV_OFFLINE;
	
	if(vision->id == DEV_ID_VISION)
		vision->errno = NONE_ERR;
	else
		vision->errno = DEV_ID_ERR;	
}
/**
 *	@brief	视觉数据解析协议
 */
void vision_update(vision_sensor_t *vision_sen, uint8_t *rxBuf)
{
	vision_info_t *vision_info = vision_sen->info;

	uint8_t res = false;
	vision_info->State.rx_cnt++;
	/* 帧首字节是否为0xA5 */
	if(rxBuf[sof] == VISION_FRAME_HEADER) 
	{	
		res = Verify_CRC8_Check_Sum( rxBuf, LEN_FRAME_HEADER );
		/* 帧头CRC8校验*/
		if(res == true)
		{
			res = Verify_CRC16_Check_Sum( rxBuf, LEN_VISION_RX_PACKET );
			/* 帧尾CRC16校验 */
			if(res == true) 
			{
				/* 数据正确则拷贝接收包 */
				memcpy(vision_info->RxPacket.RxData.data, rxBuf, LEN_VISION_RX_PACKET);
				vision_info->State.rx_data_update = true;	// 视觉数据更新	
				{
//					for(i = 0; i < 40; i++)
//					{
						memcpy((void*)(vision_info->RxPacket.RxData.jiesuan),vision_info->RxPacket.RxData.data+3,LEN_VISION_RX_PACKET-5);
//						vision_info->RxPacket.RxData.jiesuan[i] = vision_info->RxPacket.RxData.data[4*i+3]<<24 | 
//																											vision_info->RxPacket.RxData.data[4*i+4]<<16 |
//																											vision_info->RxPacket.RxData.data[4*i+5]<<8 |
//																											vision_info->RxPacket.RxData.data[4*i+6];
//          }
				}
				/* 帧率计算 */
				vision_info->State.rx_time_now = xTaskGetTickCountFromISR();
				vision_info->State.rx_time_fps = vision_info->State.rx_time_now - vision_info->State.rx_time_prev;
				vision_info->State.rx_time_prev = vision_info->State.rx_time_now;		
				vision_info->State.offline_cnt=0;
			}
		}
	}	
	/* 数据有效性判断 */
	if(res == true) {
		vision_info->State.rx_data_valid = true;
	} else if(res == false) {
		vision_info->State.rx_data_valid = false;
		vision_info->State.rx_err_cnt++;
	}
	radar_Info_store();
}

/**
 *	@brief	在串口1中解析遥控数据协议
 */
void USART1_rxDataHandler(uint8_t *rxBuf)
{
	// 更新视觉数据
	vision_sensor.update(&vision_sensor, rxBuf);
	vision_sensor.check(&vision_sensor);
}
