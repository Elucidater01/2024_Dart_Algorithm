#ifndef  __judge_infantrypotocol_h
#define  __judge_infantrypotocol_h
#include "rp_config.h"
//***********����ϵͳ�ķ�����Ϣ***************/
enum
{ 
	ID_game_state       						= 0x0001,//����״̬���ݣ�1Hz
	ID_game_result 	   							= 0x0002,//����������ݣ�������������
	ID_game_robot_HP       					= 0x0003,//����������Ѫ�����ݣ�1Hz����
//	ID_dart_status									= 0x0004,//���ڷ���״̬�����ڷ���ʱ����
	ID_ICRA_buff_debuff_zone_status = 0x0005,//�˹�������ս���ӳ���ͷ���״̬��1Hz
	ID_event_data  									= 0x0101,//�����¼����ݣ��¼� �����ı�󡪡� ����
	ID_supply_projectile_action   	= 0x0102,//���ز���վ������ʶ���ݣ����� �����ı�󡪡� ����
//	ID_supply_projectile_booking    = 0x0103,//���󲹸�վ�������� ���������ӷ��͡�����RM �Կ�����δ����10Hz
	ID_referee_warning					 		= 0x0104,//���о������ݣ���������󡪡�����
	ID_dart_remaining_time					= 0x0105,//���ڷ���ڵ���ʱ��1Hz
	ID_game_robot_state    					= 0x0201,//������״̬���ݣ�10Hz
	ID_power_heat_data    					= 0x0202,//ʵʱ�����������ݣ�50Hz
	ID_game_robot_pos        				= 0x0203,//������λ�����ݣ�10Hz
	ID_buff_musk										= 0x0204,//�������������ݣ�1Hz
	ID_aerial_robot_energy					= 0x0205,//���л���������״̬���ݣ�10Hz��ֻ�С������л��������ء�������
	ID_robot_hurt										= 0x0206,//�˺�״̬���ݣ��˺���������
	ID_shoot_data										= 0x0207,//ʵʱ������ݣ��ӵ��������
	ID_bullet_remaining							= 0x0208,//����ʣ�෢���������������л����ˣ��ڱ������ˡ����Լ�ICRA�����˷��ͣ�1Hz
	ID_rfid_status									= 0x0209,//������RFID״̬��1Hz
	
	ID_dart_client_directive        = 0x020A,//���ڻ����˿ͻ���ָ����, 10Hz
	
	ID_robot_interactive_header_data			= 0x0301,//�����˽������ݣ��������ͷ������������� 10Hz
	ID_controller_interactive_header_data = 0x0302,//�Զ���������������ݽӿڣ�ͨ�������ͻ��˴����������� 30Hz
	ID_map_interactive_header_data        = 0x0303,//�ͻ���С��ͼ�������ݣ������������͡���
	ID_keyboard_information               = 0x0304,//���̡������Ϣ��ͨ������ͼ�����ڡ�������
  ID_smallmap                           = 0x0305,//�ͻ���С��ͼ������Ϣ 
};
//������ö��CMD_ID
//**************����ϵͳ������Ϣ�ĳ��ȣ���λ/�ֽڣ�**************************/
enum judge_data_length_t {
	/* Std */
	LEN_FRAME_HEAD 	                 = 5,	// ֡ͷ����
	LEN_CMD_ID 		                   = 2,	// �����볤��
	LEN_FRAME_TAIL 	                 = 2,	// ֡βCRC16
	/* Ext */
	// 0x000x
	LEN_GAME_STATUS 				         = 11,
	LEN_GAME_RESULT 				         = 1,
	LEN_GAME_ROBOT_HP 			         = 32,
//	LEN_DART_STATUS					         = 3,
	LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS = 11,//0x0005
	// 0x010x
	LEN_EVENT_DATA					         = 4,
	LEN_SUPPLY_PROJECTILE_ACTION	   = 4,//����������������������������������
//	LEN_SUPPLY_PROJECTILE_BOOKING	   = 2,//�Կ���δ����
	LEN_REFEREE_WARNING				       = 2,
	LEN_DART_REMAINING_TIME		     	 = 1,//0x0105
	// 0x020x
	LEN_GAME_ROBOT_STATUS			       = 27,//15!!!!!!!!!!!!!!!!!!!!!!!!!!!
	LEN_POWER_HEAT_DATA 			       = 16,//��������������������
	LEN_GAME_ROBOT_POS				       = 16,
	LEN_BUFF_MASK		 				         = 1,
	LEN_AERIAL_ROBOT_ENERGY 	     	 = 1,//����������
	LEN_ROBOT_HURT				         	 = 1,
	LEN_SHOOT_DATA					         = 7,//��������
	LEN_BULLET_REMAINING	 		       = 6,//��������
	LEN_RFID_STATUS					         = 4,
	LEN_DART_CLIENT_DIRECTIVE        = 6,//0x020A

