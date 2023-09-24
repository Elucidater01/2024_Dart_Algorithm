/**
 * @file        judge_potocol.c
 * @author      MarkVimy
 * @Version     V1.0
 * @date        4-November-2020
 * @brief       Referee Potocol.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "judge_potocol.h"

#include "judge_sensor.h"
#include "crc.h"
#include "string.h"

/* Private macro -------------------------------------------------------------*/
#define JUDGE_FRAME_HEADER	0xA5

/* Private function prototypes -----------------------------------------------*/
/* 帧字节偏移 */
enum judge_frame_offset_t {
	FRAME_HEADER	= 0,
	CMD_ID			= 5,
	DATA_SEG		= 7
};

/* 帧头字节偏移 */
enum judge_frame_header_offset_t {//帧头的详细位置
	SOF			= 0,
	DATA_LENGTH	= 1,
	SEQ			= 3,
	CRC8		= 4
};//FRAME_HEADER

enum judge_cmd_id_t {//命令码ID
	ID_GAME_STATUS 									= 0x0001,	// 比赛状态	1HZ		
	ID_GAME_RESULT 									= 0x0002,	// 比赛结果 		
	ID_GAME_ROBOT_HP 								= 0x0003,	// 机器人血量数据  1HZ	
	ID_DART_STATUS									= 0x0004,	// 飞镖发射状态
	ID_ICRA_BUFF_DEBUFF_ZONE_STATUS = 0x0005,	// 人工智能挑战赛加成与惩罚区状态 1HZ
	
	ID_EVENT_DATA 									= 0x0101,	// 场地事件数据		
	ID_SUPPLY_PROJECTILE_ACTION 		= 0x0102,	// 补给站动作标识	
	//ID_SUPPLY_PROJECTILE_BOOKING 	= 0x0103,	// 请求补给站补弹子弹
	ID_REFEREE_WARNING 							= 0x0104,	// 裁判警告信息
	ID_DART_REMAINING_TIME					= 0x0105,	// 飞镖发射口倒计时  1HZ
	
	ID_GAME_ROBOT_STATUS 						= 0x0201,	// 比赛机器人状态  10HZ
	ID_POWER_HEAT_DATA 							= 0x0202,	// 实时功率热量数据 50Hz
	ID_GAME_ROBOT_POS								= 0x0203,	// 机器人位置  10Hz
	ID_BUFF													= 0x0204,	// 机器人增益
	ID_AERIAL_ROBOT_ENERGY					= 0x0205,	// 空中机器人能量状态
	ID_ROBOT_HURT										= 0x0206,	// 机器人伤害状态
	ID_SHOOT_DATA										= 0x0207,	// 实时射击信息
	ID_BULLET_REMAINING							= 0x0208,	// 子弹剩余发射数  1HZ
	ID_RFID_STATUS									= 0x0209,	// 机器人RFID状态  1HZ
	
	ID_COMMUNICATION								= 0x0301,	// 机器人间交互数据(发送方触发发送)
};

enum judge_data_length_t {
	/* Std */
	LEN_FRAME_HEAD 	= 5,	// 帧头长度
	LEN_CMD_ID 		= 2,	// 命令码长度
	LEN_FRAME_TAIL 	= 2,	// 帧尾CRC16
	/* Ext */
	// 0x000x
	LEN_GAME_STATUS 				= 11,//!!!!!11  0x0001
	LEN_GAME_RESULT 				= 1,//         0x0002
	LEN_GAME_ROBOT_HP 			= 28,//28      0x0003
	LEN_DART_STATUS					= 3,//         0x0004
	LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS= 11,//11  0x0101
	
	// 0x010x
	LEN_EVENT_DATA					= 4,
	LEN_SUPPLY_PROJECTILE_ACTION	= 3,
	//LEN_SUPPLY_PROJECTILE_BOOKING	= 3,
	LEN_REFEREE_WARNING				= 2,
	LEN_DART_REMAINING_TIME			= 1,
	
