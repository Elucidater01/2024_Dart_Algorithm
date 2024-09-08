 
/* Includes ------------------------------------------------------------------*/
#include "judge_protocol.h"
#include "string.h"

#include "cap.h"
#include "vision_sensor.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void cap_data_send(uint8_t can_num);

/* 弹速统计 */
void Speed_Statistic(void);
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16_t shoot_num;
	
	uint16_t speed_260;
	uint16_t speed_270;
	uint16_t speed_271;
	uint16_t speed_272;
	uint16_t speed_273;
	uint16_t speed_274;
	uint16_t speed_275;
	uint16_t speed_276;
	uint16_t speed_277;
	uint16_t speed_278;
	uint16_t speed_279;
	uint16_t speed_280;
	uint16_t speed_281;
	uint16_t speed_282;
	uint16_t speed_283;
	uint16_t speed_284;
	uint16_t speed_285;
	uint16_t speed_286;
	uint16_t speed_287;
	uint16_t speed_288;
	uint16_t speed_289;
	uint16_t speed_290;
	uint16_t speed_291;
	uint16_t speed_292;
	uint16_t speed_293;
	uint16_t speed_294;
	uint16_t speed_295;
	uint16_t speed_296;
	uint16_t speed_297;
	uint16_t speed_298;
	uint16_t speed_299;
	uint16_t speed_300;
	
	float mean;
	float variance;
	
}shoot_data_t;

/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void judge_update(judge_t *self, uint16_t canId, uint8_t *rxbuf)
{
	switch (canId)
	{
		case GAME_STATUS_ID: // 3Hz
			vision_sensor.info->tx_info->size_3 = rxbuf[3];
			vision_sensor.info->tx_info->size_4 = rxbuf[4];
			vision_sensor.info->tx_info->size_5 = rxbuf[5];
			break;
		case GAME_ROBOT_HP_ID: // 1Hz
			vision_sensor.info->tx_info->blood_1 = rxbuf[0];
			vision_sensor.info->tx_info->blood_2 = rxbuf[1];
			vision_sensor.info->tx_info->blood_3 = rxbuf[2];
			vision_sensor.info->tx_info->blood_4 = rxbuf[3];
			vision_sensor.info->tx_info->blood_5 = rxbuf[4];
			vision_sensor.info->tx_info->blood_6 = rxbuf[5];
			vision_sensor.info->tx_info->blood_7 = rxbuf[6];
			vision_sensor.info->tx_info->blood_8 = rxbuf[7];
			break;
		case POWER_HEAT_ID: // 50Hz
			memcpy((void*)&judge.info->power_heat_data.chassis_power_buffer, (void*)rxbuf, 2);
			memcpy((void*)&judge.info->power_heat_data.shooter_id1_17mm_cooling_heat, (void*)&rxbuf[2], 2);
			cap.setdata(&cap, judge.info->power_heat_data.chassis_power_buffer, \
												judge.info->game_robot_status.chassis_power_limit, \
												judge.info->power_heat_data.chassis_volt, \
												judge.info->power_heat_data.chassis_current);
			cap_data_send(1);
			break;
		case GAME_ROBOT_STATUS_ID: // 10Hz
			judge.info->my_color = rxbuf[0];
			memcpy((void*)&judge.info->game_robot_status.shooter_id1_17mm_cooling_limit, (void*)&rxbuf[1], 2);
			memcpy((void*)&judge.info->game_robot_status.shooter_id1_17mm_speed_limit, (void*)&rxbuf[3], 2);
			memcpy((void*)&judge.info->game_robot_status.chassis_power_limit, (void*)&rxbuf[5], 2);
		  judge.info->remain_HP = rxbuf[7];
			break;
		case ROBOT_HURT_ID: // max25Hz
			judge.info->robot_hurt.armor_id = rxbuf[0];
			judge.info->robot_hurt.hurt_type = 1;
			break;
		case SHOOT_DATA_ID: // max25Hz
			memcpy((void*)&judge.info->shoot_data.bullet_speed, (void*)rxbuf, 4);
			Speed_Statistic();
			break;
		case 0x2E:
			judge.info->power_heat_data.chassis_volt = (uint16_t)(rxbuf[2] << 8 | rxbuf[3]);
			judge.info->power_heat_data.chassis_current = (uint16_t)(rxbuf[4] << 8 | rxbuf[5]);
			break;
		default:
			break;
	}
	
	self->info->offline_cnt = 0;
}

