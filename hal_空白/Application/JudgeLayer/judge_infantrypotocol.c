#include "judge_infantrypotocol.h"

#include "judge_sensor.h"
#include "crc.h"
#include "string.h"
#include "drv_uart.h"
#include "usart.h"

#include "vision_potocol.h"
#include "vision_sensor.h"
#define JUDGE_FRAME_HEADER	0xA5  //��Ϊ֡ͷ��SOF,֡ͷ��Ϊ SOF,length,Seq,CRC8
//?????????????????????????????????????
char rescue_sentry_Find[5] = "Find:";
char rescue_sentry_Find_hero[4] = "hero";
char rescue_sentry_Find_infantry[8] = "infantry";
char rescue_sentry_stealed_tower[10] = "Warning!!!";
float ttr1qq,ttr2qq;
float storage_radar_detection[5][4];//��������,���һ�зŸ���ʱ��

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

enum //����ͨ��
{
	//�ڱ�����֧Ԯ
	infantry3_Rescue_sentryID = 0x0201,
	infantry4_Rescue_sentryID = 0x0202,
	infantry5_Rescue_sentryID = 0x0203,
	hero_Rescue_sentryID      = 0x0204,
	engineer_Rescue_sentryID  = 0x0205,
};

typedef enum 
{
	RED_radar = 1,
	BLUE_radar = 2,
	
	type_position = 0,
	x_position = 1,
	y_position = 2,
	time_position = 3,//��������ϵͳ��˳��
	
	get_x_position = 0,
	get_y_position = 1,
	get_color_position = 3,
	get_number_position = 2//�ӵ��Եõ����ݵ�˳��
}radar_target;
radar_target enemy_radar_target;

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
int qq;
void Determine_ID(void);
void judge_sensor_update(judge_sensor_t *judge_sen, uint8_t *rxBuf)
{
	uint8_t  res = false;
	uint16_t frame_length;
	uint16_t cmd_id;
  uint16_t data_cmd_id;
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
//					case 0x0200:
//					memcpy(&judge_info->ext, (rxBuf+DATA_SEG), 2);
//						break;
//					
					case ID_robot_interactive_header_data:
					{
						data_cmd_id = (rxBuf[DATA_SEG+1] << 8 | rxBuf[DATA_SEG]);
						switch (data_cmd_id)
						{
							case infantry3_Rescue_sentryID:
							case infantry4_Rescue_sentryID:
							case infantry5_Rescue_sentryID:
							case hero_Rescue_sentryID:
							case engineer_Rescue_sentryID:
								memcpy(&judge_info->sentry_Info_rescue,(rxBuf+DATA_SEG+LEN_CMD_ID),LEN_SENTRY_INFO);
							break;
						}
					}
					
					case ID_game_state: 
						memcpy(&judge_info->game_status, (rxBuf+DATA_SEG), LEN_GAME_STATUS);
						break;
					
					case ID_game_result: 
						memcpy(&judge_info->game_result, (rxBuf+DATA_SEG), LEN_GAME_RESULT);
						break;
					
					case ID_game_robot_HP: 
						memcpy(&judge_info->game_robot_HP, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_HP);
						break;//���л����˵�Ѫ�������Ի��
					
//					case ID_dart_status: 
//						memcpy(&judge_info->dart_status, (rxBuf+DATA_SEG), LEN_DART_STATUS);
//						break;
//					
					case ID_ICRA_buff_debuff_zone_status:
						memcpy(&judge_info->ICRA_buff,(rxBuf+DATA_SEG),LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS);
						break;
					
					case ID_event_data: 
						memcpy(&judge_info->event_data, (rxBuf+DATA_SEG), LEN_EVENT_DATA);
						break;
					
					case ID_supply_projectile_action: 
						memcpy(&judge_info->supply_projectile_action, (rxBuf+DATA_SEG), LEN_SUPPLY_PROJECTILE_ACTION);
						break;
					
					case ID_referee_warning: 
						memcpy(&judge_info->referee_warning, (rxBuf+DATA_SEG), LEN_REFEREE_WARNING);
						break;
					
					case ID_dart_remaining_time: 
						memcpy(&judge_info->dart_remaining_time, (rxBuf+DATA_SEG), LEN_DART_REMAINING_TIME);
						break;
						
					case ID_game_robot_state: 
						memcpy(&judge_info->game_robot_status, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_STATUS);
						Determine_ID();
						break;
					
					case ID_power_heat_data: 
						memcpy(&judge_info->power_heat_data, (rxBuf+DATA_SEG), LEN_POWER_HEAT_DATA);
						break;
					
					case ID_game_robot_pos: 
						memcpy(&judge_info->game_robot_pos, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_POS);
						break;
					
					case ID_buff_musk: 
						memcpy(&judge_info->buff, (rxBuf+DATA_SEG), LEN_BUFF_MASK);
						break;
					
					case ID_aerial_robot_energy: 
						memcpy(&judge_info->aerial_robot_energy, (rxBuf+DATA_SEG), LEN_AERIAL_ROBOT_ENERGY);
						break;
					
					case ID_robot_hurt: 
						memcpy(&judge_info->robot_hurt, (rxBuf+DATA_SEG), LEN_ROBOT_HURT);
						break;
					
					case ID_shoot_data: 
						memcpy(&judge_info->shoot_data, (rxBuf+DATA_SEG), LEN_SHOOT_DATA);
						break;
					
					case ID_bullet_remaining: 
						memcpy(&judge_info->bullet_remaining, (rxBuf+DATA_SEG), LEN_BULLET_REMAINING);
						break;
					
					case ID_rfid_status: 
						memcpy(&judge_info->rfid_status, (rxBuf+DATA_SEG), LEN_RFID_STATUS);
						break;
						
					case ID_dart_client_directive:
						memcpy(&judge_info->dart_client,(rxBuf+DATA_SEG),LEN_DART_CLIENT_DIRECTIVE);
						break;
					
//					case ID_COMMUNICATION: 
//						//JUDGE_ReadFromCom();
//						break;
				}
			}
		}
		
		/* ֡βCRC16��һ�ֽ��Ƿ�Ϊ0xA5 */
		if(rxBuf[frame_length] == JUDGE_FRAME_HEADER)
		{
			/* ���һ�����ݰ������˶�֡���ݾ��ٴζ�ȡ */
			judge_sensor_update( judge_sen, &rxBuf[frame_length] );
		}
	}
	
	judge_info->data_valid = res;
	if(judge_info->data_valid != true)
		judge_info->err_cnt++;
	
	// ���յ����ݱ�ʾ����
	judge_info->offline_cnt = 0;
}
void Determine_ID(void)//�ж��Լ����ĸ�����
{
	if(judge_info.game_robot_status.robot_id < 10)//�������˵�ID���췽
	{ 
		judge_info.ids.teammate_hero 		 	= 1;
		judge_info.ids.teammate_engineer  = 2;
		judge_info.ids.teammate_infantry3 = 3;
		judge_info.ids.teammate_infantry4 = 4;
		judge_info.ids.teammate_infantry5 = 5;
		judge_info.ids.teammate_plane		 	= 6;
		judge_info.ids.teammate_sentry		= 7;
		
		judge_info.ids.client_hero 		 	= 0x0101;
		judge_info.ids.client_engineer  = 0x0102;
		judge_info.ids.client_infantry3 = 0x0103;
		judge_info.ids.client_infantry4 = 0x0104;
		judge_info.ids.client_infantry5 = 0x0105;
		judge_info.ids.client_plane			= 0x0106;
		
		if     (judge_info.game_robot_status.robot_id == hero_red)//����ˢ�·����ڱ����и�����ɫ
			judge_info.self_client = judge_info.ids.client_hero;
		else if(judge_info.game_robot_status.robot_id == engineer_red)
			judge_info.self_client = judge_info.ids.client_engineer;
		else if(judge_info.game_robot_status.robot_id == infantry3_red)
			judge_info.self_client = judge_info.ids.client_infantry3;
		else if(judge_info.game_robot_status.robot_id == infantry4_red)
			judge_info.self_client = judge_info.ids.client_infantry4;
		else if(judge_info.game_robot_status.robot_id == infantry5_red)
			judge_info.self_client = judge_info.ids.client_infantry5;
		else if(judge_info.game_robot_status.robot_id == plane_red)
			judge_info.self_client = judge_info.ids.client_plane;
	}
	else //����
	{
		judge_info.ids.teammate_hero 		 	= 101;
		judge_info.ids.teammate_engineer  = 102;
		judge_info.ids.teammate_infantry3 = 103;
		judge_info.ids.teammate_infantry4 = 104;
		judge_info.ids.teammate_infantry5 = 105;
		judge_info.ids.teammate_plane		 	= 106;
		judge_info.ids.teammate_sentry		= 107;
		
		judge_info.ids.client_hero 		 	= 0x0165;
		judge_info.ids.client_engineer  = 0x0166;
		judge_info.ids.client_infantry3 = 0x0167;
		judge_info.ids.client_infantry4 = 0x0168;
		judge_info.ids.client_infantry5 = 0x0169;
		judge_info.ids.client_plane			= 0x016A;
		
		if     (judge_info.game_robot_status.robot_id == hero_blue)
			judge_info.self_client = judge_info.ids.client_hero;
		else if(judge_info.game_robot_status.robot_id == engineer_blue)
			judge_info.self_client = judge_info.ids.client_engineer;
		else if(judge_info.game_robot_status.robot_id == infantry3_blue)
			judge_info.self_client = judge_info.ids.client_infantry3;
		else if(judge_info.game_robot_status.robot_id == infantry4_blue)
			judge_info.self_client = judge_info.ids.client_infantry4;
		else if(judge_info.game_robot_status.robot_id == infantry5_blue)
			judge_info.self_client = judge_info.ids.client_infantry5;
		else if(judge_info.game_robot_status.robot_id == plane_blue)
			judge_info.self_client = judge_info.ids.client_plane;
		
	}

}
/*********************************************/
uint8_t CliendTxBuffer[200];
uint8_t CliendTxBuffer_int[200];
uint8_t TeammateTxBuffer[200];
char first_line[30]  = {"readyfire:"};//�Ƿ�������,����30���ַ�����bool
char second_line[30] = {"      top:"};//С����
char third_line[30]  = {" auto_aim:"};//����
char fourth_line[30] = {"    twist:"};//Ť��
char fifth_line[30]  = {":bullet_sum"};//������int
char sixth_line[30]  = {":bullet_speed"};//����int
char seventh_line[30]= {":supercapacitor"};//��������ʣ����,float
char empty_line[30]  = {"                   "};
extern char str_try[7];
//*******************************���ַ���******************************/
ext_charstring_data_t tx_client_char;
uint8_t state_first_graphic;//0~7ѭ��
char tt_flag;
void Char_Graphic(ext_client_string_t* graphic,//����Ҫ����ȥ�������е����ݶ�����
									const char* name,
									uint32_t operate_tpye,
									
									uint32_t layer,
									uint32_t color,
									uint32_t size,
									uint32_t length,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,

									
									
									const char *character)//�ⲿ���������
{
	graphic_data_struct_t *data_struct = &graphic->grapic_data_struct;
	for(char i=0;i<3;i++)
		data_struct->graphic_name[i] = name[i];	//�ַ�����
	data_struct->operate_tpye = operate_tpye; //ͼ�����
	data_struct->graphic_tpye = CHAR;         //Char��
	data_struct->layer = layer;//���ڵ����
	data_struct->color = color;//���ǰ�ɫ
	data_struct->start_angle = size;
	data_struct->end_angle = length;	
	data_struct->width = width;
	data_struct->start_x = start_x;
	data_struct->start_y = start_y;	
	
	data_struct->radius = 0;
	data_struct->end_x = 0;
	data_struct->end_y = 0;
//	memcpy(graphic->data,empty_line,19);
	memcpy(graphic->data,character,length);
}

