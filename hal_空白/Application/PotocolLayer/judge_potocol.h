//#ifndef __JUDGE_POTOCOL_H
//#define __JUDGE_POTOCOL_H

///* Includes ------------------------------------------------------------------*/
//#include "rp_config.h"

///* Exported functions --------------------------------------------------------*/

///* Exported macro ------------------------------------------------------------*/
//// �����˼佻�����ݶγ���
//#define INTERACT_DATA_LEN	100
///* Exported types ------------------------------------------------------------*/

//enum
//{ 
//	ID_game_state       						= 0x0001,//����״̬���ݣ�1Hz
//	ID_game_result 	   							= 0x0002,//����������ݣ�������������
//	ID_game_robot_HP       					= 0x0003,//����������Ѫ�����ݣ�1Hz����
//	ID_dart_status									= 0x0004,//���ڷ���״̬�����ڷ���ʱ����
//	ID_ICRA_buff_debuff_zone_status = 0x0005,//�˹�������ս���ӳ���ͷ���״̬��1Hz
//	ID_event_data  									= 0x0101,//�����¼����ݣ�1Hz
//	ID_supply_projectile_action   	= 0x0102,//���ز���վ������ʶ����
//	ID_referee_warning					 		= 0x0104,//���о������ݣ��������
//	ID_dart_remaining_time					= 0x0105,//���ڷ���ڵ���ʱ��1Hz
//	ID_game_robot_state    					= 0x0201,//������״̬���ݣ�10Hz
//	ID_power_heat_data    					= 0x0202,//ʵʱ�����������ݣ�50Hz
//	ID_game_robot_pos        				= 0x0203,//������λ�����ݣ�10Hz
//	ID_buff_musk										= 0x0204,//�������������ݣ�1Hz
//	ID_aerial_robot_energy					= 0x0205,//���л���������״̬���ݣ�10Hz��ֻ�п��л��������ط���
//	ID_robot_hurt										= 0x0206,//�˺�״̬���ݣ��˺���������
//	ID_shoot_data										= 0x0207,//ʵʱ������ݣ��ӵ��������
//	ID_bullet_remaining							= 0x0208,//����ʣ�෢�����������л����ˣ��ڱ��������Լ�ICRA�����˷��ͣ�1Hz
//	ID_rfid_status									= 0x0209,//������RFID״̬��1Hz
//	ID_interactive_header_data			= 0x0301 //�����˽������ݣ����ͷ���������
//};
////������ö��

//enum
//{
//	LEN_game_state       						= 3,			//0x0001,//����״̬���ݣ�1Hz
//	LEN_game_result 	   						= 1,			//0x0002,//����������ݣ�������������
//	LEN_game_robot_HP       				= 32,			//0x0003,//����������Ѫ�����ݣ�1Hz����
//	LEN_dart_status									= 3,			//0x0004,//���ڷ���״̬�����ڷ���ʱ����
//	LEN_ICRA_buff_debuff_zone_status= 3,			//0x0005,//�˹�������ս���ӳ���ͷ���״̬��1Hz
//	LEN_event_data  								= 4,			//0x0101,//�����¼����ݣ�1Hz
//	LEN_supply_projectile_action   	= 4,			//0x0102,//���ز���վ������ʶ����
//	LEN_referee_warning					 		= 2,			//0x0104,//���о������ݣ��������
//	LEN_dart_remaining_time					= 1,			//0x0105,//���ڷ���ڵ���ʱ��1Hz
//	LEN_game_robot_state    				= 18,			//0x0201,//������״̬���ݣ�10Hz
//	LEN_power_heat_data    					= 16,			//0x0202,//ʵʱ�����������ݣ�50Hz
//	LEN_game_robot_pos        			= 16,			//0x0203,//������λ�����ݣ�10Hz
//	LEN_buff_musk										= 1,			//0x0204,//�������������ݣ�1Hz
//	LEN_aerial_robot_energy					= 3,			//0x0205,//���л���������״̬���ݣ�10Hz��ֻ�п��л��������ط���
//	LEN_robot_hurt									= 1,			//0x0206,//�˺�״̬���ݣ��˺���������
//	LEN_shoot_data									= 6,			//0x0207,//ʵʱ������ݣ��ӵ��������
//	LEN_bullet_remaining						= 2,			//0x0208,//����ʣ�෢�����������л����ˣ��ڱ��������Լ�ICRA�����˷��ͣ�1Hz
//	LEN_rfid_status									= 4				//0x0209,//������RFID״̬��1Hz
////	LEN_interactive_header_data			= n			//0x0301 //�����˽������ݣ����ͷ���������
//};
////����ö��



