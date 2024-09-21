#ifndef __MOTOR_DEF_H
#define __MOTOR_DEF_H

#include "stm32f4xx_hal.h"

/*-------------------------电机接收ID定义开始--------------------------*/

// RM3508电机 CAN ID(RX)
#define RM3508_CAN_ID_201 (0x201U)
#define RM3508_CAN_ID_202 (0x202U)
#define RM3508_CAN_ID_203 (0x203U)
#define RM3508_CAN_ID_204 (0x204U)
#define RM3508_CAN_ID_205 (0x205U)
#define RM3508_CAN_ID_206 (0x206U)
#define RM3508_CAN_ID_207 (0x207U)
#define RM3508_CAN_ID_208 (0x208U)


// GM6020电机 CAN ID(RX)
#define GM6020_CAN_ID_205 (0x205U)
#define GM6020_CAN_ID_206 (0x206U)
#define GM6020_CAN_ID_207 (0x207U)
#define GM6020_CAN_ID_208 (0x208U)
#define GM6020_CAN_ID_209 (0x209U)
#define GM6020_CAN_ID_20A (0x20AU)
#define GM6020_CAN_ID_20B (0x20BU)


// RM2006电机 CAN ID(RX)
#define RM2006_CAN_ID_201 (0x201U)
#define RM2006_CAN_ID_202 (0x202U)
#define RM2006_CAN_ID_203 (0x203U)
#define RM2006_CAN_ID_204 (0x204U)
#define RM2006_CAN_ID_205 (0x205U)
#define RM2006_CAN_ID_206 (0x206U)
#define RM2006_CAN_ID_207 (0x207U)
#define RM2006_CAN_ID_208 (0x208U)


// RM3510电机 CAN ID(RX)
#define RM3510_CAN_ID_201 (0x201U)
#define RM3510_CAN_ID_202 (0x202U)
#define RM3510_CAN_ID_203 (0x203U)
#define RM3510_CAN_ID_204 (0x204U)


// GM3510电机 CAN ID(RX)
#define GM3510_CAN_ID_205 (0x205U)
#define GM3510_CAN_ID_206 (0x206U)
#define GM3510_CAN_ID_207 (0x207U)


//KT9015电机  CAN ID(RX)
#define KT9015_CAN_ID_141 (0x141U)
#define KT9015_CAN_ID_142 (0x142U)
#define KT9015_CAN_ID_143 (0x143U)
#define KT9015_CAN_ID_144 (0x144U)
#define KT9015_CAN_ID_145 (0x145U)
#define KT9015_CAN_ID_146 (0x146U)
#define KT9015_CAN_ID_147 (0x147U)
#define KT9015_CAN_ID_148 (0x148U)


//KT9025电机  CAN ID(RX)
#define KT9025_CAN_ID_141 (0x141U)
#define KT9025_CAN_ID_142 (0x142U)
#define KT9025_CAN_ID_143 (0x143U)
#define KT9025_CAN_ID_144 (0x144U)
#define KT9025_CAN_ID_145 (0x145U)
#define KT9025_CAN_ID_146 (0x146U)
#define KT9025_CAN_ID_147 (0x147U)
#define KT9025_CAN_ID_148 (0x148U)


/*-------------------------电机接收ID定义结束--------------------------*/




#define OFFLINE_LINE_CNT_MAX 100

//RM2006或者RM3508发送ID指令
#define RM_F_ID 0x200
#define RM_B_ID 0x1FF

//GM6020发送ID指令
#define GM_F_ID 0x1FF
#define GM_B_ID 0x2FF


//KT电机多电机发送ID指令，应注意：如果是单电机发送ID指令，已经保存在了电机的id结构体中
#define KT_MULTI_TX_ID   0x280



