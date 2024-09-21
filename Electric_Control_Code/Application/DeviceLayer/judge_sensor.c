/**
 * @file        judge_sensor.c
 * @author      MarkVimy
 * @Version     V1.0
 * @date        5-November-2020
 * @brief       Device Judge.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "judge_sensor.h"
#include "drv_uart.h"

/* Exported functions --------------------------------------------------------*/
extern void judge_sensor_init(judge_sensor_t *judge_sen);
extern void judge_sensor_update(judge_sensor_t *judge_sen, uint8_t *rxBuf);

/* Private macro -------------------------------------------------------------*/
#define JUDGE_OFFLINE_MAX_CNT	100
/* Private function prototypes -----------------------------------------------*/
static void judge_sensor_check(judge_sensor_t *judge_sen);
static void judge_sensor_heart_beat(judge_sensor_t *judge_sen);

/* Exported typedef -----------------------------------------------------------*/
referee_data_t referee_data;
/* Private variables ---------------------------------------------------------*/
// 裁判系统信息
judge_info_t 	judge_info = {
	.offline_max_cnt = JUDGE_OFFLINE_MAX_CNT,
};

// 裁判系统
judge_sensor_t	judge_sensor = {
	.info = &judge_info,
	.init = judge_sensor_init,
	.update = judge_sensor_update,
	.check = judge_sensor_check,
	.heart_beat = judge_sensor_heart_beat,
	.work_state = DEV_OFFLINE,
	.errno = NONE_ERR,
	.id = DEV_ID_JUDGE,
};
/* Exported variables --------------------------------------------------------*/
/*获取二进制某一位的值*/
uint8_t get_bit(uint8_t data, uint8_t bit)
{
	return (data >> bit) & 0x01;
}
/* Private functions ---------------------------------------------------------*/

static void judge_sensor_check(judge_sensor_t *judge_sen)
{
	judge_info_t *judge_info = judge_sen->info;
	
	if(judge_info->dart_client.dart_launch_opening_status <= 2 \
        && (judge_info->game_robot_status.robot_id == 8 \
            || judge_info->game_robot_status.robot_id == 108))
    {
        referee_data.dart_launch_opening_status = judge_info->dart_client.dart_launch_opening_status;
		/*bit 5-6:飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为 0，选中基地固定目标为 1，选中基地随机目标为 2*/
        referee_data.dart_attack_target = (uint8_t)((judge_info->dart_info.dart_info >> 5) & 0x03);
//	    referee_data.dart_attack_target = judge_info->dart_info.dart_info;		
        referee_data.operate_launch_cmd_time = judge_info->dart_client.operate_launch_cmd_time;
        referee_data.dart_remaining_time = judge_info->dart_info.dart_remaining_time;
        referee_data.game_type = judge_info->game_status.game_type;
        referee_data.game_progress = judge_info->game_status.game_progress;
        referee_data.stage_remain_time = judge_info->game_status.stage_remain_time;
        referee_data.robot_id = judge_info->game_robot_status.robot_id;
        referee_data.red_outpost_HP = judge_info->game_robot_HP.red_outpost_HP;
        referee_data.red_base_HP = judge_info->game_robot_HP.red_base_HP;
        referee_data.blue_outpost_HP = judge_info->game_robot_HP.blue_outpost_HP;
        referee_data.blue_base_HP = judge_info->game_robot_HP.blue_base_HP;
    }
}

/**
 *	@brief	裁判系统心跳包
 */
static void judge_sensor_heart_beat(judge_sensor_t *judge_sen)
{
	judge_info_t *judge_info = judge_sen->info;

	judge_info->offline_cnt++;
	if(judge_info->offline_cnt > judge_info->offline_max_cnt) {
		judge_info->offline_cnt = judge_info->offline_max_cnt;
		judge_sen->work_state = DEV_OFFLINE;
	} 
	else {
		/* 离线->在线 */
		if(judge_sen->work_state == DEV_OFFLINE)
			judge_sen->work_state = DEV_ONLINE;
	}
}