void Figure_Graphic(graphic_data_struct_t* graphic,//����Ҫ����ȥ����������ݶ�����
									const char* name,
									uint32_t operate_tpye,
									uint32_t graphic_tpye,//����ʲôͼ��
									uint32_t layer,
									uint32_t color,
									uint32_t start_angle,
									uint32_t end_angle,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
									uint32_t radius,
									uint32_t end_x,
									uint32_t end_y)							
{
	for(char i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];	//�ַ�����
	graphic->operate_tpye = operate_tpye; //ͼ�����
	graphic->graphic_tpye = graphic_tpye;         
	graphic->layer        = layer;//���ڵ�һ��
	graphic->color        = color;//��ɫ
	graphic->start_angle  = start_angle;
	graphic->end_angle    = end_angle;	
	graphic->width        = width;
	graphic->start_x      = start_x;
	graphic->start_y      = start_y;	
	graphic->radius = radius;
	graphic->end_x  = end_x;
	graphic->end_y  = end_y;
}


//******************���Ƹ�����*************************/
//�����ͼ��
ext_float_two_data_t tx_gimbal_angle;
int update_float_flag;
void Float_Graphic(Float_data_struct_t* graphic,//����Ҫ����ȥ����������ݶ�����
									const char* name,
									uint32_t operate_tpye,
									uint32_t graphic_tpye,//����ʲôͼ��
									uint32_t layer,
									uint32_t color,
									uint32_t size,
									uint32_t decimal,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
									int number)							
{
	for(char i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];	//�ַ�����
	graphic->operate_tpye = operate_tpye; //ͼ�����
	graphic->graphic_tpye = graphic_tpye;  
	graphic->layer        = layer;//
	graphic->color        = color;//��ɫ
	graphic->start_angle  = size;
	graphic->end_angle    = decimal;//С����Чλ	
	graphic->width        = width;
	graphic->start_x      = start_x;
	graphic->start_y      = start_y;	
	graphic->number       = number;
}
//**********************����int����***************************/
ext_int_two_data_t tx_bullet_int;
int update_int_flag;
void Int_Graphic(Int_data_struct_t* graphic,//����Ҫ����ȥ����������ݶ�����
									const char* name,
									uint32_t operate_tpye,
									uint32_t graphic_tpye,//����ʲôͼ��
									uint32_t layer,
									uint32_t color,
									uint32_t size,
									uint32_t zero,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
									int number)							
{
	for(char i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];	//�ַ�����
	graphic->operate_tpye = operate_tpye; //ͼ�����
	graphic->graphic_tpye = graphic_tpye;        
	graphic->layer        = layer;//���ڵ�һ��
	graphic->color        = color;//��ɫ
	graphic->start_angle  = size;
	graphic->end_angle    = zero;	
	graphic->width        = width;
	graphic->start_x      = start_x;
	graphic->start_y      = start_y;	
	graphic->number       = number;
}