///* �Զ���֡ͷ Byte: 5 */
//typedef __packed struct
//{
//	uint8_t  sof;
//	uint16_t data_length;
//	uint8_t  seq;
//	uint8_t  crc8;
//} std_frame_header_t;


///* ID: 0x0001	Byte: 	3	����״̬���� */
//typedef __packed struct 
//{ 
//	uint8_t game_type : 4;				// ��������
//	uint8_t game_progress : 4;		// �����׶�
//	uint16_t stage_remain_time;		// ��ǰ�׶�ʣ��ʱ��(��λ:s)
//} ext_game_status_t; 


///* ID: 0x0002	Byte:	1	����������� */
//typedef __packed struct 
//{ 
//	uint8_t winner;
//} ext_game_result_t; 


///* ID: 0x0003	Byte:	32	������Ѫ���������� */
//typedef __packed struct 
//{ 
//	uint16_t red_1_robot_HP;	// ��1Ӣ�ۻ�����Ѫ��(δ�ϳ�������Ѫ��Ϊ0)
//	uint16_t red_2_robot_HP;	// ��2���̻�����Ѫ��
//	uint16_t red_3_robot_HP;	// ��3����������Ѫ��
//	uint16_t red_4_robot_HP;	// ��4����������Ѫ��
//	uint16_t red_5_robot_HP;	// ��5����������Ѫ��
//	uint16_t red_7_robot_HP;	// ��7�ڱ�������Ѫ��
//	uint16_t red_outpost_HP;	// �췽ǰ��վѪ��
//	uint16_t red_base_HP;			// �췽����Ѫ��
//	uint16_t blue_1_robot_HP;	// ��1Ӣ�ۻ�����Ѫ��
//	uint16_t blue_2_robot_HP;	// ��2���̻�����Ѫ��
//	uint16_t blue_3_robot_HP;	// ��3����������Ѫ��
//	uint16_t blue_4_robot_HP;	// ��4����������Ѫ��
//	uint16_t blue_5_robot_HP;	// ��5����������Ѫ��
//	uint16_t blue_7_robot_HP;	// ��7�ڱ�������Ѫ��
//	uint16_t blue_outpost_HP;	// ����ǰ��վѪ��
//	uint16_t blue_base_HP;		// ��������Ѫ��	
//} ext_game_robot_HP_t; 


///* ID: 0x0004 	Byte:	3	���ڷ���״̬ */
//typedef __packed struct
//{
//	uint8_t dart_belong;
//	uint16_t stage_remaining_time;
//} ext_dart_status_t;


///* ID: 0x0005 	Byte:	3	�˹�������ս���ӳ���ͷ���״̬ */
//typedef __packed struct
//{
//	uint8_t F1_zone_status:1;
//	uint8_t F1_zone_buff_debuff_status:3;
//	uint8_t F2_zone_status:1;
//	uint8_t F2_zone_buff_debuff_status:3;
//	uint8_t F3_zone_status:1;
//	uint8_t F3_zone_buff_debuff_status:3;
//	uint8_t F4_zone_status:1;
//	uint8_t F4_zone_buff_debuff_status:3;
//	uint8_t F5_zone_status:1;
//	uint8_t F5_zone_buff_debuff_status:3;
//	uint8_t F6_zone_status:1;
//	uint8_t F6_zone_buff_debuff_status:3;
//} ext_ICRA_buff_debuff_zone_status_t;