	// 0x020x
	LEN_GAME_ROBOT_STATUS			= 15,
	LEN_POWER_HEAT_DATA 			= 14,
	LEN_GAME_ROBOT_POS				= 16,
	LEN_BUFF		 				= 1,
	LEN_AERIAL_ROBOT_ENERGY 		= 3,
	LEN_ROBOT_HURT					= 1,
	LEN_SHOOT_DATA					= 6,
	LEN_BULLET_REMAINING	 		= 2,
	LEN_RFID_STATUS					= 4,
};

/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart5;
/* Private variables ---------------------------------------------------------*/
ext_student_interactive_data_t 	tx_client_data;
ext_student_interactive_data1_t 	tx_client_data1;
ext_student_interactive_data7_t 	tx_client_data2;
ext_student_interactive_data_char_t	tx_clinet_char;
ext_student_interactive_data_t 	tx_teammate_data;
ext_student_interactive_data_t  rx_teammate_data;

uint8_t CliendTxBuffer[200];
uint8_t TeammateTxBuffer[200];

robot_interactive_data_t								robot_interactive_data;/*交互数据包*/
ext_client_custom_graphic_delete_t			graphic_delete;/*客户端删除图形*/
ext_client_custom_graphic_single_t			graphic_single;/*客户端绘制一个图形*/
ext_client_custom_graphic_double_t			graphic_double;/*客户端绘制两个图形*/
ext_client_custom_graphic_five_t				graphic_five;/*客户端绘制五个图形*/
ext_client_custom_graphic_seven_t				graphic_seven;/*客户端绘制七个图形*/
ext_client_custom_character_t						character;/*客户端绘制字符*/
ext_student_interactive_header_data_t 	interactive_header_data;/*交互数据的数据结构体*/

/* Private functions ---------------------------------------------------------*/
bool Determine_ID(void);

/* Exported functions --------------------------------------------------------*/
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

