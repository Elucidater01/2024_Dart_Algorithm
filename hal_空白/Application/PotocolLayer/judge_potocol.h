//#ifndef __JUDGE_POTOCOL_H
//#define __JUDGE_POTOCOL_H

///* Includes ------------------------------------------------------------------*/
//#include "rp_config.h"

///* Exported functions --------------------------------------------------------*/

///* Exported macro ------------------------------------------------------------*/
//// 机器人间交互数据段长度
//#define INTERACT_DATA_LEN	100
///* Exported types ------------------------------------------------------------*/

//enum
//{ 
//	ID_game_state       						= 0x0001,//比赛状态数据，1Hz
//	ID_game_result 	   							= 0x0002,//比赛结果数据，比赛结束发送
//	ID_game_robot_HP       					= 0x0003,//比赛机器人血量数据，1Hz发送
//	ID_dart_status									= 0x0004,//飞镖发射状态，飞镖发射时发送
//	ID_ICRA_buff_debuff_zone_status = 0x0005,//人工智能挑战赛加成与惩罚区状态，1Hz
//	ID_event_data  									= 0x0101,//场地事件数据，1Hz
//	ID_supply_projectile_action   	= 0x0102,//场地补给站动作标识数据
//	ID_referee_warning					 		= 0x0104,//裁判警告数据，警告后发送
//	ID_dart_remaining_time					= 0x0105,//飞镖发射口倒计时，1Hz
//	ID_game_robot_state    					= 0x0201,//机器人状态数据，10Hz
//	ID_power_heat_data    					= 0x0202,//实时功率热量数据，50Hz
//	ID_game_robot_pos        				= 0x0203,//机器人位置数据，10Hz
//	ID_buff_musk										= 0x0204,//机器人增益数据，1Hz
//	ID_aerial_robot_energy					= 0x0205,//空中机器人能量状态数据，10Hz，只有空中机器人主控发送
//	ID_robot_hurt										= 0x0206,//伤害状态数据，伤害发生后发送
//	ID_shoot_data										= 0x0207,//实时射击数据，子弹发射后发送
//	ID_bullet_remaining							= 0x0208,//弹丸剩余发送数，仅空中机器人，哨兵机器人以及ICRA机器人发送，1Hz
//	ID_rfid_status									= 0x0209,//机器人RFID状态，1Hz
//	ID_interactive_header_data			= 0x0301 //机器人交互数据，发送方触发发送
//};
////命令码枚举

//enum
//{
//	LEN_game_state       						= 3,			//0x0001,//比赛状态数据，1Hz
//	LEN_game_result 	   						= 1,			//0x0002,//比赛结果数据，比赛结束发送
//	LEN_game_robot_HP       				= 32,			//0x0003,//比赛机器人血量数据，1Hz发送
//	LEN_dart_status									= 3,			//0x0004,//飞镖发射状态，飞镖发射时发送
//	LEN_ICRA_buff_debuff_zone_status= 3,			//0x0005,//人工智能挑战赛加成与惩罚区状态，1Hz
//	LEN_event_data  								= 4,			//0x0101,//场地事件数据，1Hz
//	LEN_supply_projectile_action   	= 4,			//0x0102,//场地补给站动作标识数据
//	LEN_referee_warning					 		= 2,			//0x0104,//裁判警告数据，警告后发送
//	LEN_dart_remaining_time					= 1,			//0x0105,//飞镖发射口倒计时，1Hz
//	LEN_game_robot_state    				= 18,			//0x0201,//机器人状态数据，10Hz
//	LEN_power_heat_data    					= 16,			//0x0202,//实时功率热量数据，50Hz
//	LEN_game_robot_pos        			= 16,			//0x0203,//机器人位置数据，10Hz
//	LEN_buff_musk										= 1,			//0x0204,//机器人增益数据，1Hz
//	LEN_aerial_robot_energy					= 3,			//0x0205,//空中机器人能量状态数据，10Hz，只有空中机器人主控发送
//	LEN_robot_hurt									= 1,			//0x0206,//伤害状态数据，伤害发生后发送
//	LEN_shoot_data									= 6,			//0x0207,//实时射击数据，子弹发射后发送
//	LEN_bullet_remaining						= 2,			//0x0208,//弹丸剩余发送数，仅空中机器人，哨兵机器人以及ICRA机器人发送，1Hz
//	LEN_rfid_status									= 4				//0x0209,//机器人RFID状态，1Hz
////	LEN_interactive_header_data			= n			//0x0301 //机器人交互数据，发送方触发发送
//};
////长度枚举