///* ID: 0x0101  Byte:  4    �����¼����� */
//typedef __packed struct 
//{ 
//	uint32_t event_type;
//} ext_event_data_t; 


///* ID: 0x0102  Byte:  4    ����վ������ʶ���� */
//typedef __packed struct 
//{ 
//	uint8_t supply_projectile_id;
//	uint8_t supply_robot_id;
//	uint8_t supply_projectile_step;
//	uint8_t supply_projectile_num;
//} ext_supply_projectile_action_t; 


/////* ID: 0X0103  Byte:  3    ���󲹸�վ�����ӵ����� */
////typedef __packed struct 
////{ 
////	uint8_t supply_projectile_id;
////	uint8_t supply_robot_id;
////	uint8_t supply_num;  
////} ext_supply_projectile_booking_t; 


///* ID: 0X0104  Byte:  2	   ���о�����Ϣ */
//typedef __packed struct
//{
//	uint8_t level;
//	uint8_t foul_robot_id;
//} ext_referee_warning_t;


///* ID: 0X0105  Byte:  1	   ���ڷ���ڵ���ʱ */
//typedef __packed struct
//{
//	uint8_t dart_remaining_time;
//} ext_dart_remaining_time_t;


///* ID: 0X0201  Byte: 18    ������״̬���� */
//typedef __packed struct 
//{ 
//	uint8_t robot_id;   										// ������ID��������У�鷢��
//	uint8_t robot_level;  									// 1һ����2������3����
//	uint16_t remain_HP;  										// ������ʣ��Ѫ��
//	uint16_t max_HP; 												// ��������Ѫ��
//	uint16_t shooter_heat0_cooling_rate;  	// ������ 17mm ǹ��ÿ����ȴֵ
//	uint16_t shooter_heat0_cooling_limit;   // ������ 17mm ǹ����������
//	uint16_t shooter_heat1_cooling_rate;		// ������ 42mm ǹ��ÿ����ȴֵ
//	uint16_t shooter_heat1_cooling_limit;   // ������ 42mm ǹ����������
//	uint8_t shooter_heat0_speed_limit;			// ������ 17mm ǹ�������ٶ�(m/s)
//	uint8_t shooter_heat1_speed_limit; 			// ������ 42mm ǹ�������ٶ�(m/s)
//	uint8_t max_chassis_power;							// �����������̹���(W)
//	uint8_t mains_power_gimbal_output : 1;  // gimbal�����	
//	uint8_t mains_power_chassis_output : 1; // chassis�����
//	uint8_t mains_power_shooter_output : 1; // shooter�����
//} ext_game_robot_status_t; 


///* ID: 0X0202  Byte: 16    ʵʱ������������ */
//typedef __packed struct 
//{ 
//	uint16_t chassis_volt;   				// ���������ѹ����λ��mV
//	uint16_t chassis_current;				// ���������������λ��mA
//	float chassis_power;   					// ˲ʱ���ʣ���λ��W
//	uint16_t chassis_power_buffer;	// ���̹��ʻ��壬��λ��60J������������(���¸��ݹ���������250J)
//	uint16_t shooter_heat0;					// 17mm ǹ������
//	uint16_t shooter_heat1;  				// 42mm ǹ������
//	uint16_t mobile_shooter_heat2;	// ���� 17mm ǹ������
//} ext_power_heat_data_t; 


///* ID: 0x0203  Byte: 16    ������λ������ */
//typedef __packed struct 
//{   
//	float x;   
//	float y;   
//	float z;   
//	float yaw; 
//} ext_game_robot_pos_t; 


///* ID: 0x0204  Byte:  1    �������������� */
//typedef __packed struct 
//{ 
//	uint8_t power_rune_buff; 
//} ext_buff_t; 


///* ID: 0x0205  Byte:  3    ���л���������״̬���� */
//typedef __packed struct 
//{ 
//	uint16_t energy_point;
//	uint8_t attack_time;
//} ext_aerial_robot_energy_t; 