	// 0x030x
	//LEN_robot_interactive_header_data      = n,
	//LEN_controller_interactive_header_data = n,
	LEN_MAP_INTERACTIVE_HEADERDATA           = 15,
	LEN_KEYBOARD_INFORMATION                 = 12,//0x0304

		LEN_SENTRY_INFO                          = 1,
};//��2-4
/*******************����ϵͳ��Ϣ��������**************************/
//??????????????????????????????????
//������Ϣ
typedef __packed struct
{
	uint8_t find_enemy   :1;
	uint8_t find_hero    :1;
	uint8_t fine_infantry:2;//������Լӷ��ּ�������
	uint8_t stealed_tower:2;//��͵��
	uint8_t meaningless  :2;//����ӹ���
}ext_sentry_t;






typedef __packed struct
{
	uint8_t  sof;
	uint16_t data_length;
	uint8_t  seq;
	uint8_t  crc8;
} std_frame_header_t;//LEN_FRAME_HEAD

//LEN_CMD_ID
//LEN_FRAME_TAIL
/* ID: 0x0001	Byte: 	11	����״̬���� */
typedef __packed struct 
{ 
	uint8_t game_type     : 4;		// ��������
	uint8_t game_progress : 4;		// �����׶�
	uint16_t stage_remain_time;		// ��ǰ�׶�ʣ��ʱ��(��λ:s)
	uint64_t SyncTimeStamp;       //�����˽��յ���ָ��ľ�ȷ Unix ʱ�䣬�����ض��յ���Ч�� NTP ��������ʱ����Ч
} ext_game_status_t; //LEN_GAME_STATUS   ��3-1
/* ID: 0x0002	Byte:	1	����������� */
typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t; //LEN_GAME_RESULT   ��3-2
/* ID: 0x0003	Byte:	32	������Ѫ���������� */
typedef __packed struct 
{ 
	uint16_t red_1_robot_HP;	// ��1Ӣ�ۻ�����Ѫ��(δ�ϳ�������Ѫ��Ϊ0)
	uint16_t red_2_robot_HP;	// ��2���̻�����Ѫ��
	uint16_t red_3_robot_HP;	// ��3����������Ѫ��
	uint16_t red_4_robot_HP;	// ��4����������Ѫ��
	uint16_t red_5_robot_HP;	// ��5����������Ѫ��
	uint16_t red_7_robot_HP;	// ��7�ڱ�������Ѫ��
	uint16_t red_outpost_HP;	// �췽ǰ��վѪ��
	uint16_t red_base_HP;			// �췽����Ѫ��
	uint16_t blue_1_robot_HP;	// ��1Ӣ�ۻ�����Ѫ��
	uint16_t blue_2_robot_HP;	// ��2���̻�����Ѫ��
	uint16_t blue_3_robot_HP;	// ��3����������Ѫ��
	uint16_t blue_4_robot_HP;	// ��4����������Ѫ��
	uint16_t blue_5_robot_HP;	// ��5����������Ѫ��
	uint16_t blue_7_robot_HP;	// ��7�ڱ�������Ѫ��
	uint16_t blue_outpost_HP;	// ����ǰ��վѪ��
	uint16_t blue_base_HP;		// ��������Ѫ��	
} ext_game_robot_HP_t; //LEN_GAME_ROBOT_HP  ��3-3
///* ID: 0x0004 	Byte:	3	���ڷ���״̬ */
//typedef __packed struct
//{
//	uint8_t dart_belong;
//	uint16_t stage_remaining_time;
//} ext_dart_status_t;//LEN_DART_STATUS   ��3-4
/* ID: 0x0005 	Byte:	11	�˹�������ս���ӳ���ͷ���״̬ */
typedef __packed struct
{
	uint8_t F1_zone_status:1;            //����״̬
	uint8_t F1_zone_buff_debuff_status:3;//�췽��Ѫ��
	uint8_t F2_zone_status:1;
	uint8_t F2_zone_buff_debuff_status:3;//�췽��ҩ������
	uint8_t F3_zone_status:1;
	uint8_t F3_zone_buff_debuff_status:3;//������Ѫ��
	uint8_t F4_zone_status:1;
	uint8_t F4_zone_buff_debuff_status:3;//������ҩ������
	uint8_t F5_zone_status:1;
	uint8_t F5_zone_buff_debuff_status:3;//��ֹ�����
	uint8_t F6_zone_status:1;
	uint8_t F6_zone_buff_debuff_status:3;//��ֹ�ƶ���
	uint16_t red1_bullet_left;           //�췽 1 ��ʣ�൯��
	uint16_t red2_bullet_left;           //�췽 2 ��ʣ�൯��
	uint16_t blue1_bullet_left;          //���� 1 ��ʣ�൯��
	uint16_t blue2_bullet_left;          //���� 2 ��ʣ�൯��
} ext_ICRA_buff_debuff_zone_status_t;//LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS  ��3-5
/* ID: 0x0101  Byte:  4    �����¼����� */
typedef __packed struct 
{ 
	uint32_t event_type;//��������վ��Ѫ��ռ��״̬,��������״̬, ���θߵ�ռ��״̬,�������ػ���״̬,����ǰ��ս״̬
} ext_event_data_t; //LEN_EVENT_DATA   ��3-6
/* ID: 0x0102  Byte:  4    ����վ������ʶ���� */
typedef __packed struct 
{ 
	uint8_t supply_projectile_id;  //����վ�� ID
	uint8_t supply_robot_id;       //���������� ID
	uint8_t supply_projectile_step;//�����ڿ���״̬
	uint8_t supply_projectile_num; //��������
} ext_supply_projectile_action_t;//LEN_SUPPLY_PROJECTILE_ACTION  ��3-7
//LEN_SUPPLY_PROJECTILE_BOOKING
/* ID: 0X0104  Byte:  2	   ���о�����Ϣ */
typedef __packed struct
{
	uint8_t level;        //����ȼ�
	uint8_t foul_robot_id;//��������� ID
} ext_referee_warning_t;//LEN_REFEREE_WARNING   ��3-8
/* ID: 0X0105  Byte:  1	   ���ڷ���ڵ���ʱ */
typedef __packed struct
{
	uint8_t dart_remaining_time;//15s ����ʱ
} ext_dart_remaining_time_t;//LEN_DART_REMAINING_TIME
/* ID: 0X0201  Byte: 27    ������״̬���� */
typedef __packed struct 
{ 
	uint8_t robot_id;                       //�������� ID
	uint8_t robot_level;                    //�����˵ȼ�
	uint16_t remain_HP;                     //������ʣ��Ѫ��
	uint16_t max_HP;                        //����������Ѫ��
	uint16_t shooter_id1_17mm_cooling_rate; //������ 1 �� 17mm ǹ��ÿ����ȴֵ
	uint16_t shooter_id1_17mm_cooling_limit;//������ 1 �� 17mm ǹ����������
	uint16_t shooter_id1_17mm_speed_limit;  //������ 1 �� 17mm ǹ�������ٶ� ��λ m/s
	uint16_t shooter_id2_17mm_cooling_rate; //������ 2 �� 17mm ǹ��ÿ����ȴֵ
	uint16_t shooter_id2_17mm_cooling_limit;//������ 2 �� 17mm ǹ����������
	uint16_t shooter_id2_17mm_speed_limit;  //������ 2 �� 17mm ǹ�������ٶ� ��λ m/s
	uint16_t shooter_id1_42mm_cooling_rate; //������ 42mm ǹ��ÿ����ȴֵ
	uint16_t shooter_id1_42mm_cooling_limit;//������ 42mm ǹ����������
	uint16_t shooter_id1_42mm_speed_limit;  //������ 42mm ǹ�������ٶ� ��λ m/s
	uint16_t chassis_power_limit;           //�����˵��̹�����������
	//���ص�Դ������
	uint8_t mains_power_gimbal_output  : 1; //gimbal ������� 1 Ϊ�� 24V �����0 Ϊ�� 24v ���
	uint8_t mains_power_chassis_output : 1; //chassis �������1 Ϊ�� 24V �����0 Ϊ�� 24v ���
	uint8_t mains_power_shooter_output : 1; //shooter �������1 Ϊ�� 24V �����0 Ϊ�� 24v ���
} ext_game_robot_status_t; //LEN_GAME_ROBOT_STATUS  ��3-10
/* ID: 0X0202  Byte: 16    ʵʱ������������ */
typedef __packed struct 
{ 
	uint16_t chassis_volt;   				// ���������ѹ����λ��mV
	uint16_t chassis_current;				// ���������������λ��mA
	float chassis_power;   					// ����˲ʱ���ʣ���λ��W
	uint16_t chassis_power_buffer;	// ���̹��ʻ��壬��λ��60J������������(���¸��ݹ���������250J)
	uint16_t shooter_id1_17mm_cooling_heat; //1 �� 17mm ǹ������
	uint16_t shooter_id2_17mm_cooling_heat; //2 �� 17mm ǹ������
	uint16_t shooter_id1_42mm_cooling_heat;	//42mm ǹ������
} ext_power_heat_data_t; //LEN_POWER_HEAT_DATA   ��3-11
/* ID: 0x0203  Byte: 16    ������λ������ */
typedef __packed struct 
{   
	float x;   
	float y;   
	float z;   
	float yaw; //λ��ǹ��,��λ����
} ext_game_robot_pos_t; //LEN_GAME_ROBOT_POS   ��3-12
/* ID: 0x0204  Byte:  1    �������������� */
typedef __packed struct 
{ 
	uint8_t power_rune_buff; 
} ext_buff_t; //LEN_BUFF_MASK  ��3-13
/* ID: 0x0205  Byte:  1    ���л���������״̬���� */
typedef __packed struct 
{ 
	uint8_t attack_time;//�ɹ���ʱ�� ��λ s��30s �ݼ��� 0
} ext_aerial_robot_energy_t; //LEN_AERIAL_ROBOT_ENERGY  ��3-14
/* ID: 0x0206  Byte:  1    �˺�״̬���� */
typedef __packed struct 
{ 
	uint8_t armor_id : 4; 	// װ���˺�ʱ����װ��ID
	uint8_t hurt_type : 4; 	// 0x0װ���˺� 0x1ģ����� 0x2������ 0x3������ 0x4������ 0x5ײ��
} ext_robot_hurt_t; //LEN_ROBOT_HURT  ��3-15
/* ID: 0x0207  Byte:  7    ʵʱ������� */
typedef __packed struct 
{ 
	uint8_t bullet_type; 	// �ӵ�����(1-17mm, 2-42mm)
	uint8_t shooter_id;   // ������� ID��1��1 �� 17mm ���������2��2 �� 17mm ���������3��42mm ���������
	uint8_t bullet_freq;  // �ӵ���Ƶ(Hz)
	float bullet_speed;		// �ӵ�����(m/s)
} ext_shoot_data_t; //LEN_SHOOT_DATA  ��3-16
/* ID: 0x0208  Byte:  6    �ӵ�ʣ�෢�������� */
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;//17mm �ӵ�ʣ�෢����Ŀ
	uint16_t bullet_remaining_num_42mm;//42mm �ӵ�ʣ�෢����Ŀ
	uint16_t coin_remaining_num;       //ʣ��������
} ext_bullet_remaining_t; //LEN_BULLET_REMAINING   ��3-17
/* ID: 0x0209  Byte:  4 	������RFID״̬ */
typedef __packed struct
{
	uint32_t rfid_status;
} ext_rfid_status_t;//LEN_RFID_STATUS  ��3-18
/*ID��0x020A   Byte��6  ���ڻ����˿ͻ���ָ������*/
typedef struct{
	uint8_t dart_launch_opening_status;//��ǰ���ڷ���ڵ�״̬
	uint8_t dart_attack_target;        //���ڵĴ��Ŀ�꣬Ĭ��Ϊǰ��վ��1��ǰ��վ��2�����أ�
	uint16_t target_change_time;       //�л����Ŀ��ʱ�ı���ʣ��ʱ��
	uint16_t operate_launch_cmd_time;  //���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��
} ext_dart_client_cmd_t; //LEN_DART_CLIENT_DIRECTIVE  ��3-19
	/*******************************************************************************/