///* 自定义帧头 Byte: 5 */
//typedef __packed struct
//{
//	uint8_t  sof;
//	uint16_t data_length;
//	uint8_t  seq;
//	uint8_t  crc8;
//} std_frame_header_t;


///* ID: 0x0001	Byte: 	3	比赛状态数据 */
//typedef __packed struct 
//{ 
//	uint8_t game_type : 4;				// 比赛类型
//	uint8_t game_progress : 4;		// 比赛阶段
//	uint16_t stage_remain_time;		// 当前阶段剩余时间(单位:s)
//} ext_game_status_t; 


///* ID: 0x0002	Byte:	1	比赛结果数据 */
//typedef __packed struct 
//{ 
//	uint8_t winner;
//} ext_game_result_t; 


///* ID: 0x0003	Byte:	32	机器人血量数据数据 */
//typedef __packed struct 
//{ 
//	uint16_t red_1_robot_HP;	// 红1英雄机器人血量(未上场及罚下血量为0)
//	uint16_t red_2_robot_HP;	// 红2工程机器人血量
//	uint16_t red_3_robot_HP;	// 红3步兵机器人血量
//	uint16_t red_4_robot_HP;	// 红4步兵机器人血量
//	uint16_t red_5_robot_HP;	// 红5步兵机器人血量
//	uint16_t red_7_robot_HP;	// 红7哨兵机器人血量
//	uint16_t red_outpost_HP;	// 红方前哨站血量
//	uint16_t red_base_HP;			// 红方基地血量
//	uint16_t blue_1_robot_HP;	// 蓝1英雄机器人血量
//	uint16_t blue_2_robot_HP;	// 蓝2工程机器人血量
//	uint16_t blue_3_robot_HP;	// 蓝3步兵机器人血量
//	uint16_t blue_4_robot_HP;	// 蓝4步兵机器人血量
//	uint16_t blue_5_robot_HP;	// 蓝5步兵机器人血量
//	uint16_t blue_7_robot_HP;	// 蓝7哨兵机器人血量
//	uint16_t blue_outpost_HP;	// 蓝方前哨站血量
//	uint16_t blue_base_HP;		// 蓝方基地血量	
//} ext_game_robot_HP_t; 


///* ID: 0x0004 	Byte:	3	飞镖发射状态 */
//typedef __packed struct
//{
//	uint8_t dart_belong;
//	uint16_t stage_remaining_time;
//} ext_dart_status_t;


///* ID: 0x0005 	Byte:	3	人工智能挑战赛加成与惩罚区状态 */
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


///* ID: 0x0101  Byte:  4    场地事件数据 */
//typedef __packed struct 
//{ 
//	uint32_t event_type;
//} ext_event_data_t; 


///* ID: 0x0102  Byte:  4    补给站动作标识数据 */
//typedef __packed struct 
//{ 
//	uint8_t supply_projectile_id;
//	uint8_t supply_robot_id;
//	uint8_t supply_projectile_step;
//	uint8_t supply_projectile_num;
//} ext_supply_projectile_action_t; 


/////* ID: 0X0103  Byte:  3    请求补给站补弹子弹数据 */
////typedef __packed struct 
////{ 
////	uint8_t supply_projectile_id;
////	uint8_t supply_robot_id;
////	uint8_t supply_num;  
////} ext_supply_projectile_booking_t; 