//KT允许发送的最大值 
#define KT_TX_ENCODER_OFFSET_MAX    16383*4         //0~16383*4
#define KT_TX_POWER_CONTROL_MAX     1000            //-1000~1000
#define KT_TX_IQ_CONTROL_MAX        500             //-2000~2000    1A--->48的值   930
#define KT_TX_ANGLE_SIGNLE_MAX      35999           //0~35999
#define K_CURRENT_TURN		          62.5f			      //电流值，反馈数值2000 对应 32A  2000 / 32


//KT电机产品型号
#define TORQUE_CONSTANT		           0.32f			    //扭矩常数N.M/A
#define MAX_TORQUE                   0.5f            //峰值扭矩N.M    4.2
#define SPEED_CONSTANT 		           20 				    //转速常数 rpm/V
#define MAX_MOTOR_CURRENT            900					  //最大电流，对应峰值电流15A //////////////
#define REDUCTION_RATIO              1					    //电机减速比	 
#define ROTOR_INERTIA                0.0004656		  //9025转子惯量kg.m^2    
#define MAX_CHAS_MOTOR_SPEED 	       710				    //rpm
#define MAX_CHAS_MOTOR_INIT_SPEED    4680		        //原始反馈速度最大值  
#define ANGLE_UNIT_CONVERSION        0.021973f	    // 360/16384   //////////////////


/*----------------------------自定义枚举类型开始--------------------------------*/
typedef enum motor_kt9025_command_e
{
	PID_RX_ID									 = 0x30, 
	PID_TX_RAM_ID							 = 0x31,  //断电失效
	PID_TX_ROM_ID							 = 0x32,  //断电有效
	ACCEL_RX_ID								 = 0x33,
	ACCEL_TX_ID								 = 0x34,
	ENCODER_RX_ID							 = 0x90,
	ZERO_ENCODER_TX_ID				 = 0x91,	
	ZERO_POSNOW_TX_ID					 = 0x19,	
	
	MOTOR_ANGLE_ID						 = 0x92,
	CIRCLE_ANGLE_ID						 = 0x94,
	STATE1_ID									 = 0x9A,
	CLEAR_ERROR_State_ID			 = 0x9B,
	STATE2_ID									 = 0x9C,
	STATE3_ID									 = 0x9D,
	MOTOR_CLOSE_ID						 = 0x80,
	MOTOR_STOP_ID						   = 0x81,
	MOTOR_RUN_ID							 = 0x88,
	
	TORQUE_OPEN_LOOP_ID        = 0xA0,
	TORQUE_CLOSE_LOOP_ID       = 0xA1,
	SPEED_CLOSE_LOOP_ID        = 0XA2,
	POSI_CLOSE_LOOP_ID1        = 0XA3,   
	POSI_CLOSE_LOOP_ID2        = 0XA4,
	POSI_CLOSE_LOOP_ID3        = 0XA5,
	POSI_CLOSE_LOOP_ID4        = 0XA6,
	POSI_CLOSE_LOOP_ID5        = 0XA7,
	POSI_CLOSE_LOOP_ID6        = 0XA8,
	
	
}motor_kt9025_command_e;



typedef enum motor_state_e
{
	M_OFFLINE = 0,	
	
	M_ONLINE,

	M_TYPE_ERR,
	M_ID_ERR,
	M_INIT_ERR,	
	M_DATA_ERR,
	
}motor_state_e;

typedef enum motor_protect_e
{
	
	M_PROTECT_ON = 0,
	M_PROTECT_OFF ,	
	
}motor_protect_e;

typedef enum motor_init_e
{

	M_DEINIT = 0,
	M_INIT,

}motor_init_e;



typedef enum motor_drive_e
{
	M_CAN1,
	M_CAN2,
	
	M_PWM,

	M_USART1,
	M_USART2,
	M_USART3,	
	M_USART4,
	M_USART5,

}motor_drive_e;

typedef enum motor_type_e
{
	
	GM6020 = 1,
	RM3508,
	RM2006,

	KT9015 = 4,
	KT9025,
	
}motor_type_e;