/*
	������ ID��
	1��Ӣ��(��)��
	2������(��)��
	3/4/5������(��)��
	6������(��)��
	7���ڱ�(��)��
	9���״�죩
	101��Ӣ��(��)��
	102������(��)��
	103/104/105������(��)��
	106������(��)��
	107���ڱ�(��)��
	109���״����

	�ͻ��� ID�� 
	0x0101 ΪӢ�۲����ֿͻ���(��) ��
	0x0102 Ϊ���̲����ֿͻ���( �� )��
	0x0103/0x0104/0x0105 Ϊ���������ֿͻ���(��)��
	0x0106 Ϊ���в����ֿͻ���((��)�� 

	0x0165��Ӣ�۲����ֿͻ���(��)��
	0x0166�����̲����ֿͻ���(��)��
	0x0167/0x0168/0x0169�����������ֿͻ���(��)��
	0x016A�����в����ֿͻ���(��)�� 
*/
enum judge_robot_ID{
	hero_red       = 1,
	engineer_red   = 2,
	infantry3_red  = 3,
	infantry4_red  = 4,
	infantry5_red  = 5,
	plane_red      = 6,
	
	hero_blue      = 101,
	engineer_blue  = 102,
	infantry3_blue = 103,
	infantry4_blue = 104,
	infantry5_blue = 105,
	plane_blue     = 106,
};
typedef struct{
	uint16_t teammate_hero;
	uint16_t teammate_engineer;
	uint16_t teammate_infantry3;
	uint16_t teammate_infantry4;
	uint16_t teammate_infantry5;
	uint16_t teammate_plane;
	uint16_t teammate_sentry;
	
	uint16_t client_hero;
	uint16_t client_engineer;
	uint16_t client_infantry3;
	uint16_t client_infantry4;
	uint16_t client_infantry5;
	uint16_t client_plane;
} ext_interact_id_t;


