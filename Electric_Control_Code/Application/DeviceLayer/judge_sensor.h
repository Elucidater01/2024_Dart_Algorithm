#ifndef __JUDGE_SENSOR_H
#define __JUDGE_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "judge_infantrypotocol.h"

extern judge_info_t judge_info;
extern judge_sensor_t judge_sensor;

/**
 *	@brief	裁判系统数据检查
 *  
 */
typedef __packed struct
{
   uint8_t dart_launch_opening_status;  //当前飞镖发射口的状态 1：关闭 2：正在开启或者关闭中 0：已经开启
   uint8_t dart_attack_target;     		//飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为0，选中基地固定目标为1选中基地随机目标为 2
   uint16_t target_change_time;    		//切换打击目标时的比赛剩余时间，单位秒，从未切换默认为 0。
   uint16_t operate_launch_cmd_time;   	//最近一次操作手确定发射指令时的比赛剩余时间，单位秒, 初始值为 0
   uint8_t dart_remaining_time;    		//飞镖发射口倒计时
   uint8_t game_type : 4;          		//当前比赛类型，1：RoboMaster机甲大师赛；2：RoboMaster机甲大师单项赛；
   uint8_t game_progress : 4;      		//当前比赛阶段，4：对战中；
   uint16_t stage_remain_time;   		//当前阶段剩余时间，单位s
   uint8_t robot_id;             		//本机器人id，8：红方飞镖机器人；108：蓝方飞镖机器人
   uint16_t red_outpost_HP;        		//红方前哨站血量
   uint16_t red_base_HP;           		//红方基地血量
   uint16_t blue_outpost_HP;       		//蓝方前哨站血量
   uint16_t blue_base_HP;         		//蓝方基地血量
} referee_data_t;

extern referee_data_t referee_data;
/* Exported functions --------------------------------------------------------*/


#endif