///* ID: 0x0206  Byte:  1    �˺�״̬���� */
//typedef __packed struct 
//{ 
//	uint8_t armor_id : 4; 	// װ���˺�ʱ����װ��ID
//	uint8_t hurt_type : 4; 	// 0x0װ���˺� 0x1ģ����� 0x2������ 0x3������ 0x4������ 0x5ײ��
//} ext_robot_hurt_t; 


///* ID: 0x0207  Byte:  6    ʵʱ������� */
//typedef __packed struct 
//{ 
//	uint8_t bullet_type; 	// �ӵ�����(1-17mm, 2-42mm)
//	uint8_t bullet_freq;  // �ӵ���Ƶ(Hz)
//	float bullet_speed;		// �ӵ�����(m/s)
//} ext_shoot_data_t; 


///* ID: 0x0208  Byte:  2    �ӵ�ʣ�෢�������� */
//typedef __packed struct
//{
//	uint16_t bullet_remaining_num;
//} ext_bullet_remaining_t;

///* ID: 0x0209  Byte:  4 	������RFID״̬ */
//typedef __packed struct
//{
//	uint32_t rfid_status;
//} ext_rfid_status_t;

//typedef struct{
//	uint16_t teammate_hero;
//	uint16_t teammate_engineer;
//	uint16_t teammate_infantry3;
//	uint16_t teammate_infantry4;
//	uint16_t teammate_infantry5;
//	uint16_t teammate_plane;
//	uint16_t teammate_sentry;
//	
//	uint16_t client_hero;
//	uint16_t client_engineer;
//	uint16_t client_infantry3;
//	uint16_t client_infantry4;
//	uint16_t client_infantry5;
//	uint16_t client_plane;
//} ext_interact_id_t;

//typedef struct judge_info_struct {
//	std_frame_header_t							fream_header;				// ֡ͷ��Ϣ
//	
//	ext_game_status_t 							game_status;				// 0x0001
//	ext_game_result_t 							game_result;				// 0x0002
//	ext_game_robot_HP_t 						game_robot_HP;			// 0x0003
//	ext_dart_status_t								dart_status;				// 0x0004
//	//ext_ICRA_buff_debuff_zone_status_t	
//	
//	ext_event_data_t								event_data;					// 0x0101
//	ext_supply_projectile_action_t	supply_projectile_action;		// 0x0102
//	//ext_supply_projectile_booking_t supply_projectile_booking;// 0x0103
//	ext_referee_warning_t						referee_warning;		// 0x0104
//	ext_dart_remaining_time_t				dart_remaining_time;// 0x0105
//	
//	ext_game_robot_status_t					game_robot_status;	// 0x0201
//	ext_power_heat_data_t						power_heat_data;		// 0x0202
//	ext_game_robot_pos_t						game_robot_pos;			// 0x0203
//	ext_buff_t											buff;								// 0x0204
//	ext_aerial_robot_energy_t				aerial_robot_energy;// 0x0205
//	ext_robot_hurt_t								robot_hurt;					// 0x0206
//	ext_shoot_data_t								shoot_data;					// 0x0207
//	ext_bullet_remaining_t					bullet_remaining;		// 0x0208	
//	ext_rfid_status_t								rfid_status;				// 0x0209	
//	
//	ext_interact_id_t								ids;								//�뱾�������Ļ�����id
//	
//	bool	 		data_valid;	// ������Ч��
//	bool			err_cnt;
//	uint8_t		offline_cnt;
//	uint8_t		offline_max_cnt;	
//} judge_info_t;

///**
// *	-----------------------------------
// *	# �����˼佻������
// *	-----------------------------------
// */
///* 
//	�������ݣ�����һ��ͳһ�����ݶ�ͷ�ṹ��
//	���������� ID���������Լ������ߵ� ID ���������ݶΣ�
//	�����������ݵİ��ܹ������Ϊ 128 ���ֽڣ�
//	��ȥ frame_header,cmd_id,frame_tail ��9���ֽ��Լ����ݶ�ͷ�ṹ�� 6 ���ֽڣ�
//	�ʶ����͵��������ݶ����Ϊ 113��
//	������������ 0x0301 �İ�����Ƶ��Ϊ 10Hz��

//	������ ID��
//	1��Ӣ��(��)��
//	2������(��)��
//	3/4/5������(��)��
//	6������(��)��
//	7���ڱ�(��)��

//	101��Ӣ��(��)��
//	102������(��)��
//	103/104/105������(��)��
//	106������(��)��
//	107���ڱ�(��)��

//	�ͻ��� ID�� 
//	0x0101 ΪӢ�۲����ֿͻ���(��) ��
//	0x0102 Ϊ���̲����ֿͻ���( �� )��
//	0x0103/0x0104/0x0105 Ϊ���������ֿͻ���(��)��
//	0x0106 Ϊ���в����ֿͻ���((��)�� 

//	0x0165��Ӣ�۲����ֿͻ���(��)��
//	0x0166�����̲����ֿͻ���(��)��
//	0x0167/0x0168/0x0169�����������ֿͻ���(��)��
//	0x016A�����в����ֿͻ���(��)�� 
//*/

///* 
//	ѧ�������˼�ͨ�� cmd_id 0x0301������ data_ID:0x0200~0x02FF
//	�������� �����˼�ͨ�ţ�0x0301��
//	����Ƶ�ʣ����������кϼƴ������� 5000 Byte�� �����з���Ƶ�ʷֱ𲻳���30Hz��
// * +------+------+-------------+------------------------------------+
// * | byte | size |    breif    |            note                    |
// * |offset|      |             |                                    |
// * +------+------+-------------+------------------------------------+
// * |  0   |  2   | 	 data_ID   | 0x0200~0x02FF,��������Щ ID ��ѡȡ |
// * |      |      |             | ����ID�����ɲ������Զ���           |
// * +------|------|-------------|------------------------------------|
// * |  2   |  2   | 	sender_ID  | ��ҪУ�鷢���ߵ� ID ��ȷ��					|
// * +------|------|-------------|------------------------------------|
// * |  4   |  2   | receiver_ID | ��ҪУ������ߵ� ID ��ȷ��					|
// * |      |      |             | ���粻�ܷ��͵��жԻ����˵�ID				| 
// * +------|------|-------------|------------------------------------|
// * |  6   |  n   | 		Data     | n ��ҪС�� 113 										|
// * +------+------+-------------+------------------------------------+
//*/

//typedef __packed struct 
//{ 
//	uint8_t data[INTERACT_DATA_LEN]; //���ݶ�,n��ҪС��113
//} robot_interactive_data_t;

///****************ͼ��UI��������*******************//////////////////////////////
////��5-2�ͻ���ɾ��ͼ�� 0x0100
//typedef __packed struct
//{
//	uint8_t operate_type; 
//	/*
//		0���ղ���
//		1��ɾ��ͼ��
//		2��ɾ������
//	*/
//	uint8_t layer;
//	/*
//		ͼ����  0-9
//	*/
//}ext_client_custom_graphic_delete_t;
///*ͼ��ɾ������*/

//enum
//{
//	LEN_INTERACT_delete_graphic = 8,			/*ɾ��ͼ��*/
//	LEN_INTERACT_draw_one_graphic = 21,
//	LEN_INTERACT_draw_two_graphic = 36,
//	LEN_INTERACT_draw_five_graphic = 81,
//	LEN_INTERACT_draw_seven_graphic = 111,
//	LEN_INTERACT_draw_char_graphic = 51
//};
///*ͼ����Ƴ���*/
///////////////////////////////////////////////////////�������ñ�5-3
////bit 0-2
//typedef enum
//{
//	NONE = 0,  /*�ղ���*/
//	ADD = 1,	/*����ͼ��*/
//	MODIFY = 2,/*�޸�ͼ��*/
//	DELETE = 3,	/*ɾ��ͼ��*/
//}Graphic_Operate;
///*ͼ�����*/