//***********************************����ͨ��*************************************/
ext_car_data_t Car_communicate;

//void  car_radar_sentry_communicate(void)
//{
//			//֡ͷ
//		Car_communicate.txFrameHeader.sof = JUDGE_FRAME_HEADER;
//		Car_communicate.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(Car_communicate.angle_Array)+sizeof(Car_communicate.See_Info_Array);
//		Car_communicate.txFrameHeader.seq = 0;//�����
//		memcpy(CliendTxBuffer,&Car_communicate.txFrameHeader,sizeof(std_frame_header_t));
//		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��

//		//������
//		Car_communicate.CmdID = ID_robot_interactive_header_data;

//		//���ݶ�ͷ�ṹ
//		Car_communicate.dataFrameHeader.data_cmd_id = INTERACT_ID_radar_sentry_ex;
//		Car_communicate.dataFrameHeader.send_ID     = judge_info.game_robot_status.robot_id;
//		Car_communicate.dataFrameHeader.receiver_ID = judge_info.ids.teammate_sentry;//�����ڱ�
//	
//		//���ݶ�
//		Car_communicate.angle_Array[0] = vision_sensor.info->RxPacket.RxData.pitch_angle;
//		Car_communicate.angle_Array[1] = vision_sensor.info->RxPacket.RxData.yaw_angle;
//		Car_communicate.angle_Array[2] = vision_sensor.info->RxPacket.RxData.distance;
//	
//		Car_communicate.See_Info_Array[0] = vision_sensor.info->RxPacket.RxData.change_for;
//		Car_communicate.See_Info_Array[1] = vision_sensor.info->RxPacket.RxData.is_find_Target;
//		Car_communicate.See_Info_Array[2] = vision_sensor.info->RxPacket.RxData.is_find_Dafu;
//		Car_communicate.See_Info_Array[3] = vision_sensor.info->RxPacket.RxData.near_face;
//		Car_communicate.See_Info_Array[4] = vision_sensor.info->RxPacket.RxData.anti_top;
//		Car_communicate.See_Info_Array[5] = vision_sensor.info->RxPacket.RxData.anti_top_change_armor;
//	
//		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&Car_communicate.CmdID, LEN_CMD_ID+Car_communicate.txFrameHeader.data_length);//���������볤��2