void judge_sensor_update(judge_sensor_t *judge_sen, uint8_t *rxBuf)
{
	uint8_t  res = false;
	uint16_t frame_length;
	uint16_t cmd_id;	
	judge_info_t *judge_info = judge_sen->info;
	
	if( rxBuf == NULL )
	{
		judge_info->data_valid = false;
		return;
	}
	
	memcpy(&judge_info->fream_header, rxBuf, LEN_FRAME_HEAD);
	
	/* 帧首字节是否为0xA5 */
	if(rxBuf[SOF] == JUDGE_FRAME_HEADER) 
	{
		/* 帧头CRC8校验 */
		if(Verify_CRC8_Check_Sum(rxBuf, LEN_FRAME_HEAD) == true) 
		{
			/* 统计一帧的总数据长度，用于CRC16校验 */     // 长度两个字节
			frame_length = LEN_FRAME_HEAD + LEN_CMD_ID + judge_info->fream_header.data_length + LEN_FRAME_TAIL;
			
			if(Verify_CRC16_Check_Sum(rxBuf, frame_length) == true)
			{
				res = true;
				
				cmd_id = (rxBuf[CMD_ID+1] << 8 | rxBuf[CMD_ID]);
				
				switch(cmd_id)
				{
					case ID_GAME_STATUS: 
						memcpy(&judge_info->game_status, (rxBuf+DATA_SEG), LEN_GAME_STATUS);
						break;
					
					case ID_GAME_RESULT: 
						memcpy(&judge_info->game_result, (rxBuf+DATA_SEG), LEN_GAME_RESULT);
						break;
					
					case ID_GAME_ROBOT_HP: 
						memcpy(&judge_info->game_robot_HP, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_HP);
						break;//所有机器人的血量都可以获得
					
					case ID_DART_STATUS: 
						memcpy(&judge_info->dart_status, (rxBuf+DATA_SEG), LEN_DART_STATUS);
						
						break;
					
					case ID_EVENT_DATA: 
						memcpy(&judge_info->event_data, (rxBuf+DATA_SEG), LEN_EVENT_DATA);
						break;
					
					case ID_SUPPLY_PROJECTILE_ACTION: 
						memcpy(&judge_info->supply_projectile_action, (rxBuf+DATA_SEG), LEN_SUPPLY_PROJECTILE_ACTION);
						break;
					
					case ID_REFEREE_WARNING: 
						memcpy(&judge_info->referee_warning, (rxBuf+DATA_SEG), LEN_REFEREE_WARNING);
						break;
					
					case ID_DART_REMAINING_TIME: 
						memcpy(&judge_info->dart_remaining_time, (rxBuf+DATA_SEG), LEN_DART_REMAINING_TIME);
						break;
						
					case ID_GAME_ROBOT_STATUS: 
						memcpy(&judge_info->game_robot_status, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_STATUS);
						Determine_ID();
						break;
					
					case ID_POWER_HEAT_DATA: 
						memcpy(&judge_info->power_heat_data, (rxBuf+DATA_SEG), LEN_POWER_HEAT_DATA);
						break;
					
					case ID_GAME_ROBOT_POS: 
						memcpy(&judge_info->game_robot_pos, (rxBuf+DATA_SEG), LEN_GAME_ROBOT_POS);
						break;
					
					case ID_BUFF: 
						memcpy(&judge_info->buff, (rxBuf+DATA_SEG), LEN_BUFF);
						break;
					
					case ID_AERIAL_ROBOT_ENERGY: 
						memcpy(&judge_info->aerial_robot_energy, (rxBuf+DATA_SEG), LEN_AERIAL_ROBOT_ENERGY);
						break;
					
					case ID_ROBOT_HURT: 
						memcpy(&judge_info->robot_hurt, (rxBuf+DATA_SEG), LEN_ROBOT_HURT);
						break;
					
					case ID_SHOOT_DATA: 
						memcpy(&judge_info->shoot_data, (rxBuf+DATA_SEG), LEN_SHOOT_DATA);
						break;
					
					case ID_BULLET_REMAINING: 
						memcpy(&judge_info->bullet_remaining, (rxBuf+DATA_SEG), LEN_BULLET_REMAINING);
						break;
					
					case ID_RFID_STATUS: 
						memcpy(&judge_info->rfid_status, (rxBuf+DATA_SEG), LEN_RFID_STATUS);
						break;
						
					case ID_COMMUNICATION: 
						//JUDGE_ReadFromCom();
						break;
				}
			}
		}
		
		/* 帧尾CRC16下一字节是否为0xA5 */
		if(rxBuf[frame_length] == JUDGE_FRAME_HEADER)
		{
			/* 如果一个数据包出现了多帧数据就再次读取 */
			judge_sensor_update( judge_sen, &rxBuf[frame_length] );
		}
	}
	
	judge_info->data_valid = res;
	if(judge_info->data_valid != true)
		judge_info->err_cnt++;
	
	// 接收到数据表示在线
	judge_info->offline_cnt = 0;
}

/**
  * @brief  判断自己ID
  * @param  void
  * @retval  RED  BLUE
  * @attention   数据打包,打包完成后通过串口发送到裁判系统
  */
bool Determine_ID(void)//!!!!!!!!!!!!之前是bool
{	
	if(judge_info.game_robot_status.robot_id < 10)
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
	}
	else 
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
	}
}

/**
  * @brief  客户端图形界面发送
  * @param  void
  * @retval void
  * @attention   数据打包,打包完成后通过串口发送到裁判系统
  */