typedef enum motor_dir_e 
{
	CLOCK_WISE    = 0x00,    
	N_CLOCK_WISE  = 0x01, 

	MOTOR_B,
	MOTOR_F,
		
}motor_dir_e;


/*----------------------------自定义枚举类型结束-------------------------------------*/











/*--------------------------RM电机     自定义结构体类型开始----------------------------------*/
typedef struct RM_motor_rx_info_t
{
	int16_t   angle;						//0~8191   电机最初始的信息
	int16_t 	speed;						//RPM      电机速度
	int16_t 	current;					//         实际转矩
	int16_t		temperature;			//°C       温度
	int16_t 	torque;						//         力矩
	
	int16_t		angle_prev;				//         上次角度
	int16_t		angle_offset;			//         角度差
	int32_t		angle_sum;				//-2147683647~2147683647 上电开始到现在的角度和

}RM_motor_rx_info_t;

typedef struct RM_motor_mec_info_t 
{
	int16_t       mec_mid;        //机械中值
	
	motor_dir_e   dir;	          //方向
	
}RM_motor_mec_info_t;

typedef struct RM_motor_base_info_t
{
	float			motor_out;
//	float			motor_err_in;
//	float			motor_err_out;
	uint8_t   lock_cnt;
}RM_motor_base_info_t;

typedef struct RM_motor_id_info_t
{
	uint32_t   tx_id;   			//发送id
	uint32_t   rx_id;   			//接收id
	uint8_t    buff_p;  			//发送/接收，在数组位置
	
	motor_drive_e   drive_type; 
	motor_type_e    motor_type;
	
}RM_motor_id_info_t;

typedef struct RM_motor_state_info_t
{
	motor_init_e   init_flag;	
	 
	uint8_t        offline_cnt_max;
	uint8_t        offline_cnt;

	motor_state_e  work_state;	
	
}RM_motor_state_info_t;


typedef struct RM_motor_pid_info_t
{
	motor_init_e init_flag;
	
	float		target;
	float	  measure;
	float 	err;
	float 	last_err;
	
	float	  integral;
	
	float 	pout;
	float 	iout;
	float 	dout;
	float 	out;
	
}RM_motor_pid_info_t;

typedef struct RM_motor_pid_set_t
{
	/* 设置参数 */
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	//误差有效的下限值
	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
	
}RM_motor_pid_set_t;

typedef struct RM_motor_pid_t 		//PID单位
{
	RM_motor_pid_info_t info;
	RM_motor_pid_set_t  set;
	
}RM_motor_pid_t;


typedef struct RM_motor_pid_all_t 
{
	RM_motor_pid_t   speed;
	
	//角度串级PID	
	RM_motor_pid_t   angle;	
	RM_motor_pid_t   angle_in;		
	
	//角度和串级PID
	RM_motor_pid_t   position;
	RM_motor_pid_t   position_in;	
	
	RM_motor_pid_t   user_define;
	
}RM_motor_pid_all_t; 


/*--------------------------RM电机      自定义结构体类型结束----------------------------------*/














/*-----------------------KT电机         自定义结构体类型开始---------------------------------*/

typedef struct KT_motor_pid_rx_info_t
{
	uint8_t angleKp;
	uint8_t angleKi;
	uint8_t speedKp;
	uint8_t speedKi;
	uint8_t iqKp;   //转矩环
	uint8_t iqKi;
	
}KT_motor_pid_rx_info_t;

typedef struct KT_motor_pid_tx_info_t
{
	uint8_t angleKp;
	uint8_t angleKi;
	uint8_t speedKp;
	uint8_t speedKi;
	uint8_t iqKp;  //转矩环
	uint8_t iqKi; 
	
}KT_motor_pid_tx_info_t;

typedef struct KT_motor_pid_t
{
	motor_init_e           init_flag;
	KT_motor_pid_rx_info_t rx;
	KT_motor_pid_tx_info_t tx;
	
}KT_motor_pid_t;