typedef struct judge_info_struct {
	std_frame_header_t							fream_header;				// ֡ͷ��Ϣ
	
	ext_game_status_t 							game_status;				// 0x0001
	ext_game_result_t 							game_result;				// 0x0002
	ext_game_robot_HP_t 						game_robot_HP;			// 0x0003
//	ext_dart_status_t								dart_status;				// 0x0004
	ext_ICRA_buff_debuff_zone_status_t	ICRA_buff;
	
	ext_event_data_t								event_data;					// 0x0101
	ext_supply_projectile_action_t	supply_projectile_action;		// 0x0102
	//ext_supply_projectile_booking_t supply_projectile_booking;// 0x0103
	ext_referee_warning_t						referee_warning;		// 0x0104
	ext_dart_remaining_time_t				dart_remaining_time;// 0x0105
	
	ext_game_robot_status_t					game_robot_status;	// 0x0201
	ext_power_heat_data_t						power_heat_data;		// 0x0202
	ext_game_robot_pos_t						game_robot_pos;			// 0x0203
	ext_buff_t											buff;								// 0x0204
	ext_aerial_robot_energy_t				aerial_robot_energy;// 0x0205
	ext_robot_hurt_t								robot_hurt;					// 0x0206
	ext_shoot_data_t								shoot_data;					// 0x0207
	ext_bullet_remaining_t					bullet_remaining;		// 0x0208	
	ext_rfid_status_t								rfid_status;				// 0x0209	
	ext_dart_client_cmd_t           dart_client;        // 0x020A
	
	ext_interact_id_t								ids;								//�뱾�������Ļ�����id
	
	uint16_t                        self_client;        //�����ͻ���
	//????????????????????????????????????
	ext_sentry_t                    sentry_Info_rescue; //�ڱ��ṩ����Ϣ
	
	bool	 		data_valid;	// ������Ч��
	bool			err_cnt;
	uint8_t		offline_cnt;
	uint8_t		offline_max_cnt;	
} judge_info_t;
/* 
	ѧ�������˼�ͨ�� cmd_id 0x0301������ data_ID:0x0200~0x02FF
	�������� �����˼�ͨ�ţ�0x0301��
	����Ƶ�ʣ����������кϼƴ������� 5000 Byte�� �����з���Ƶ�ʷֱ𲻳���30Hz��
 * +------+------+-------------+------------------------------------+
 * | byte | size |    breif    |            note                    |
 * |offset|      |             |                                    |
 * +------+------+-------------+------------------------------------+
 * |  0   |  2   | 	 data_ID   | 0x0200~0x02FF,��������Щ ID ��ѡȡ |
 * |      |      |             | ����ID�����ɲ������Զ���           |
 * +------|------|-------------|------------------------------------|
 * |  2   |  2   | 	sender_ID  | ��ҪУ�鷢���ߵ� ID ��ȷ��					|
 * +------|------|-------------|------------------------------------|
 * |  4   |  2   | receiver_ID | ��ҪУ������ߵ� ID ��ȷ��					|
 * |      |      |             | ���粻�ܷ��͵��жԻ����˵�ID				| 
 * +------|------|-------------|------------------------------------|
 * |  6   |  n   | 		Data     | n ��ҪС�� 113 										|
 * +------+------+-------------+------------------------------------+
*/
/******************************�ͻ��˽�������**************************************/
#define INTERACT_DATA_LEN	113
typedef __packed struct //���ݶ����ݸ�ʽ
{ 
	uint16_t data_cmd_id;    
	uint16_t send_ID;    
	uint16_t receiver_ID; 
} ext_client_data_header_t; 
enum
{
	//0x200-0x02ff 	�����Զ������� ��ʽ  INTERACT_ID_XXXX
	INTERACT_ID_delete_graphic 			= 0x0100,	/*�ͻ���ɾ��ͼ��*/
	INTERACT_ID_draw_one_graphic 		= 0x0101,	/*�ͻ��˻���һ��ͼ��*/
	INTERACT_ID_draw_two_graphic 		= 0x0102,	/*�ͻ��˻���2��ͼ��*/
	INTERACT_ID_draw_five_graphic 	= 0x0103,	/*�ͻ��˻���5��ͼ��*/
	INTERACT_ID_draw_seven_graphic 	= 0x0104,	/*�ͻ��˻���7��ͼ��*/
	INTERACT_ID_draw_char_graphic 	= 0x0110,	/*�ͻ��˻����ַ�ͼ��*/
	INTERACT_ID_radar_sentry_ex			= 0x0200//�״﷢���ڱ��Ƕ���Ϣ
};
typedef __packed struct 
{ 
	uint8_t data[INTERACT_DATA_LEN]; //���ݶ�,n��ҪС��113
} robot_interactive_data_t;
//��λ���ֽڣ�
enum
{
	LEN_INTERACT_delete_graphic     = 8,  //ɾ��ͼ�� 2(��������ID)+2(������ID)+2��������ID��+2���������ݣ�  
	LEN_INTERACT_draw_one_graphic   = 21, // ����2+2+2+15
	LEN_INTERACT_draw_two_graphic   = 36, //6+15*2
	LEN_INTERACT_draw_five_graphic  = 81, //6+15*5
	LEN_INTERACT_draw_seven_graphic = 111,//6+15*7
	LEN_INTERACT_draw_char_graphic  = 51, //6+15+30���ַ������ݣ�
};
//****************************��ͼ�����ݶ�����****************************/