//		//֡β
//		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(Car_communicate));
//		
//		HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(Car_communicate));
//}
/////////////////////////////////////////////////////////////////////////////////////////////
//����
//		judge_info.ids.teammate_hero 		 	= 101;
//		judge_info.ids.teammate_engineer  = 102;
//		judge_info.ids.teammate_infantry3 = 103;
//		judge_info.ids.teammate_infantry4 = 104;
//		judge_info.ids.teammate_infantry5 = 105;
//		judge_info.ids.teammate_plane		 	= 106;
//		judge_info.ids.teammate_sentry		= 107;
//�췽
//		judge_info.ids.teammate_hero 		 	= 1;
//		judge_info.ids.teammate_engineer  = 2;
//		judge_info.ids.teammate_infantry3 = 3;
//		judge_info.ids.teammate_infantry4 = 4;
//		judge_info.ids.teammate_infantry5 = 5;
//		judge_info.ids.teammate_plane		 	= 6;
//		judge_info.ids.teammate_sentry		= 7;

float temp_ID[5]={1,2,3,4,5};

void radar_Info_store()
{
	uint32_t get_time;
	uint32_t i,j;
	get_time = 	HAL_GetTick();
	for (i=0;i<10;i++)//ÿ�ĸ�Ϊһ�飬1-x,2-y,3-��ɫ��4-���ֱ���
	{
		if(vision_sensor.info->RxPacket.RxData.jiesuan[i*4+get_color_position] == enemy_radar_target)//�ж���ɫ
		{
			for(j = 0;j<5;j++)
			{
				if(vision_sensor.info->RxPacket.RxData.jiesuan[i*4+get_number_position] == temp_ID[j])//�ж�����
				{
					storage_radar_detection[j][x_position] = vision_sensor.info->RxPacket.RxData.jiesuan[i*4+get_x_position]/1000.f;
					storage_radar_detection[j][y_position] = vision_sensor.info->RxPacket.RxData.jiesuan[i*4+get_y_position]/1000.f;
					storage_radar_detection[j][time_position] = get_time;
					break;
				}
			}
		}
		if(vision_sensor.info->RxPacket.RxData.jiesuan[(i+1)*4+get_x_position] == 0
			&& vision_sensor.info->RxPacket.RxData.jiesuan[(i+1)*4+get_y_position] == 0)//û��ʶ�𵽳����˺���ȫ��0����ѭ��
			break;
	}
}