typedef struct KT_motor_rx_info_t
{	
	int32_t 	accel;						 //加速度 1dps/s
	
	uint16_t 	encoder;					 //编码器位置             （0~16383 * 4）
	uint16_t 	encoderRaw;				 //编码器原始位置         （0~16383 * 4）
	uint16_t 	encoderOffset;		 //编码器零偏             （0~16383 * 4）
	
	int8_t    temperature;       //温度    1°C/LSB
	uint16_t  voltage;					 //电压    1V
  uint8_t		errorState;				 //高四位无效    低四位0xx0都正常；0xx1温度正常+低压保护；1xx0过温保护+电压正常；1001过温保护+低压保护
	
	int16_t   current;           //转矩电流    返回的值无单位说明   -2048 ~ 2048  （-33~33A）
	int16_t   speed;             //电机转速    1dps/LSB
	
	int16_t 	current_A;				 //ABC三相电流数据     1A/64LSB
	int16_t 	current_B;
	int16_t 	current_C;
		
	int16_t   powerControl;      //输出功率（-1000~1000）  无单位说明
	
	int64_t   motorAngle;        //电机多圈角度   0.01°/LSB  （-2^63~2^63）    顺时针增加，逆时针减少
	
	uint32_t  circleAngle;       //电机单圈角度   0.01°/LSB  （ 0~36000 * 减速比-1） 
															 //以编码零点作为起始点，顺时针增加，再次到达零点时数值变为0
	


	int16_t   angle_add;         //-4096~4096
	
}KT_motor_rx_info_t;


typedef struct KT_motor_tx_info_t
{	
	int32_t 	accel;						 //加速度     1dps/s
	
	uint16_t 	encoderOffset;		 //编码器零偏（0~16383*4）
	
	int16_t   powerControl;      //输出功率 （-1000~1000）     不受上位机的Max Power限制
	
	int16_t   iqControl;         //扭矩电流 （-2000~2000，对应的实际扭矩电流-32A~32A） 不受上位机限制
	
	int32_t   speedControl;      //实际转速为0.01dps/LSB       最大值受上位机设置
	
	int32_t   angle_sum_Control; //多圈角度，  0.01degree/LSB     36000代表360°      最大值受上位机设置
	
	uint16_t  angle_sum_Control_maxSpeed; //多圈角度的最大速度   1dps/LSB   最大值受上位机设置
	
	uint16_t  angle_single_Control; //单圈角度    0.01degree/LSB       0~35999对应实际角度0~359.99°   最大值受上位机设置
	
	uint16_t  angle_single_Control_maxSpeed; //单圈角度的最大速度   1dps/LSB   最大值受上位机设置
	
	uint8_t   angle_single_Control_spinDirection;     //单圈角度旋转方向，0x00顺时针，0x01逆时针
	
	int32_t   angle_add_Control;     //角度位置增量，转动方向由控制量符号决定   0.01degree/LSB     最大值受上位机设置
	
	uint16_t  angle_add_Control_maxSpeed;  //角度位置增量的最大速度  1dps/LSB   最大值受上位机设置
	 
}KT_motor_tx_info_t;



typedef struct KT_motor_id_info_t
{
	uint32_t   tx_id;   			//发送id
	uint32_t   rx_id;   			//接收id
	
	motor_drive_e   drive_type; 
	motor_type_e    motor_type;
	
}KT_motor_id_info_t;


typedef struct KT_motor_state_info_t
{
	motor_init_e     init_flag;	
	 
	uint8_t          offline_cnt_max;
	uint8_t          offline_cnt;

	motor_state_e    work_state;	
	
	motor_protect_e  selfprotect_flag;
}KT_motor_state_info_t;

typedef struct KT_motor_info_t
{
	KT_motor_pid_t         pid_info;
	KT_motor_rx_info_t     rx_info;
	KT_motor_tx_info_t     tx_info;
	KT_motor_id_info_t     id;
	KT_motor_state_info_t  state_info;
	
}KT_motor_info_t;

#endif