////bit3-5
//typedef enum
//{
//	LINE = 0,//ֱ��
//	RECTANGLE = 1,//����
//	CIRCLE = 2,//��Բ
//	OVAL = 3, 		/*��Բ*/
//	ARC = 4, 			/*Բ��*/
//	FLOAT = 5, //������
//	INT = 6,//������
//	CHAR = 7//�ַ�
//}Graphic_Type;
///*ͼ������*/

////bit 6-9ͼ���� ���Ϊ9����С0

////bit 10-13��ɫ
//typedef enum
//{
//	RED_BLUE = 0,//������ɫ	
//	YELLOW = 1,
//	GREEN = 2,
//	ORANGE = 3,
//	FUCHSIA = 4,	/*�Ϻ�ɫ*/
//	PINK = 5,
//	CYAN_BLUE = 6,	/*��ɫ*/
//	BLACK = 7,
//	WHITE = 8
//}Graphic_Color;
///*ͼ����ɫ����*/

////bit 14-31 �Ƕ�
/////////////////////////////////////////////////////////////////////////////////
////��5-3

//typedef __packed struct//��ʮ����ֽ�
//{ 
//	uint8_t graphic_name[3]; 
//	/*ͼ��������Ϊ�ͻ��˵�����*/
//	uint32_t operate_tpye:3; 
//	/*
//		0���ղ���
//		1������
//		2���޸�
//		3��ɾ��
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//		0ֱ��
//		1����
//		2��Բ
//		3��Բ
//		4Բ��
//		5������
//		6������
//		7�ַ�
//	*/
//	uint32_t layer:4; 
//	/*ͼ���� 0-9*/
//	uint32_t color:4; 
//	/*0���� 1��ɫ 2��ɫ 3��ɫ 4�Ϻ�ɫ 5��ɫ 6��ɫ 7��ɫ 8��ɫ*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//�߿�
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	uint32_t radius:10; //�����С���߰뾶
//	uint32_t end_x:11; 
//	uint32_t end_y:11; 
//	/*�������ͼ�����͵Ķ��岻ͬ
//	���� 	start_angle 	end_angle 		width 	start_x 	start_y 	radius 	end_x 	end_y
//	ֱ��  	��     					�� 		 		 �߿�  		���x  	 ���y     ��   		�յ�x  	�յ�y
//	����		��							��				 �߿�			���x  	 ���y     ��   		�Խ�x  	�Խ�y
//	��Բ		��							��				 �߿�			Բ��x  	 Բ��y     �뾶   	��  		��
//	��Բ		��							��				 �߿�			Բ��x  	 Բ��y     ��   		x����  	y����
//	Բ��		��ʼ�Ƕ�		��ֹ�Ƕ�			 �߿�			Բ��x  	 Բ��y     ��   		x���� 	y����
//	����		�����С		��Чλ��			 �߿�			���x  	 ���y     32bit������--float
//	����		�����С				��				 �߿�			���x  	 ���y     32bit������--int32_t
//	�ַ�		�����С		�ַ�����			 �߿�			���x  	 ���y     ��   		��  		��
//	*/
//}graphic_data_struct_t;
///*����ͼ��*/