radar_command_t radar_send_place;
float anglegg;//101
void radar_communicate_(float* radar_Info)//����
{
	uint16_t tmp;
	radar_send_place.txFrameHeader.sof = JUDGE_FRAME_HEADER;
	radar_send_place.txFrameHeader.data_length =  sizeof(ext_client_map_command_t);
	radar_send_place.txFrameHeader.seq = 0;//�����
	memcpy(CliendTxBuffer,&radar_send_place.txFrameHeader,sizeof(std_frame_header_t));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��

	radar_send_place.CmdID = ID_smallmap;
	
	//���ݶΣ�
	radar_send_place.clientData.target_position_x = *(radar_Info+x_position);
	radar_send_place.clientData.target_position_y = *(radar_Info+y_position);
	
	tmp = (uint16_t)*(radar_Info+type_position);
	radar_send_place.clientData.target_robot_ID = tmp;//����Ӣ��
	
	radar_send_place.clientData.toward_Angle = anglegg;
	memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&radar_send_place.CmdID, LEN_CMD_ID+radar_send_place.txFrameHeader.data_length);//���������볤��2
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(radar_send_place));
	
	HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(radar_send_place));
}
//	get_x_position = 0,
//	get_y_position = 1,
//	get_color_position = 2,
//	get_number_position = 3//�ӵ��Եõ����ݵ�˳��
void try_see_point()
{
	storage_radar_detection[0][x_position] = 14.1;
	storage_radar_detection[0][y_position] = 11;
	storage_radar_detection[1][x_position] = 3;
	storage_radar_detection[1][y_position] = 10;
	

}


void radar_runing()//һֱ��
{
	uint32_t get_time;
	char i;
	static char j;
	get_time = HAL_GetTick();
	if(judge_info.game_robot_status.robot_id<10)//�����Ǻ췽
	{
		storage_radar_detection[0][type_position] = 101;//hero
		storage_radar_detection[1][type_position] = 102;//engineer
		storage_radar_detection[2][type_position] = 103;//infantry3 
		storage_radar_detection[3][type_position] = 104;//infantry4
		storage_radar_detection[4][type_position] = 105;//infantry5
		enemy_radar_target = BLUE_radar;
	}
	else //����������
	{
		storage_radar_detection[0][type_position] = 1;//hero
		storage_radar_detection[1][type_position] = 2;//engineer
		storage_radar_detection[2][type_position] = 3;//infantry3 
		storage_radar_detection[3][type_position] = 4;//infantry4
		storage_radar_detection[4][type_position] = 5;//infantry5
		enemy_radar_target = RED_radar;
	}
//	try_see_point();
	for(i = 0;i<5;i++)//�ܹ�����
	{
		if(get_time - storage_radar_detection[i][time_position]>=800)//����5��û�и�������
		{
			storage_radar_detection[i][x_position] = 0;
			storage_radar_detection[i][y_position] = 0;
		}
	}
	radar_communicate_(storage_radar_detection[j]);//j = 0~4
	j++;
	if(j>4)
		j=0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//���̳�

char task_line_1[9] = {"Wait"};
char task_line_2[30] = {"       "};

ext_charstring_data_t engineer_tx_client_char;
int global_engineer_task;
char engineer_task_updateflag,engineer_bool_updateflag,engineer_condition_updateflag;
static void engineer_draw_char(bool engineer_task)
{
	if(engineer_task == false)//normal                                            �����         size
		Char_Graphic(&engineer_tx_client_char.clientData,"ENO",ADD,0,FUCHSIA,30,strlen(task_line_1),3,  1920-250,(1080*9/12),  task_line_1);
	else if(engineer_task == true)//mine
		Char_Graphic(&engineer_tx_client_char.clientData,"ENO",DELETE,0,FUCHSIA,30,strlen(task_line_1),3,  1920-250,(1080*9/12),  task_line_1);
}
void Client_string_taskstate()
{
		//֡ͷ
		engineer_tx_client_char.txFrameHeader.sof = JUDGE_FRAME_HEADER;
		engineer_tx_client_char.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(ext_client_string_t);
		engineer_tx_client_char.txFrameHeader.seq = 0;//�����
		memcpy(CliendTxBuffer,&engineer_tx_client_char.txFrameHeader,sizeof(std_frame_header_t));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��
	
		//������
		engineer_tx_client_char.CmdID = ID_robot_interactive_header_data;
		
		//���ݶ�ͷ�ṹ
		engineer_tx_client_char.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_char_graphic;
		engineer_tx_client_char.dataFrameHeader.send_ID     = judge_info.game_robot_status.robot_id;
		engineer_tx_client_char.dataFrameHeader.receiver_ID = judge_info.self_client;
		
		//���ݶ�
		engineer_draw_char(global_engineer_task);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&engineer_tx_client_char.CmdID, LEN_CMD_ID+engineer_tx_client_char.txFrameHeader.data_length);//���������볤��2
		
		//֡β
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(engineer_tx_client_char));
		
		HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(engineer_tx_client_char));
}
//////
ext_graphic_five_data_t engineer_tx_client_Redgreen;
bool global_engineer_lock,global_engineer_top,global_engineer_middle,global_engineer_bottom,
	   global_engineer_comp_1,global_engineer_comp_2,global_engineer_comp_3;
