#include "judge_infantrypotocol.h"

#include "judge_sensor.h"
#include "crc.h"
#include "string.h"
#include "drv_uart.h"
#include "usart.h"
#include "axis.h"
//#include "vision_potocol.h"
//#include "vision_sensor.h"
#define JUDGE_FRAME_HEADER	0xA5  //��Ϊ֡ͷ��SOF,֡ͷ��Ϊ SOF,length,Seq,CRC8

//char rescue_sentry_Find[5] = "Find:";
//char rescue_sentry_Find_hero[4] = "hero";
//char rescue_sentry_Find_infantry[8] = "infantry";
//char rescue_sentry_stealed_tower[10] = "Warning!!!";
//float ttr1qq,ttr2qq;
//float storage_radar_detection[5][4];//��������,���һ�зŸ���ʱ��


void USART5_rxDataHandler(uint8_t *rxBuf)
{	
	judge_sensor.update(&judge_sensor, rxBuf);
	judge_sensor.check(&judge_sensor);
}


/**********ƫ��λ��**********/
//֡�ֽڣ���λ/�ֽڣ�
enum judge_frame_offset_t {//֡ͷռ����ֽڣ�������ռ�����ֽڣ��ӵ��߸���ʼΪ����֡
	FRAME_HEADER	= 0,
	CMD_ID			= 5,
	DATA_SEG		= 7
};
//֡ͷ�ֽڣ���λ/bit��
enum judge_frame_header_offset_t {//֡ͷ����ϸλ��
	SOF			= 0,
	DATA_LENGTH	= 1,
	SEQ			= 3,
	CRC8		= 4
};//FRAME_HEADER



void judge_sensor_init(judge_sensor_t *judge_sen)
{
	judge_sen->info->offline_cnt = judge_sen->info->offline_max_cnt+1;
	judge_sen->work_state= DEV_OFFLINE;
	
	if(judge_sen->id == DEV_ID_JUDGE) {
		judge_sen->errno = NONE_ERR;
	}
	else {
		judge_sen->errno = DEV_ID_ERR;
	}
}
//int qq;
//void Determine_ID(void);
void judge_sensor_update(judge_sensor_t *judge_sen, uint8_t *rxBuf)
{
	uint8_t  res = false;
	uint16_t frame_length;
	uint16_t cmd_id;
//  uint16_t data_cmd_id;
	judge_info_t *judge_info = judge_sen->info;
	
	if( rxBuf == NULL )
	{
		judge_info->data_valid = false;
		return;
	}
	
	memcpy(&judge_info->fream_header, rxBuf, LEN_FRAME_HEAD);//5���ֽ�
	
	/* ֡���ֽ��Ƿ�Ϊ0xA5 */
	if(rxBuf[SOF] == JUDGE_FRAME_HEADER) 
	{
		/* ֡ͷCRC8У�� */
		if(Verify_CRC8_Check_Sum(rxBuf, LEN_FRAME_HEAD) == true) 
		{
			/* ͳ��һ֡�������ݳ��ȣ�����CRC16У�� */     // ���������ֽ�
			frame_length = LEN_FRAME_HEAD + LEN_CMD_ID + judge_info->fream_header.data_length + LEN_FRAME_TAIL;
			
			if(Verify_CRC16_Check_Sum(rxBuf, frame_length) == true)
			{
				res = true;
				
				cmd_id = (rxBuf[CMD_ID+1] << 8 | rxBuf[CMD_ID]);
				
				switch(cmd_id)
				{
					
					case ID_game_state: 
						memcpy(&judge_info->game_status, (rxBuf+DATA_SEG), LEN_GAME_STATUS);
						break;
					
					case ID_game_result: 
						memcpy(&judge_info->game_result, (rxBuf+DATA_SEG), LEN_GAME_RESULT);
						break;
					
					case ID_game_robot_HP: 
						memcpy(&judge_info->game_robot_HP, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_HP);
						break;//���л����˵�Ѫ�������Ի��
					
					
					case ID_dart_info: 
						memcpy(&judge_info->dart_info, (rxBuf+DATA_SEG), LEN_DART_INFO);
						break; //(2024���ģ�
						
					case ID_game_robot_state: 
						memcpy(&judge_info->game_robot_status, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_STATUS);
//						Determine_ID();
						break;
				
						
					case ID_dart_client_directive:
						memcpy(&judge_info->dart_client,(rxBuf+DATA_SEG),LEN_DART_CLIENT_DIRECTIVE);
						break;
					
				}
				/* ֡βCRC16��һ�ֽ��Ƿ�Ϊ0xA5 */
				if(rxBuf[frame_length] == JUDGE_FRAME_HEADER)
				{
					/* ���һ�����ݰ������˶�֡���ݾ��ٴζ�ȡ */
					judge_sensor_update( judge_sen, &rxBuf[frame_length] );
				}
			}
		}
	}
	
	judge_info->data_valid = res;
	if(judge_info->data_valid != true)
		judge_info->err_cnt++;
	
	// ���յ����ݱ�ʾ����
	judge_info->offline_cnt = 0;
}

extern char str_try[7];