//typedef __packed struct//�Լ�д�Ĺ�ʮ����ֽ�//////////////////////////////////////////////
//{ 
//	uint8_t graphic_name[3]; 
//	/*ͼ��������Ϊ�ͻ��˵�����*/
//	uint32_t operate_tpye:3; 
//	/*
//		0���ղ���
//		1������
//		2���޸�
//		3��ɾ��
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//		6������
//	*/
//	uint32_t layer:4; 
//	/*ͼ���� 0-9*/
//	uint32_t color:4; 
//	/*0���� 1��ɫ 2��ɫ 3��ɫ 4�Ϻ�ɫ 5��ɫ 6��ɫ 7��ɫ 8��ɫ*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//�߿�
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	int     number; 
//}graphic_Intdata_struct_t;
//typedef __packed struct//�Լ�д�Ĺ�ʮ����ֽ�//////////////////////////////////////////////
//{ 
//	uint8_t graphic_name[3]; 
//	/*ͼ��������Ϊ�ͻ��˵�����*/
//	uint32_t operate_tpye:3; 
//	/*
//		0���ղ���
//		1������
//		2���޸�
//		3��ɾ��
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//	  5������
//	*/
//	uint32_t layer:4; 
//	/*ͼ���� 0-9*/
//	uint32_t color:4; 
//	/*0���� 1��ɫ 2��ɫ 3��ɫ 4�Ϻ�ɫ 5��ɫ 6��ɫ 7��ɫ 8��ɫ*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//�߿�
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	float    number; 
//}graphic_Floatdata_struct_t;
//enum
//{
//	//0x200-0x02ff 	�����Զ������� ��ʽ  INTERACT_ID_XXXX
//	INTERACT_ID_delete_graphic 			= 0x0100,	/*�ͻ���ɾ��ͼ��*/
//	INTERACT_ID_draw_one_graphic 		= 0x0101,	/*�ͻ��˻���һ��ͼ��*/
//	INTERACT_ID_draw_two_graphic 		= 0x0102,	/*�ͻ��˻���2��ͼ��*/
//	INTERACT_ID_draw_five_graphic 	= 0x0103,	/*�ͻ��˻���5��ͼ��*/
//	INTERACT_ID_draw_seven_graphic 	= 0x0104,	/*�ͻ��˻���7��ͼ��*/
//	INTERACT_ID_draw_char_graphic 	= 0x0110,	/*�ͻ��˻����ַ�ͼ��*/
//	INTERACT_ID_bigbome_num					= 0x02ff
//};





//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct;
//}ext_client_custom_graphic_single_t;
///*�ͻ��˻���һ��ͼ��*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[2];
//}ext_client_custom_graphic_double_t;
///*�ͻ��˻�������ͼ��*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[5];
//}ext_client_custom_graphic_five_t;
///*�ͻ��˻������ͼ��*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[7];
//}ext_client_custom_graphic_seven_t;
///*�ͻ��˻����߸�ͼ��*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct;
//	uint8_t data[30];
//}ext_client_custom_character_t;
///*�ͻ��˻����ַ�*/

///* �Զ���֡ͷ */
//typedef __packed struct
//{
//	uint8_t  SOF; 	
//	uint16_t DataLength;
//	uint8_t  Seq; 	
//	uint8_t  CRC8; 
//}ext_FrameHeader;
///* �������ݽ�����Ϣ��0x0301  */
//typedef __packed struct 
//{ 
//	uint16_t data_cmd_id;    
//	uint16_t send_ID;    
//	uint16_t receiver_ID; 
//} ext_student_interactive_header_data_t; 

///*�����˼佻������*/
//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//֡ͷ
//	uint16_t  CmdID;										//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	graphic_data_struct_t clientData;		//���ݶ�
//	uint16_t	FrameTail;								//֡β
//}ext_student_interactive_data_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//֡ͷ
//	uint16_t  CmdID;										//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	graphic_Intdata_struct_t clientData[2];//���ݶ�
//	uint16_t	FrameTail;								//֡β
//}ext_student_interactive_data1_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//֡ͷ
//	uint16_t  CmdID;										//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	graphic_Floatdata_struct_t clientData[2];//���ݶ�
//	uint16_t	FrameTail;								//֡β
//}ext_student_floateractive_data2_t;
///////////////////////////////////////////////////////////////////////
//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//֡ͷ
//	uint16_t  CmdID;										//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	graphic_data_struct_t clientData[7];//���ݶ�]
//	uint16_t	FrameTail;								//֡β
//}ext_student_interactive_data7_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//֡ͷ
//	uint16_t  CmdID;										//������
//	ext_student_interactive_header_data_t   dataFrameHeader;//���ݶ�ͷ�ṹ
//	ext_client_custom_character_t clientData;//���ݶ�]
//	uint16_t	FrameTail;								//֡β
//}ext_student_interactive_data_char_t;