///* ID: 0X0104  Byte:  2	   裁判警告信息 */
//typedef __packed struct
//{
//	uint8_t level;
//	uint8_t foul_robot_id;
//} ext_referee_warning_t;


///* ID: 0X0105  Byte:  1	   飞镖发射口倒计时 */
//typedef __packed struct
//{
//	uint8_t dart_remaining_time;
//} ext_dart_remaining_time_t;


///* ID: 0X0201  Byte: 18    机器人状态数据 */
//typedef __packed struct 
//{ 
//	uint8_t robot_id;   										// 机器人ID，可用来校验发送
//	uint8_t robot_level;  									// 1一级，2二级，3三级
//	uint16_t remain_HP;  										// 机器人剩余血量
//	uint16_t max_HP; 												// 机器人满血量
//	uint16_t shooter_heat0_cooling_rate;  	// 机器人 17mm 枪口每秒冷却值
//	uint16_t shooter_heat0_cooling_limit;   // 机器人 17mm 枪口热量上限
//	uint16_t shooter_heat1_cooling_rate;		// 机器人 42mm 枪口每秒冷却值
//	uint16_t shooter_heat1_cooling_limit;   // 机器人 42mm 枪口热量上限
//	uint8_t shooter_heat0_speed_limit;			// 机器人 17mm 枪口上限速度(m/s)
//	uint8_t shooter_heat1_speed_limit; 			// 机器人 42mm 枪口上限速度(m/s)
//	uint8_t max_chassis_power;							// 机器人最大底盘功率(W)
//	uint8_t mains_power_gimbal_output : 1;  // gimbal口输出	
//	uint8_t mains_power_chassis_output : 1; // chassis口输出
//	uint8_t mains_power_shooter_output : 1; // shooter口输出
//} ext_game_robot_status_t; 


///* ID: 0X0202  Byte: 16    实时功率热量数据 */
//typedef __packed struct 
//{ 
//	uint16_t chassis_volt;   				// 底盘输出电压，单位：mV
//	uint16_t chassis_current;				// 底盘输出电流，单位：mA
//	float chassis_power;   					// 瞬时功率，单位：W
//	uint16_t chassis_power_buffer;	// 底盘功率缓冲，单位：60J焦耳缓冲能量(飞坡根据规则增加至250J)
//	uint16_t shooter_heat0;					// 17mm 枪口热量
//	uint16_t shooter_heat1;  				// 42mm 枪口热量
//	uint16_t mobile_shooter_heat2;	// 机动 17mm 枪口热量
//} ext_power_heat_data_t; 


///* ID: 0x0203  Byte: 16    机器人位置数据 */
//typedef __packed struct 
//{   
//	float x;   
//	float y;   
//	float z;   
//	float yaw; 
//} ext_game_robot_pos_t; 


///* ID: 0x0204  Byte:  1    机器人增益数据 */
//typedef __packed struct 
//{ 
//	uint8_t power_rune_buff; 
//} ext_buff_t; 


///* ID: 0x0205  Byte:  3    空中机器人能量状态数据 */
//typedef __packed struct 
//{ 
//	uint16_t energy_point;
//	uint8_t attack_time;
//} ext_aerial_robot_energy_t; 


///* ID: 0x0206  Byte:  1    伤害状态数据 */
//typedef __packed struct 
//{ 
//	uint8_t armor_id : 4; 	// 装甲伤害时代表装甲ID
//	uint8_t hurt_type : 4; 	// 0x0装甲伤害 0x1模块掉线 0x2超射速 0x3超热量 0x4超功率 0x5撞击
//} ext_robot_hurt_t; 


///* ID: 0x0207  Byte:  6    实时射击数据 */
//typedef __packed struct 
//{ 
//	uint8_t bullet_type; 	// 子弹类型(1-17mm, 2-42mm)
//	uint8_t bullet_freq;  // 子弹射频(Hz)
//	float bullet_speed;		// 子弹射速(m/s)
//} ext_shoot_data_t; 