ext_client_custom_character_t test_GUI;
void Send_Char_To_Client0(void)//发送字符串
{
	static uint8_t graphic_num = 1;//画多少个图形
	
	/* 帧头 */
	tx_clinet_char.txFrameHeader.SOF = 0xA5;
	tx_clinet_char.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t)  //数据头结构6字节
																					+ sizeof(ext_client_custom_character_t)*graphic_num;  //数据段
	tx_clinet_char.txFrameHeader.Seq = 0;
	
	memcpy(CliendTxBuffer, &tx_clinet_char.txFrameHeader, sizeof(ext_FrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(ext_FrameHeader));//头校验
	
	/* 数据段 头结构 */
	tx_clinet_char.CmdID = 0x0301;											//学生机器人间通信 cmd_id 0x0301	
	tx_clinet_char.dataFrameHeader.data_cmd_id = 0x0110;//可以在0x0100~0x0110段选取，具体ID含义自定义
	tx_clinet_char.dataFrameHeader.send_ID = judge_info.game_robot_status.robot_id;
	tx_clinet_char.dataFrameHeader.receiver_ID = judge_info.ids.client_infantry3;
	
	/* 数据段 绘图层 */ //索引       第六个字符串长度                    目标字符串 
	Draw_Char(&test_GUI, 	"TR0", 1, GREEN, 50, 5, 5, (1920/2), (1080/3*2), "RP_NB");
	memcpy(&tx_clinet_char.clientData, &test_GUI, sizeof(ext_client_custom_character_t)*graphic_num);
	
	/* 数据段 打包 */	
	memcpy(CliendTxBuffer+5, (uint8_t*)&tx_clinet_char.CmdID, 2+tx_clinet_char.txFrameHeader.DataLength);//加上命令码长度2
	
	/* 帧尾 */
	Append_CRC16_Check_Sum(CliendTxBuffer, sizeof(tx_clinet_char));
	
	HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(tx_clinet_char));
}

graphic_Intdata_struct_t test_GUI1[2];
void Send_Data_To_Client1(void)//
{
	static uint8_t graphic_num = 2;//画多少个图形
	
	/* 帧头 */
	tx_client_data1.txFrameHeader.SOF = 0xA5;
	tx_client_data1.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t)\
																					+ sizeof(graphic_data_struct_t)*graphic_num;
	tx_client_data1.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer, &tx_client_data1.txFrameHeader, sizeof(ext_FrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(ext_FrameHeader));//头校验
	
	/* 数据段 头结构 */
	tx_client_data1.CmdID = 0x0301;											//学生机器人间通信 cmd_id 0x0301	
	tx_client_data1.dataFrameHeader.data_cmd_id = 0x0102;//可以在0x0100~0x0110段选取，具体ID含义自定义
	tx_client_data1.dataFrameHeader.send_ID = judge_info.game_robot_status.robot_id;
	tx_client_data1.dataFrameHeader.receiver_ID = judge_info.ids.client_infantry3;
	
	/* 数据段 绘图层 */
	//画字符有bug，画出乱码，待解决
	Draw_Int(&test_GUI1[0], 	"TR1", 3, WHITE, 40, 4, (1920-400), (1080/4*3), 6);
	Draw_Int(&test_GUI1[1], "TR2", 2, WHITE, 40, 4, (1920-400), (1080/4*2), 7);
	memcpy(tx_client_data1.clientData, test_GUI1, sizeof(graphic_data_struct_t)*graphic_num);
	
	/* 数据段 打包 */	
	memcpy(CliendTxBuffer+5, (uint8_t*)&tx_client_data1.CmdID, 2+tx_client_data1.txFrameHeader.DataLength);
	
	/* 帧尾 */
	Append_CRC16_Check_Sum(CliendTxBuffer, sizeof(tx_client_data1));
	
	HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(tx_client_data1));
}
/////////////////////////////////////////////////////////////////////////////////
ext_student_floateractive_data2_t tx_floatclient_data2;
graphic_Floatdata_struct_t test_GUI4[2];
void Send_Data_To_Client3(void)//
{
	static uint8_t graphic_num = 2;//画多少个图形
	
	/* 帧头 */
	tx_floatclient_data2.txFrameHeader.SOF = 0xA5;
	tx_floatclient_data2.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t)//6个字节
																					+ sizeof(graphic_data_struct_t)*graphic_num;
	tx_floatclient_data2.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer, &tx_floatclient_data2.txFrameHeader, sizeof(ext_FrameHeader));//定下前五个字节
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(ext_FrameHeader));//头校验
	
	/* 数据段 头结构 */
	tx_floatclient_data2.CmdID = 0x0301;											//学生机器人间通信 cmd_id 0x0301	
	tx_floatclient_data2.dataFrameHeader.data_cmd_id = 0x0102;//可以在0x0100~0x0110段选取，具体ID含义自定义
	tx_floatclient_data2.dataFrameHeader.send_ID = judge_info.game_robot_status.robot_id;
	tx_floatclient_data2.dataFrameHeader.receiver_ID = judge_info.ids.client_infantry3;
	
	/* 数据段 绘图层 */
	Draw_Float(&test_GUI4[0], "T1R", 4, WHITE, 40, 2, 4, (1920-400), (1080/4*3), 9.0);
	Draw_Float(&test_GUI4[1], "T2R", 5, WHITE, 40, 2, 4, (1920-400), (1080/4*2), 12.18);
	memcpy(tx_floatclient_data2.clientData, test_GUI4, sizeof(graphic_data_struct_t)*graphic_num);
	
	/* 数据段 打包 */	
	memcpy(CliendTxBuffer+5, (uint8_t*)&tx_floatclient_data2.CmdID, 2+tx_floatclient_data2.txFrameHeader.DataLength);
	
	/* 帧尾 */
	Append_CRC16_Check_Sum(CliendTxBuffer, sizeof(tx_floatclient_data2));
	
	HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(tx_floatclient_data2));
}