typedef __packed struct
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_ID;
} ext_robot_command_t;


//

typedef __packed struct
{
	uint16_t target_robot_ID;
	float target_position_x;
	float target_position_y;
	float toward_Angle;
} ext_client_map_command_t;

typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_map_command_t clientData;		
	uint16_t	FrameTail;								
}radar_command_t;



typedef __packed struct//ͼ��
{                          
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; //ֱ��  ����  ��Բ  ��Բ  Բ��  ����  ����  �ַ�
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t start_angle:9;  //��    ��    ��    ��    �Ƕ�  ��С  ��С  ��С
	uint32_t end_angle:9;    //��    ��    ��    ��          λ��  ��    ����
	uint32_t width:10;       
	uint32_t start_x:11;     //���  ���  Բ��  Բ��  Բ��  ���  ���  ���
	uint32_t start_y:11;     //
	uint32_t radius:10;      //��    ��    �뾶  ��    ��    ��    ��    ��
	uint32_t end_x:11;       //�յ�  �Զ�  ��    ����  ����  ��    ��    ��
	uint32_t end_y:11;       //                              ��    ��    ��
} graphic_data_struct_t;
typedef __packed struct//������
{                          
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t start_angle:9;  
	uint32_t end_angle:9;    
	uint32_t width:10;       
	uint32_t start_x:11;    
	uint32_t start_y:11;     
  int number;       
} Float_data_struct_t;
typedef __packed struct//������
{                          
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t start_angle:9;  
	uint32_t end_angle:9;    
	uint32_t width:10;       
	uint32_t start_x:11;    
	uint32_t start_y:11;     
  int32_t number;       
} Int_data_struct_t;
/* data_ID: 0X0100  Byte:  2	    �ͻ���ɾ��ͼ��*/
typedef __packed struct
{
	uint8_t operate_type; 
	uint8_t layer;//ͼ������0~9
}ext_client_custom_graphic_delete_t;
typedef enum
{
	NONE_delete    = 0,
	GRAPHIC_delete = 1,
	ALL_delete     = 2
}delete_Graphic_Operate;//ext_client_custom_graphic_delete_t��uint8_t operate_type
/*ͼ��ɾ������*/