///* ID: 0x0208  Byte:  2    子弹剩余发射数数据 */
//typedef __packed struct
//{
//	uint16_t bullet_remaining_num;
//} ext_bullet_remaining_t;

///* ID: 0x0209  Byte:  4 	机器人RFID状态 */
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
//	std_frame_header_t							fream_header;				// 帧头信息
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
//	ext_interact_id_t								ids;								//与本机交互的机器人id
//	
//	bool	 		data_valid;	// 数据有效性
//	bool			err_cnt;
//	uint8_t		offline_cnt;
//	uint8_t		offline_max_cnt;	
//} judge_info_t;

///**
// *	-----------------------------------
// *	# 机器人间交互数据
// *	-----------------------------------
// */
///* 
//	交互数据，包括一个统一的数据段头结构，
//	包含了内容 ID，发送者以及接收者的 ID 和内容数据段，
//	整个交互数据的包总共长最大为 128 个字节，
//	减去 frame_header,cmd_id,frame_tail 共9个字节以及数据段头结构的 6 个字节，
//	故而发送的内容数据段最大为 113。
//	整个交互数据 0x0301 的包上行频率为 10Hz。

//	机器人 ID：
//	1，英雄(红)；
//	2，工程(红)；
//	3/4/5，步兵(红)；
//	6，空中(红)；
//	7，哨兵(红)；

//	101，英雄(蓝)；
//	102，工程(蓝)；
//	103/104/105，步兵(蓝)；
//	106，空中(蓝)；
//	107，哨兵(蓝)；

//	客户端 ID： 
//	0x0101 为英雄操作手客户端(红) ；
//	0x0102 为工程操作手客户端( 红 )；
//	0x0103/0x0104/0x0105 为步兵操作手客户端(红)；
//	0x0106 为空中操作手客户端((红)； 

//	0x0165，英雄操作手客户端(蓝)；
//	0x0166，工程操作手客户端(蓝)；
//	0x0167/0x0168/0x0169，步兵操作手客户端(蓝)；
//	0x016A，空中操作手客户端(蓝)。 
//*/

///* 
//	学生机器人间通信 cmd_id 0x0301，内容 data_ID:0x0200~0x02FF
//	交互数据 机器人间通信：0x0301。
//	发送频率：数据上下行合计带宽不超过 5000 Byte。 上下行发送频率分别不超过30Hz。
// * +------+------+-------------+------------------------------------+
// * | byte | size |    breif    |            note                    |
// * |offset|      |             |                                    |
// * +------+------+-------------+------------------------------------+
// * |  0   |  2   | 	 data_ID   | 0x0200~0x02FF,可以在这些 ID 段选取 |
// * |      |      |             | 具体ID含义由参赛队自定义           |
// * +------|------|-------------|------------------------------------|
// * |  2   |  2   | 	sender_ID  | 需要校验发送者的 ID 正确性					|
// * +------|------|-------------|------------------------------------|
// * |  4   |  2   | receiver_ID | 需要校验接收者的 ID 正确性					|
// * |      |      |             | 例如不能发送到敌对机器人的ID				| 
// * +------|------|-------------|------------------------------------|
// * |  6   |  n   | 		Data     | n 需要小于 113 										|
// * +------+------+-------------+------------------------------------+
//*/

//typedef __packed struct 
//{ 
//	uint8_t data[INTERACT_DATA_LEN]; //数据段,n需要小于113
//} robot_interactive_data_t;

///****************图形UI界面配置*******************//////////////////////////////
////表5-2客户端删除图形 0x0100
//typedef __packed struct
//{
//	uint8_t operate_type; 
//	/*
//		0：空操作
//		1：删除图层
//		2：删除所有
//	*/
//	uint8_t layer;
//	/*
//		图层数  0-9
//	*/
//}ext_client_custom_graphic_delete_t;
///*图层删除操作*/