////////////////////////////////////////////////////////////////////////////////
graphic_data_struct_t test_GUI2[7];
int32_t write_num = 0;
void Send_Data_To_Client2(void)
{
	static uint8_t graphic_num = 7;//画多少个图形就填几
	
	/* 帧头 */
	tx_client_data2.txFrameHeader.SOF = 0xA5;
	tx_client_data2.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t)\
																					+ sizeof(graphic_data_struct_t)*graphic_num;
	tx_client_data2.txFrameHeader.Seq = 0;
	memcpy(CliendTxBuffer, &tx_client_data2.txFrameHeader, sizeof(ext_FrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(ext_FrameHeader));//头校验
	
	/* 数据段 头结构 */
	tx_client_data2.CmdID = 0x0301;											//学生机器人间通信 cmd_id 0x0301	
	tx_client_data2.dataFrameHeader.data_cmd_id = 0x0104;//可以在0x0100~0x0110段选取，具体ID含义自定义
	tx_client_data2.dataFrameHeader.send_ID = judge_info.game_robot_status.robot_id;
	tx_client_data2.dataFrameHeader.receiver_ID = judge_info.ids.client_infantry3;
	
	/* 数据段 绘图层 */
	Draw_Line(&test_GUI2[0], "TR3", 3, PINK, 5, (1920/2-40), (1080/2-30), (1920/2+40), (1080/2+30));
	Draw_Line(&test_GUI2[1], "TR4", 4, PINK, 5, (1920/2-40), (1080/2+30), (1920/2+40), (1080/2-30));
	Draw_Rectangle(&test_GUI2[2], "TR5", 5, GREEN, 5, (1920/2-60), (1080/2-40), (1920/2+60), (1080/2+40));
	Draw_Rectangle(&test_GUI2[3], "TR6", 6, WHITE, 5, (1920/2-90), (1080/2-60), (1920/2+90), (1080/2+60));
	Draw_Circle(&test_GUI2[4], "TR7", 7, RED_BLUE, 5, (1920/2), (1080/2), 200);
	Draw_Oval(&test_GUI2[5], "TR8", 8, CYAN_BLUE, 5, (1920/2), (1080/2), 200, 150);
	Draw_ARC(&test_GUI2[6], "TR9", 9, CYAN_BLUE, (1920/2-100), (1920/2+100), 5, (1920/2), (1080/2+100), 200, 150);
	memcpy(tx_client_data2.clientData, &test_GUI2, sizeof(graphic_data_struct_t)*graphic_num);
	
	/* 数据段 打包 */	
	memcpy(CliendTxBuffer+5, (uint8_t*)&tx_client_data2.CmdID, 2+tx_client_data2.txFrameHeader.DataLength);
	
	/* 帧尾 */
	Append_CRC16_Check_Sum(CliendTxBuffer, sizeof(tx_client_data2));
	
	HAL_UART_Transmit_DMA(&huart5, CliendTxBuffer, sizeof(tx_client_data2));
}