char global_engineer_orientation,global_engineer_mine_color;

bool global_miner;
bool global_angle;
char global_liner;
void engineer_state(bool miner,bool angle,char liner)
{
	if(miner == true)//��ɫ����
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[0],"EG1",engineer_bool_updateflag,RECTANGLE,4,GREEN,0,0,5,  1920-320+60,730, 0,1920-320+90+60,730-90);
	else if(miner == false)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[0],"EG1",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,5,  1920-320+60,730, 0,1920-320+90+60,730-90);

	if(angle == true)
	{
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[1],"EG2",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320, 670-90,      0, 1920-320+60,670-80-90);
    Figure_Graphic(&engineer_tx_client_Redgreen.clientData[2],"EG3",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60,670-80-90, 0, 1920-320+60+60,670-80-90);
	}
	else if(angle == false)
	{
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[1],"EG2",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60, 670-90,      0, 1920-320+60,670-80-90);
    Figure_Graphic(&engineer_tx_client_Redgreen.clientData[2],"EG3",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60,670-80-90, 0, 1920-320+60+60,670-80-90);		
	}
	if(liner == 0)
	{
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[3],"EG4",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+50+50, 390,      0, 1920-320+60+50+90,390);
    Figure_Graphic(&engineer_tx_client_Redgreen.clientData[4],"EG5",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60,360, 0, 1920-320+60+90,360);		
	}
	else if(liner == 1)
	{
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[3],"EG4",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60, 390,      0, 1920-320+60+90,390);
    Figure_Graphic(&engineer_tx_client_Redgreen.clientData[4],"EG5",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60,360, 0, 1920-320+60+90,360);		
	}
	else if(liner == 2)
	{
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[3],"EG4",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60+90, 390,      0, 1920-320+60+90+90,390);
    Figure_Graphic(&engineer_tx_client_Redgreen.clientData[4],"EG5",engineer_bool_updateflag,LINE,4,FUCHSIA,0,0,5,  1920-320+60,360, 0, 1920-320+60+90,360);			
	}
		
		//	else if(clock_lock == false)//�Ϻ�ɫȦȦ
//		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[0],"EG1",engineer_bool_updateflag,CIRCLE,4,FUCHSIA,0,0,5,  200,1080*9/12, 20,0,0);
}
static void engineer_location_mine(bool top,bool middle,bool bottom)
{
	if(top == true)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[1],"EG2",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,  200+20-70,520+230,0,200+20,520+160);
	else if(top == false)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[1],"EG2",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,  200+20-70,520+230,0,200+20,520+160);
	if(middle == true)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[2],"EG3",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,  200+20-70,520+150,0,200+20,520+80);
	else if(middle == false)	
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[2],"EG3",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,  200+20-70,520+150,0,200+20,520+80);
	if(bottom == true)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[3],"EG4",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,  200+20-70,520+70, 0,200+20,520);
	else if(bottom == false)	
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[3],"EG4",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,  200+20-70,520+70, 0,200+20,520);

}
static void engineer_small_computer(bool comp_1)
{
	if(comp_1 == true)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[4],"EG5",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,1920-300,675,0,1920-210,585);
	else if(comp_1 == false)
		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[4],"EG5",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,1920-300,675,0,1920-210,585);
