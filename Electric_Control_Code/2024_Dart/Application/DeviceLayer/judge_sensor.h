#ifndef __JUDGE_SENSOR_H
#define __JUDGE_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "judge_infantrypotocol.h"

extern judge_info_t judge_info;
extern judge_sensor_t judge_sensor;

/**
 *	@brief	����ϵͳ���ݼ��
 *  
 */
typedef __packed struct
{
   uint8_t dart_launch_opening_status;  //��ǰ���ڷ���ڵ�״̬ 1���ر� 2�����ڿ������߹ر��� 0���Ѿ�����
   uint8_t dart_attack_target;     		//���ڴ�ʱѡ���Ļ���Ŀ�꣬����Ĭ�ϻ�δѡ��/ѡ��ǰ��վʱΪ0��ѡ�л��ع̶�Ŀ��Ϊ1ѡ�л������Ŀ��Ϊ 2
   uint16_t target_change_time;    		//�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ�룬��δ�л�Ĭ��Ϊ 0��
   uint16_t operate_launch_cmd_time;   	//���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��, ��ʼֵΪ 0
   uint8_t dart_remaining_time;    		//���ڷ���ڵ���ʱ
   uint8_t game_type : 4;          		//��ǰ�������ͣ�1��RoboMaster���״�ʦ����2��RoboMaster���״�ʦ��������
   uint8_t game_progress : 4;      		//��ǰ�����׶Σ�4����ս�У�
   uint16_t stage_remain_time;   		//��ǰ�׶�ʣ��ʱ�䣬��λs
   uint8_t robot_id;             		//��������id��8���췽���ڻ����ˣ�108���������ڻ�����
   uint16_t red_outpost_HP;        		//�췽ǰ��վѪ��
   uint16_t red_base_HP;           		//�췽����Ѫ��
   uint16_t blue_outpost_HP;       		//����ǰ��վѪ��
   uint16_t blue_base_HP;         		//��������Ѫ��
} referee_data_t;

extern referee_data_t referee_data;
/* Exported functions --------------------------------------------------------*/


#endif