/**
* @brief  发送给队友信息
  * @param  void
  * @retval  RED  BLUE
  * @attention   数据打包,打包完成后通过串口发送到裁判系统
  */
void Send_Data_To_Teammate(void)
{
	static uint8_t data_len = 0;
	/* 帧头 */
	tx_teammate_data.txFrameHeader.SOF = 0xA5;
	tx_teammate_data.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t)\
																						+ sizeof(robot_interactive_data_t);
	tx_teammate_data.txFrameHeader.Seq = 0;
	memcpy(TeammateTxBuffer, &tx_teammate_data.txFrameHeader, sizeof(ext_FrameHeader));
	Append_CRC8_Check_Sum(TeammateTxBuffer, sizeof(ext_FrameHeader));//头校验
	
	/* 数据段 头结构 */
	tx_teammate_data.CmdID = 0x0301;											//学生机器人间通信 cmd_id 0x0301
	tx_teammate_data.dataFrameHeader.data_cmd_id = 0x0200;//可以在0x0200~0x02FF段选取，具体ID含义自定义,全兵种统一
	tx_teammate_data.dataFrameHeader.send_ID = judge_info.game_robot_status.robot_id;
	tx_teammate_data.dataFrameHeader.receiver_ID = judge_info.ids.teammate_hero;
	
	/*数据段 数据内容 */
	//需要用到的数据，等待填充
	//tx_teammate_data.clientData.data[]=  ;
	
	/*复制数据到发送包*/
	memcpy(TeammateTxBuffer+5,(uint8_t *)&tx_teammate_data.CmdID, \
		sizeof(tx_teammate_data.CmdID)+sizeof(tx_teammate_data.dataFrameHeader)+sizeof(tx_teammate_data.clientData));
	
	/* 帧尾 */
	Append_CRC16_Check_Sum(TeammateTxBuffer,sizeof(tx_teammate_data));//尾校验
	
	HAL_UART_Transmit_DMA(&huart5, TeammateTxBuffer, sizeof(tx_client_data));		
}

/**
* @brief 绘制一个矩形
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					width 线宽
					start_x 开始点x坐标
					start_y 开始点y坐标
					diagonal_x 对角点x坐标
					diagonal_y 对角点y坐标
* @return NONE
*/
void Draw_Rectangle(graphic_data_struct_t* graphic,
										const uint8_t *name,
										uint8_t layer,
										Graphic_Color color,
										uint16_t width,
										uint16_t start_x,
										uint16_t start_y,
										uint16_t diagonal_x,
										uint16_t diagonal_y)
{
	Add_Graphic(graphic,name,layer,RECTANGLE,color,0,0,width,start_x,start_y,0,diagonal_x,diagonal_y);
}

/**
* @brief 绘制一条直线
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					width 线宽
					start_x 开始点x坐标
					start_y 开始点y坐标
					end_x 终点x坐标
					end_y 终点y坐标
* @return NONE
*/
void Draw_Line(graphic_data_struct_t* graphic,
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t width,
							uint16_t start_x,
							uint16_t start_y,
							uint16_t end_x,
							uint16_t end_y)
{
	Add_Graphic(graphic,name,layer,LINE,color,0,0,width,start_x,start_y,0,end_x,end_y);
}

/**
* @brief 绘制一个圆
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					width 线宽
					center_x 圆心x坐标
					center_y 圆心y坐标
					radius 半径
* @return NONE
*/
void Draw_Circle(graphic_data_struct_t* graphic,
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t width,
							uint16_t center_x,
							uint16_t center_y,
							uint16_t radius)
{
	Add_Graphic(graphic,name,layer,CIRCLE,color,0,0,width,center_x,center_y,radius,0,0);
}