#if 1
shoot_data_t shoot_statistics;
uint8_t cali_flag = 0;
void Speed_Statistic(void)
{
			float s_speed = judge.info->shoot_data.bullet_speed;

			if (s_speed <= 27.0f)
			{
				shoot_statistics.speed_260++;
				shoot_statistics.shoot_num--;
			}
			else if (s_speed >= 27.0f && s_speed <= 27.1f)
			{
				shoot_statistics.speed_270++;
			}
			else if (s_speed >= 27.1f && s_speed <= 27.2f)
			{
				shoot_statistics.speed_271++;
			}
			else if (s_speed >= 27.2f && s_speed <= 27.3f)
			{
				shoot_statistics.speed_272++;
			}
			else if (s_speed >= 27.3f && s_speed <= 27.4f)
			{
				shoot_statistics.speed_273++;
			}
			else if (s_speed >= 27.4f && s_speed <= 27.5f)
			{
				shoot_statistics.speed_274++;
			}
			else if (s_speed >= 27.5f && s_speed <= 27.6f)
			{
				shoot_statistics.speed_275++;
			}
			else if (s_speed >= 27.6f && s_speed <= 27.7f)
			{
				shoot_statistics.speed_276++;
			}
			else if (s_speed >= 27.7f && s_speed <= 27.8f)
			{
				shoot_statistics.speed_277++;
			}
			else if (s_speed >= 27.8f && s_speed <= 27.9f)
			{
				shoot_statistics.speed_278++;
			}
			else if (s_speed >= 27.9f && s_speed <= 28.0f)
			{
				shoot_statistics.speed_279++;
			}
			else if (s_speed >= 28.0f && s_speed <= 28.1f)
			{
				shoot_statistics.speed_280++;
			}
			else if (s_speed >= 28.1f && s_speed <= 28.2f)
			{
				shoot_statistics.speed_281++;
			}
			else if (s_speed >= 28.2f && s_speed <= 28.3f)
			{
				shoot_statistics.speed_282++;
			}
			else if (s_speed >= 28.3f && s_speed <= 28.4f)
			{
				shoot_statistics.speed_283++;
			}
			else if (s_speed >= 28.4f && s_speed <= 28.5f)
			{
				shoot_statistics.speed_284++;
			}
			else if (s_speed >= 28.5f && s_speed <= 28.6f)
			{
				shoot_statistics.speed_285++;
			}
			else if (s_speed >= 28.6f && s_speed <= 28.7f)
			{
				shoot_statistics.speed_286++;
			}
			else if (s_speed >= 28.7f && s_speed <= 28.8f)
			{
				shoot_statistics.speed_287++;
			}
			else if (s_speed >= 28.8f && s_speed <= 28.9f)
			{
				shoot_statistics.speed_288++;
			}
			else if (s_speed >= 28.9f && s_speed <= 29.0f)
			{
				shoot_statistics.speed_289++;
			}
			else if (s_speed >= 29.0f && s_speed <= 29.1f)
			{
				shoot_statistics.speed_290++;
			}
			else if (s_speed >= 29.1f && s_speed <= 29.2f)
			{
				shoot_statistics.speed_291++;
			}
			else if (s_speed >= 29.2f && s_speed <= 29.3f)
			{
				shoot_statistics.speed_292++;
			}
			else if (s_speed >= 29.3f && s_speed <= 29.4f)
			{
				shoot_statistics.speed_293++;
			}
			else if (s_speed >= 29.4f && s_speed <= 29.5f)
			{
				shoot_statistics.speed_294++;
			}
			else if (s_speed >= 29.5f && s_speed <= 29.6f)
			{
				shoot_statistics.speed_295++;
			}
			else if (s_speed >= 29.6f && s_speed <= 29.7f)
			{
				shoot_statistics.speed_296++;
			}
			else if (s_speed >= 29.7f && s_speed <= 29.8f)
			{
				shoot_statistics.speed_297++;
			}
			else if (s_speed >= 29.8f && s_speed <= 29.9f)
			{
				shoot_statistics.speed_298++;
			}
			else if (s_speed >= 29.9f && s_speed <= 30.0f)
			{
				shoot_statistics.speed_299++;
			}
			else if (s_speed >= 30.0f)
			{
				shoot_statistics.speed_300++;
				shoot_statistics.shoot_num--;
			}
			
			/*统计部分*/
			shoot_statistics.shoot_num++;
			
			if (cali_flag == 1)
			{
				shoot_statistics.mean = ( shoot_statistics.speed_270 * 27.0f + shoot_statistics.speed_271 * 27.1f + shoot_statistics.speed_272 * 27.2f + shoot_statistics.speed_273 * 27.3f + shoot_statistics.speed_274 * 27.4f + shoot_statistics.speed_275 * 27.5f + shoot_statistics.speed_276 * 27.6f + shoot_statistics.speed_277 * 27.7f + shoot_statistics.speed_278 * 27.8f + shoot_statistics.speed_279 * 27.9f \
				+	shoot_statistics.speed_280 * 28.0f + shoot_statistics.speed_281 * 28.1f + shoot_statistics.speed_282 * 28.2f + shoot_statistics.speed_283 * 28.3f + shoot_statistics.speed_284 * 28.4f + shoot_statistics.speed_285 * 28.5f + shoot_statistics.speed_286 * 28.6f + shoot_statistics.speed_287 * 28.7f + shoot_statistics.speed_288 * 28.8f + shoot_statistics.speed_289 * 28.9f \
				+ shoot_statistics.speed_290 * 29.0f + shoot_statistics.speed_291 * 29.1f + shoot_statistics.speed_292 * 29.2f + shoot_statistics.speed_293 * 29.3f + shoot_statistics.speed_294 * 29.4f + shoot_statistics.speed_295 * 29.5f + shoot_statistics.speed_296 * 29.6f + shoot_statistics.speed_297 * 29.7f + shoot_statistics.speed_298 * 29.8f + shoot_statistics.speed_299 * 29.9f ) \
				/ (float)shoot_statistics.shoot_num;
				
				// Calculate the variance
				float sum_of_squares = (shoot_statistics.speed_270 * ((27.0f - shoot_statistics.mean) * (27.0f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_271 * ((27.1f - shoot_statistics.mean) * (27.1f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_272 * ((27.2f - shoot_statistics.mean) * (27.2f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_273 * ((27.3f - shoot_statistics.mean) * (27.3f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_274 * ((27.4f - shoot_statistics.mean) * (27.4f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_275 * ((27.5f - shoot_statistics.mean) * (27.5f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_276 * ((27.6f - shoot_statistics.mean) * (27.6f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_277 * ((27.7f - shoot_statistics.mean) * (27.7f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_278 * ((27.8f - shoot_statistics.mean) * (27.8f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_279 * ((27.9f - shoot_statistics.mean) * (27.9f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_280 * ((28.0f - shoot_statistics.mean) * (28.0f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_281 * ((28.1f - shoot_statistics.mean) * (28.1f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_282 * ((28.2f - shoot_statistics.mean) * (28.2f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_283 * ((28.3f - shoot_statistics.mean) * (28.3f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_284 * ((28.4f - shoot_statistics.mean) * (28.4f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_285 * ((28.5f - shoot_statistics.mean) * (28.5f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_286 * ((28.6f - shoot_statistics.mean) * (28.6f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_287 * ((28.7f - shoot_statistics.mean) * (28.7f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_288 * ((28.8f - shoot_statistics.mean) * (28.8f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_289 * ((28.9f - shoot_statistics.mean) * (28.9f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_290 * ((29.0f - shoot_statistics.mean) * (29.0f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_291 * ((29.1f - shoot_statistics.mean) * (29.1f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_292 * ((29.2f - shoot_statistics.mean) * (29.2f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_293 * ((29.3f - shoot_statistics.mean) * (29.3f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_294 * ((29.4f - shoot_statistics.mean) * (29.4f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_295 * ((29.5f - shoot_statistics.mean) * (29.5f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_296 * ((29.6f - shoot_statistics.mean) * (29.6f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_297 * ((29.7f - shoot_statistics.mean) * (29.7f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_298 * ((29.8f - shoot_statistics.mean) * (29.8f - shoot_statistics.mean))) +
										 (shoot_statistics.speed_299 * ((29.9f - shoot_statistics.mean) * (29.9f - shoot_statistics.mean))) ;

				shoot_statistics.variance = sum_of_squares / (float)shoot_statistics.shoot_num;
		}
			
}
#endif