//typedef struct judge_sensor_struct {
//	judge_info_t		*info;
//	drv_uart_t			*driver;
//	void		(*init)(struct judge_sensor_struct *self);
//	void		(*update)(struct judge_sensor_struct *self, uint8_t *rxBuf);
//	void		(*check)(struct judge_sensor_struct *self);	
//	void		(*heart_beat)(struct judge_sensor_struct *self);
//	void		(*send2client)(struct judge_sensor_struct *self);
//	void		(*send2teammate)(struct judge_sensor_struct *self);
//	dev_work_state_t	work_state;
//	dev_errno_t				errno;
//	dev_id_t					id;
//} judge_sensor_t;

//void Send_Char_To_Client0(void);
//void Send_Data_To_Client1(void);
//void Send_Data_To_Client2(void);
//void Send_Data_To_Teammate(void);
//void Send_Data_To_Client3(void);
///*��������������������������������������������������ͼ�λ�����API��������������������������������������������������*/
//void Delete_All_Graphic_Layer(uint16_t target);
//void Delete_Graphic_Layer(uint16_t target,uint8_t layer);
//void Add_Graphic(graphic_data_struct_t* graphic,
//									const uint8_t* name,
//									uint32_t layer,
//									uint32_t type,
//									uint32_t color,
//									uint32_t start_angle,
//									uint32_t end_angle,
//									uint32_t width,
//									uint32_t start_x,
//									uint32_t start_y,
//									uint32_t radius,
//									uint32_t end_x,
//									uint32_t end_y);

//void Modify_Graphic(graphic_data_struct_t* graphic,
//								  const uint8_t* name,
//									uint8_t layer,
//									uint8_t type,
//									uint8_t color,
//									uint16_t start_angle,
//									uint16_t end_angle,
//									uint16_t width,
//									uint16_t start_x,
//									uint16_t start_y,
//									uint16_t radius,
//									uint16_t end_x,
//									uint16_t end_y);									
//									
//void Delete_Graphic(graphic_data_struct_t* graphic);			
//void Draw_Rectangle(graphic_data_struct_t* graphic,
//										const uint8_t *name,
//										uint8_t layer,
//										Graphic_Color color,
//										uint16_t width,
//										uint16_t start_x,
//										uint16_t start_y,
//										uint16_t diagonal_x,
//										uint16_t diagonal_y);

//void Draw_Line(graphic_data_struct_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t width,
//							uint16_t start_x,
//							uint16_t start_y,
//							uint16_t end_x,
//							uint16_t end_y);
//							
//void Draw_Circle(graphic_data_struct_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t width,
//							uint16_t center_x,
//							uint16_t center_y,
//							uint16_t radius);
//							
//void Draw_Oval(graphic_data_struct_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t width,
//							uint16_t center_x,
//							uint16_t center_y,
//							uint16_t axis_x,
//							uint16_t axis_y);

//void Draw_ARC(graphic_data_struct_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t start_angle,
//							uint16_t end_angle,
//							uint16_t width,
//							uint16_t center_x,
//							uint16_t center_y,
//							uint16_t axis_x,
//							uint16_t axis_y);							

//void Draw_Float(graphic_Floatdata_struct_t* graphic,
//								const uint8_t *name,
//								uint8_t layer,
//								Graphic_Color color,
//								uint16_t font_size,
//								uint16_t accuracy,
//								uint16_t width,
//								uint32_t start_x,
//								uint32_t start_y,
//								float number);
//								/////////////////////////////////////
//void Draw_Int(graphic_Intdata_struct_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t font_size,
//							uint16_t width,
//							uint16_t start_x,
//							uint16_t start_y,
//							int32_t number);
//							
//void Draw_Char(ext_client_custom_character_t* graphic,
//							const uint8_t *name,
//							uint8_t layer,
//							Graphic_Color color,
//							uint16_t font_size,
//							uint16_t length,
//							uint16_t width,
//							uint16_t start_x,
//							uint16_t start_y,
//							const uint8_t *character);							

//#endif