/**
* @brief 绘制一个椭圆
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					width 线宽
					center_x 圆心x坐标
					center_y 圆心y坐标
					axis_x x半轴
					axis_y y半轴
* @return NONE
*/
void Draw_Oval(graphic_data_struct_t* graphic,
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t width,
							uint16_t center_x,
							uint16_t center_y,
							uint16_t axis_x,
							uint16_t axis_y)
{	
	Add_Graphic(graphic,name,layer,OVAL,color,0,0,width,center_x,center_y,0,axis_x,axis_y);
}

/**
* @brief 绘制一个圆弧
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					start_angle 起始角度
					end_angle 	终止角度
					width 线宽
					center_x 圆心x坐标
					center_y 圆心y坐标
					axis_x x半轴
					axis_y y半轴
* @return NONE
*/
void Draw_ARC(graphic_data_struct_t* graphic,
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t start_angle,
							uint16_t end_angle,
							uint16_t width,
							uint16_t center_x,
							uint16_t center_y,
							uint16_t axis_x,
							uint16_t axis_y)
{
	Add_Graphic(graphic,name,layer,ARC,color,start_angle,end_angle,width,center_x,center_y,0,axis_x,axis_y);
}

/**
* @brief 绘制浮点数
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					font_size	字体大小
					accuracy 小数点位数
					width 线宽
					start_x	起始x坐标
					start_y 起始y坐标
					number 显示的数字
* @return NONE
*/
void Add_Graphic_Float(graphic_Floatdata_struct_t* graphic,
									const uint8_t* name,
									uint32_t layer,
									uint32_t type,
									uint32_t color,
									uint32_t start_angle,
									uint32_t end_angle,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
                  float Fnumber)
{
	graphic->color = color;
	for(uint8_t i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];
	graphic->layer = layer;
	graphic->graphic_tpye = type;
	graphic->operate_tpye = ADD;
	graphic->start_x = start_x;
	graphic->start_y = start_y;
	graphic->start_angle = start_angle;
	graphic->end_angle = end_angle;
	graphic->width = width;
	graphic->number = Fnumber;
}
void Draw_Float(graphic_Floatdata_struct_t* graphic,
								const uint8_t *name,
								uint8_t layer,
								Graphic_Color color,
								uint16_t font_size,
								uint16_t accuracy,
								uint16_t width,
								uint32_t start_x,
								uint32_t start_y,
								float number)
{
	float num_tmp_f = (float)number;
//	uint32_t num_tmp_u=0x00;
//	memcpy(&num_tmp_u,&num_tmp_f,4);
	Add_Graphic_Float(graphic,name,layer,FLOAT,color,font_size,accuracy,width,start_x,start_y,num_tmp_f);
}

/**
* @brief 绘制整数
* @param  graphic *	图层信息
					name			图层名
					layer 		图层数
					color 		图形颜色
					font_size	字体大小
					width 		线宽
					start_x		起始x坐标
					start_y 	起始y坐标
					number 		显示的数字
* @return NONE
*/
void Add_Graphic_Int(graphic_Intdata_struct_t* graphic,
									const uint8_t* name,
									uint32_t layer,
									uint32_t type,
									uint32_t color,
									uint32_t start_angle,
									uint32_t end_angle,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
                  int number)
{
	graphic->color = color;
	for(uint8_t i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];
	graphic->layer = layer;
	graphic->graphic_tpye = type;
	graphic->operate_tpye = ADD;
	
	graphic->start_angle = start_angle;
	graphic->end_angle = end_angle;
	graphic->width = width;
	graphic->start_x = start_x;
	graphic->start_y = start_y;
	graphic->number = number;
}
void Draw_Int(graphic_Intdata_struct_t* graphic,
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t font_size,
							uint16_t width,
							uint16_t start_x,
							uint16_t start_y,
							int32_t number)
{
//	int32_t num_tmp_u;
//	memcpy(&num_tmp_u,&number,4);
	Add_Graphic_Int(graphic,name,layer,INT,color,font_size,0,width,start_x,start_y,\
							number);
}