//enum
//{
//	LEN_INTERACT_delete_graphic = 8,			/*删除图层*/
//	LEN_INTERACT_draw_one_graphic = 21,
//	LEN_INTERACT_draw_two_graphic = 36,
//	LEN_INTERACT_draw_five_graphic = 81,
//	LEN_INTERACT_draw_seven_graphic = 111,
//	LEN_INTERACT_draw_char_graphic = 51
//};
///*图层绘制长度*/
///////////////////////////////////////////////////////以下无用表5-3
////bit 0-2
//typedef enum
//{
//	NONE = 0,  /*空操作*/
//	ADD = 1,	/*增加图层*/
//	MODIFY = 2,/*修改图层*/
//	DELETE = 3,	/*删除图层*/
//}Graphic_Operate;
///*图层操作*/

////bit3-5
//typedef enum
//{
//	LINE = 0,//直线
//	RECTANGLE = 1,//矩形
//	CIRCLE = 2,//正圆
//	OVAL = 3, 		/*椭圆*/
//	ARC = 4, 			/*圆弧*/
//	FLOAT = 5, //浮点数
//	INT = 6,//整型数
//	CHAR = 7//字符
//}Graphic_Type;
///*图层类型*/

////bit 6-9图层数 最大为9，最小0

////bit 10-13颜色
//typedef enum
//{
//	RED_BLUE = 0,//红蓝主色	
//	YELLOW = 1,
//	GREEN = 2,
//	ORANGE = 3,
//	FUCHSIA = 4,	/*紫红色*/
//	PINK = 5,
//	CYAN_BLUE = 6,	/*青色*/
//	BLACK = 7,
//	WHITE = 8
//}Graphic_Color;
///*图层颜色类型*/

////bit 14-31 角度
/////////////////////////////////////////////////////////////////////////////////
////表5-3

//typedef __packed struct//共十五个字节
//{ 
//	uint8_t graphic_name[3]; 
//	/*图形名，作为客户端的索引*/
//	uint32_t operate_tpye:3; 
//	/*
//		0：空操作
//		1：增加
//		2：修改
//		3：删除
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//		0直线
//		1矩形
//		2整圆
//		3椭圆
//		4圆弧
//		5浮点数
//		6整形数
//		7字符
//	*/
//	uint32_t layer:4; 
//	/*图层数 0-9*/
//	uint32_t color:4; 
//	/*0红蓝 1黄色 2绿色 3橙色 4紫红色 5粉色 6青色 7黑色 8白色*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//线宽
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	uint32_t radius:10; //字体大小或者半径
//	uint32_t end_x:11; 
//	uint32_t end_y:11; 
//	/*后面根据图形类型的定义不同
//	类型 	start_angle 	end_angle 		width 	start_x 	start_y 	radius 	end_x 	end_y
//	直线  	空     					空 		 		 线宽  		起点x  	 起点y     空   		终点x  	终点y
//	矩形		空							空				 线宽			起点x  	 起点y     空   		对角x  	对角y
//	正圆		空							空				 线宽			圆心x  	 圆心y     半径   	空  		空
//	椭圆		空							空				 线宽			圆心x  	 圆心y     空   		x半轴  	y半轴
//	圆弧		起始角度		终止角度			 线宽			圆心x  	 圆心y     空   		x半轴 	y半轴
//	浮点		字体大小		有效位数			 线宽			起点x  	 起点y     32bit浮点数--float
//	整形		字体大小				空				 线宽			起点x  	 起点y     32bit整形数--int32_t
//	字符		字体大小		字符长度			 线宽			起点x  	 起点y     空   		空  		空
//	*/
//}graphic_data_struct_t;
///*绘制图形*/