//bit 0-2
typedef enum
{
	NONE   = 0,/*�ղ���*/
	ADD    = 1,/*����ͼ��*/
	MODIFY = 2,/*�޸�ͼ��*/
	DELETE = 3,/*ɾ��ͼ��*/
}Graphic_Operate;//graphic_data_struct_t��uint32_t operate_tpye
/*ͼ�����*/
//bit3-5
typedef enum
{
	LINE      = 0,//ֱ��
	RECTANGLE = 1,//����
	CIRCLE    = 2,//��Բ
	OVAL      = 3,//��Բ
	ARC       = 4,//Բ��
	INT     = 5,//������
	FLOAT       = 6,//������FLOAT
	CHAR      = 7 //�ַ�
}Graphic_Type;
/*ͼ������*/
//bit 6-9ͼ���� ���Ϊ9����С0
//bit 10-13��ɫ
typedef enum
{
	RED_BLUE  = 0,//������ɫ	
	YELLOW    = 1,
	GREEN     = 2,
	ORANGE    = 3,
	FUCHSIA   = 4,	/*�Ϻ�ɫ*/
	PINK      = 5,
	CYAN_BLUE = 6,	/*��ɫ*/
	BLACK     = 7,
	WHITE     = 8
}Graphic_Color;
/*ͼ����ɫ����*/
//bit 14-31 �Ƕ� [0,360]
/**********************************�ͻ��˻�ͼ************************************************/
//ɾ��ͼ��
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	ext_client_custom_graphic_delete_t clientData;		
	uint16_t	FrameTail;								
}ext_deleteLayer_data_t;