/**
* @brief 绘制字符
* @param  graphic *字符图层信息
					name	图层名
					layer 图层数
					color 图形颜色
					font_size	字体大小
					length	字符长度
					width 线宽
					start_x	起始x坐标
					start_y 起始y坐标
					character 字符串信息
* @return NONE
*/
void Draw_Char(ext_client_custom_character_t* graphic,//图形配置15*n个字节
							const uint8_t *name,
							uint8_t layer,
							Graphic_Color color,
							uint16_t font_size,
							uint16_t length,
							uint16_t width,
							uint16_t start_x,
							uint16_t start_y,
							const uint8_t *character)
//                                                                 字体大小 字符长度
{//                                                   图层类型     初始角度 最终角度
	Add_Graphic(&(graphic->grapic_data_struct),name,layer,CHAR,color,font_size,length,width,start_x,start_y,0,0,0);
	memcpy(graphic->data, character, length);
	     //uint8_t data[30];
}


/**
* @brief 新增一个图层，填充图层信息
* @param  graphic *图层信息
					name	图层名
					layer 图层数
					type  图形类型
					color 图形颜色
					等等
* @return NONE
*/
void Add_Graphic(graphic_data_struct_t* graphic,
									const uint8_t* name,
									uint32_t layer,
									uint32_t type,
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
	graphic->color = color;
	for(uint8_t i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];
	graphic->layer = layer;
	graphic->graphic_tpye = type;
	graphic->operate_tpye = ADD;
	
	graphic->start_angle = start_angle;
	graphic->end_angle = end_angle;
	graphic->width = width;
	graphic->start_x = start_x;
	graphic->start_y = start_y;
	graphic->radius = radius;
	graphic->end_x = end_x;
	graphic->end_y = end_y;
}

/**
* @brief 删除目标客户端所有图层的API函数
* @param Interact_Target 目标客户端ID
* @return NONE
*/
void Delete_All_Graphic_Layer(uint16_t target)
{
	static ext_student_interactive_header_data_t 	interactive_header_data;/*交互数据的数据结构体*/

	uint8_t data[17]={0};	/*6+2+9*/  	/*临时数组*/
	uint16_t data_length = LEN_INTERACT_delete_graphic;	 /*data字段的长度*/
	uint8_t CRC8=0x00;
	uint16_t CRC16=0x00;
	/*两个校验变量*/
	
	data[0] = 0xA5;
	data[1] = data_length>>8;
	data[2] = data_length;
	data[3] = 1;
	CRC8 = Get_CRC8_Check_Sum(data,4,0xff);
	data[4] = CRC8;
	/*SOF*/
	
	data[5] = ID_interactive_header_data>>8;
	data[6] = (uint8_t)ID_interactive_header_data;
	/*填充交互的cmd-ID*/
	
	interactive_header_data.data_cmd_id = INTERACT_ID_delete_graphic;
	interactive_header_data.receiver_ID = target;
	interactive_header_data.send_ID = judge_info.game_robot_status.robot_id;
	memcpy(data+7,&interactive_header_data,6);
	/*填充data包头*/
	
	graphic_delete.operate_type = 2;
	graphic_delete.layer = 9;
	memcpy(data+13,&graphic_delete,2);
	/*填充data内容*/
	
	CRC16 = Get_CRC16_Check_Sum(data,15,0xffff);
	data[15] = CRC16>>8;
	data[16] = CRC16;
	/*获取CRC16值*/
	
	memcpy(&robot_interactive_data,data,17);
	
	HAL_UART_Transmit_DMA(&huart5, TeammateTxBuffer, sizeof(tx_client_data));		
}


/**
 *	@brief	在串口5中解析裁判系统数据协议
 */
void USART5_rxDataHandler(uint8_t *rxBuf)
{	
	judge_sensor.update(&judge_sensor, rxBuf);
	judge_sensor.check(&judge_sensor);
}