//	if(comp_2 == true)
//		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[5],"EG6",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,1920-200,675,0,1920-200+90,585);
//	else if(comp_2 == false)
//		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[5],"EG6",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,1920-200,675,0,1920-200+90,585);
//	if(comp_3 == true)
//		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[6],"EG7",engineer_bool_updateflag,RECTANGLE,4,GREEN,  0,0,10,1920-100,675,0,1920-100+90,585);
//	else if(comp_3 == false)	
//		Figure_Graphic(&engineer_tx_client_Redgreen.clientData[6],"EG7",engineer_bool_updateflag,RECTANGLE,4,FUCHSIA,0,0,10,1920-100,675,0,1920-100+90,585);
}

//void Draw_engineer_bool(void)
//{
//	engineer_lock_state(global_engineer_lock);
//	engineer_location_mine(global_engineer_top,global_engineer_middle,global_engineer_bottom);
//	engineer_small_computer(global_engineer_comp_1);
//}

void Client_engineer_Redgreen()//���ͼ��һ�����
{
		//֡ͷ
		engineer_tx_client_Redgreen.txFrameHeader.sof = JUDGE_FRAME_HEADER;
		engineer_tx_client_Redgreen.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(graphic_data_struct_t)*5;
		engineer_tx_client_Redgreen.txFrameHeader.seq = 0;//�����
		memcpy(CliendTxBuffer,&engineer_tx_client_Redgreen.txFrameHeader,sizeof(std_frame_header_t));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��

		//������
		engineer_tx_client_Redgreen.CmdID = ID_robot_interactive_header_data;

		//���ݶ�ͷ�ṹ
		engineer_tx_client_Redgreen.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_five_graphic;
		engineer_tx_client_Redgreen.dataFrameHeader.send_ID     = judge_info.game_robot_status.robot_id;
		engineer_tx_client_Redgreen.dataFrameHeader.receiver_ID = judge_info.self_client;
	
		//���ݶ�
		engineer_state(global_miner,global_angle,global_liner);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&engineer_tx_client_Redgreen.CmdID, LEN_CMD_ID+engineer_tx_client_Redgreen.txFrameHeader.data_length);//���������볤��2

		//֡β
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(engineer_tx_client_Redgreen));
		
		HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(engineer_tx_client_Redgreen));
}
ext_graphic_two_data_t engineer_tx_client_Redgreen_2;
void Draw_engineer_bool_2(bool comp_2,bool comp_3)
{
	if(comp_2 == true)
		Figure_Graphic(&engineer_tx_client_Redgreen_2.clientData[0],"EG6",engineer_bool_updateflag,RECTANGLE,1,GREEN,  0,0,10,1920-200,675,0,1920-200+90,585);
	else if(comp_2 == false)
		Figure_Graphic(&engineer_tx_client_Redgreen_2.clientData[0],"EG6",engineer_bool_updateflag,RECTANGLE,1,FUCHSIA,0,0,10,1920-200,675,0,1920-200+90,585);
	if(comp_3 == true)
		Figure_Graphic(&engineer_tx_client_Redgreen_2.clientData[1],"EG7",engineer_bool_updateflag,RECTANGLE,1,GREEN,  0,0,10,1920-100,675,0,1920-100+90,585);
	else if(comp_3 == false)	
		Figure_Graphic(&engineer_tx_client_Redgreen_2.clientData[1],"EG7",engineer_bool_updateflag,RECTANGLE,1,FUCHSIA,0,0,10,1920-100,675,0,1920-100+90,585);
}
void Client_2engineer_Redgreen(void)//����ͼ��һ�����
{
		//֡ͷ
		engineer_tx_client_Redgreen_2.txFrameHeader.sof = JUDGE_FRAME_HEADER;
		engineer_tx_client_Redgreen_2.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(graphic_data_struct_t)*2;
		engineer_tx_client_Redgreen_2.txFrameHeader.seq = 0;//�����
		memcpy(CliendTxBuffer,&engineer_tx_client_Redgreen_2.txFrameHeader,sizeof(std_frame_header_t));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��

		//������
		engineer_tx_client_Redgreen_2.CmdID = ID_robot_interactive_header_data;

		//���ݶ�ͷ�ṹ
		engineer_tx_client_Redgreen_2.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_two_graphic;
		engineer_tx_client_Redgreen_2.dataFrameHeader.send_ID     = judge_info.game_robot_status.robot_id;
		engineer_tx_client_Redgreen_2.dataFrameHeader.receiver_ID = judge_info.self_client;
	
		//���ݶ�
		Draw_engineer_bool_2(global_engineer_comp_2,global_engineer_comp_3);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&engineer_tx_client_Redgreen_2.CmdID, LEN_CMD_ID+engineer_tx_client_Redgreen_2.txFrameHeader.data_length);//���������볤��2

		//֡β
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(engineer_tx_client_Redgreen_2));
		
		HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(engineer_tx_client_Redgreen_2));
}
////
ext_graphic_five_data_t engineer_tx_client_condition;