//typedef __packed struct//自己写的共十五个字节//////////////////////////////////////////////
//{ 
//	uint8_t graphic_name[3]; 
//	/*图形名，作为客户端的索引*/
//	uint32_t operate_tpye:3; 
//	/*
//		0：空操作
//		1：增加
//		2：修改
//		3：删除
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//		6整形数
//	*/
//	uint32_t layer:4; 
//	/*图层数 0-9*/
//	uint32_t color:4; 
//	/*0红蓝 1黄色 2绿色 3橙色 4紫红色 5粉色 6青色 7黑色 8白色*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//线宽
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	int     number; 
//}graphic_Intdata_struct_t;
//typedef __packed struct//自己写的共十五个字节//////////////////////////////////////////////
//{ 
//	uint8_t graphic_name[3]; 
//	/*图形名，作为客户端的索引*/
//	uint32_t operate_tpye:3; 
//	/*
//		0：空操作
//		1：增加
//		2：修改
//		3：删除
//	*/
//	uint32_t graphic_tpye:3;
//	/*
//	  5浮点数
//	*/
//	uint32_t layer:4; 
//	/*图层数 0-9*/
//	uint32_t color:4; 
//	/*0红蓝 1黄色 2绿色 3橙色 4紫红色 5粉色 6青色 7黑色 8白色*/
//	uint32_t start_angle:9;
//	uint32_t end_angle:9;
//	////////////////////
//	uint32_t width:10;//线宽
//	uint32_t start_x:11; 
//	uint32_t start_y:11; 
//	////////////////////
//	float    number; 
//}graphic_Floatdata_struct_t;
//enum
//{
//	//0x200-0x02ff 	队伍自定义命令 格式  INTERACT_ID_XXXX
//	INTERACT_ID_delete_graphic 			= 0x0100,	/*客户端删除图形*/
//	INTERACT_ID_draw_one_graphic 		= 0x0101,	/*客户端绘制一个图形*/
//	INTERACT_ID_draw_two_graphic 		= 0x0102,	/*客户端绘制2个图形*/
//	INTERACT_ID_draw_five_graphic 	= 0x0103,	/*客户端绘制5个图形*/
//	INTERACT_ID_draw_seven_graphic 	= 0x0104,	/*客户端绘制7个图形*/
//	INTERACT_ID_draw_char_graphic 	= 0x0110,	/*客户端绘制字符图形*/
//	INTERACT_ID_bigbome_num					= 0x02ff
//};





//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct;
//}ext_client_custom_graphic_single_t;
///*客户端绘制一个图形*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[2];
//}ext_client_custom_graphic_double_t;
///*客户端绘制两个图形*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[5];
//}ext_client_custom_graphic_five_t;
///*客户端绘制五个图形*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct[7];
//}ext_client_custom_graphic_seven_t;
///*客户端绘制七个图形*/

//typedef __packed struct
//{
//	graphic_data_struct_t grapic_data_struct;
//	uint8_t data[30];
//}ext_client_custom_character_t;
///*客户端绘制字符*/

///* 自定义帧头 */
//typedef __packed struct
//{
//	uint8_t  SOF; 	
//	uint16_t DataLength;
//	uint8_t  Seq; 	
//	uint8_t  CRC8; 
//}ext_FrameHeader;
///* 交互数据接收信息：0x0301  */
//typedef __packed struct 
//{ 
//	uint16_t data_cmd_id;    
//	uint16_t send_ID;    
//	uint16_t receiver_ID; 
//} ext_student_interactive_header_data_t; 

///*机器人间交互数据*/
//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
//	graphic_data_struct_t clientData;		//数据段
//	uint16_t	FrameTail;								//帧尾
//}ext_student_interactive_data_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
//	graphic_Intdata_struct_t clientData[2];//数据段
//	uint16_t	FrameTail;								//帧尾
//}ext_student_interactive_data1_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
//	graphic_Floatdata_struct_t clientData[2];//数据段
//	uint16_t	FrameTail;								//帧尾
//}ext_student_floateractive_data2_t;
///////////////////////////////////////////////////////////////////////
//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
//	graphic_data_struct_t clientData[7];//数据段]
//	uint16_t	FrameTail;								//帧尾
//}ext_student_interactive_data7_t;

//typedef __packed struct
//{
//	ext_FrameHeader txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
//	ext_client_custom_character_t clientData;//数据段]
//	uint16_t	FrameTail;								//帧尾
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
///*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓图形化绘制API↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/
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