//���ַ���
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
} ext_client_string_t;

typedef __packed struct
{
	std_frame_header_t txFrameHeader;			//֡ͷ
	uint16_t  CmdID;										//������
	ext_client_data_header_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	ext_client_string_t clientData;//���ݶ�
	uint16_t	FrameTail;								//֡β
}ext_charstring_data_t;
//������ͼ
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			//֡ͷ
	uint16_t  CmdID;										//������
	ext_client_data_header_t   dataFrameHeader;//���ݶ�ͷ�ṹ
	graphic_data_struct_t clientData;		//���ݶ�
	uint16_t	FrameTail;								//֡β
}ext_graphic_one_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	graphic_data_struct_t clientData[2];		
	uint16_t	FrameTail;								
}ext_graphic_two_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	graphic_data_struct_t clientData[5];		
	uint16_t	FrameTail;								
}ext_graphic_five_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	graphic_data_struct_t clientData[7];		
	uint16_t	FrameTail;								
}ext_graphic_seven_data_t;
//���Ƹ�����
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Float_data_struct_t clientData;		
	uint16_t	FrameTail;								
}ext_float_one_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Float_data_struct_t clientData[2];		
	uint16_t	FrameTail;								
}ext_float_two_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Float_data_struct_t clientData[7];		
	uint16_t	FrameTail;								
}ext_float_seven_data_t;
//��������
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Int_data_struct_t clientData;		
	uint16_t	FrameTail;								
}ext_int_one_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Int_data_struct_t clientData[2];		
	uint16_t	FrameTail;								
}ext_int_two_data_t;
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	Int_data_struct_t clientData[7];		
	uint16_t	FrameTail;								
}ext_int_seven_data_t;
//����ͨ��
typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_data_header_t   dataFrameHeader;
	float angle_Array[3];
	char   See_Info_Array[6];
	uint16_t	FrameTail;								
}ext_car_data_t;
typedef struct judge_sensor_struct {
	judge_info_t		*info;
	drv_uart_t			*driver;
	void		(*init)(struct judge_sensor_struct *self);
	void		(*update)(struct judge_sensor_struct *self, uint8_t *rxBuf);
	void		(*check)(struct judge_sensor_struct *self);	
	void		(*heart_beat)(struct judge_sensor_struct *self);
	void		(*sendclient)(struct judge_sensor_struct *self);
	void		(*sendteammate)(struct judge_sensor_struct *self);
	dev_work_state_t	work_state;
	dev_errno_t				errno;
	dev_id_t					id;
} judge_sensor_t;

void Client_graphic_Init(void);
void Client_graphic_delete_update(uint8_t delete_layer);//ɾ��ͼ����Ϣ
void Client_graphic_Info_update(void);


void Client_aim_update(void);//׼��
void Client_supercapacitor_update(void);//��������
void Client_gimbal_angle_update(void);//����Ƕ�
void Client_bullet_int_update(void);//������Ϣ
void  car_radar_sentry_communicate(void);

//Ӣ��
void Client_aim_line(void);//
void _high_aim_(void);
void _lowshort_aim_2(void);
void _lowshort_aim_3(void);
void _lowshortstem_aim_4(void);
void _lowlong_aim_(void);

//����
//void engineer_state(void);
void engineer_mine(void);
void Client_engineer_condition(void);
void Client_engineer_Redgreen(void);
void Client_2engineer_Redgreen(void);
void Client_string_taskstate(void);
//�״�
void radar_runing(void);
void radar_Info_store(void);
#endif