static void engineer_orientation_graphic(char orientation)//�ڶ�ͼ��
{
	
	if(orientation == 0)//����
	{
		Figure_Graphic(&engineer_tx_client_condition.clientData[0],"ET1",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,790,0,1920-200+45,740);
		Figure_Graphic(&engineer_tx_client_condition.clientData[1],"ET2",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,790,0,1920-200+45-30,790-25);
		Figure_Graphic(&engineer_tx_client_condition.clientData[2],"ET3",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,790,0,1920-200+45+30,790-25);
	}
	else if(orientation == 1)//����	
	{
		Figure_Graphic(&engineer_tx_client_condition.clientData[0],"ET1",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,790,0,1920-200+45,740);
		Figure_Graphic(&engineer_tx_client_condition.clientData[1],"ET2",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,740,0,1920-200+45-30,740+25);
		Figure_Graphic(&engineer_tx_client_condition.clientData[2],"ET3",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+45,740,0,1920-200+45+30,740+25);	
	}
	else if(orientation == 2)//����
	{
		Figure_Graphic(&engineer_tx_client_condition.clientData[0],"ET1",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+20,765,0,1920-200+90-20,765);
		Figure_Graphic(&engineer_tx_client_condition.clientData[1],"ET2",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+20+25,765+30,0,1920-200+90-20,765);
		Figure_Graphic(&engineer_tx_client_condition.clientData[2],"ET3",engineer_condition_updateflag,LINE,2,CYAN_BLUE,0,0,10,1920-200+20+25,765-30,0,1920-200+90-20,765);	
	}
	Figure_Graphic(&engineer_tx_client_condition.clientData[3],"ET4",engineer_condition_updateflag,RECTANGLE,2,WHITE,      0,0,10,1920-200,810,0,1920-200+90,720);	
}
static void engineer_mine_color(char mine_color)
{
	if(mine_color == 0)//����ɫ
		Figure_Graphic(&engineer_tx_client_condition.clientData[4],"ET5",engineer_condition_updateflag,RECTANGLE,2,YELLOW, 0,0,10,50,720+45,0,50+90,720-45);	
	else if(mine_color == 1)//�����ɫ
		Figure_Graphic(&engineer_tx_client_condition.clientData[4],"ET5",engineer_condition_updateflag,RECTANGLE,2,WHITE, 0,0,10,50,720+45,0,50+90,720-45);
	else if(mine_color == 2)//�ؿ���ɫ���ݵص���ɫ
		Figure_Graphic(&engineer_tx_client_condition.clientData[4],"ET5",engineer_condition_updateflag,RECTANGLE,2,GREEN, 0,0,10,50,720+45,0,50+90,720-45);
	else if(mine_color == 3)//�һ����ɫ��Ǯ����ɫ	
		Figure_Graphic(&engineer_tx_client_condition.clientData[4],"ET5",engineer_condition_updateflag,RECTANGLE,2,PINK, 0,0,10,50,720+45,0,50+90,720-45);
	else if(mine_color == 4)//�������ɫ
		Figure_Graphic(&engineer_tx_client_condition.clientData[4],"ET5",engineer_condition_updateflag,RECTANGLE,2,ORANGE, 0,0,10,50,720+45,0,50+90,720-45);

}
void Draw_engineer_condition(void)
{
	engineer_orientation_graphic(global_engineer_orientation);
	engineer_mine_color(global_engineer_mine_color);
}
void Client_engineer_condition()//���ͼ��һ�����
{
		//֡ͷ
		engineer_tx_client_condition.txFrameHeader.sof = JUDGE_FRAME_HEADER;
		engineer_tx_client_condition.txFrameHeader.data_length = sizeof(ext_client_data_header_t) + sizeof(graphic_data_struct_t)*5;
		engineer_tx_client_condition.txFrameHeader.seq = 0;//�����
		memcpy(CliendTxBuffer,&engineer_tx_client_condition.txFrameHeader,sizeof(std_frame_header_t));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(std_frame_header_t));//ͷУ��

		//������
		engineer_tx_client_condition.CmdID = ID_robot_interactive_header_data;

		//���ݶ�ͷ�ṹ
		engineer_tx_client_condition.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_five_graphic;
		engineer_tx_client_condition.dataFrameHeader.send_ID     = judge_info.game_robot_status.robot_id;
		engineer_tx_client_condition.dataFrameHeader.receiver_ID = judge_info.self_client;
	
		//���ݶ�
		Draw_engineer_condition();
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&engineer_tx_client_condition.CmdID, LEN_CMD_ID+engineer_tx_client_condition.txFrameHeader.data_length);//���������볤��2

		//֡β
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(engineer_tx_client_condition));
		
		HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(engineer_tx_client_condition));
}
