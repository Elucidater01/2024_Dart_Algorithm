#ifndef __MOTOR_DEF_H
#define __MOTOR_DEF_H

#include "stm32f4xx_hal.h"

/*-------------------------�������ID���忪ʼ--------------------------*/

// RM3508��� CAN ID(RX)
#define RM3508_CAN_ID_201 (0x201U)
#define RM3508_CAN_ID_202 (0x202U)
#define RM3508_CAN_ID_203 (0x203U)
#define RM3508_CAN_ID_204 (0x204U)
#define RM3508_CAN_ID_205 (0x205U)
#define RM3508_CAN_ID_206 (0x206U)
#define RM3508_CAN_ID_207 (0x207U)
#define RM3508_CAN_ID_208 (0x208U)


// GM6020��� CAN ID(RX)
#define GM6020_CAN_ID_205 (0x205U)
#define GM6020_CAN_ID_206 (0x206U)
#define GM6020_CAN_ID_207 (0x207U)
#define GM6020_CAN_ID_208 (0x208U)
#define GM6020_CAN_ID_209 (0x209U)
#define GM6020_CAN_ID_20A (0x20AU)
#define GM6020_CAN_ID_20B (0x20BU)


// RM2006��� CAN ID(RX)
#define RM2006_CAN_ID_201 (0x201U)
#define RM2006_CAN_ID_202 (0x202U)
#define RM2006_CAN_ID_203 (0x203U)
#define RM2006_CAN_ID_204 (0x204U)
#define RM2006_CAN_ID_205 (0x205U)
#define RM2006_CAN_ID_206 (0x206U)
#define RM2006_CAN_ID_207 (0x207U)
#define RM2006_CAN_ID_208 (0x208U)


// RM3510��� CAN ID(RX)
#define RM3510_CAN_ID_201 (0x201U)
#define RM3510_CAN_ID_202 (0x202U)
#define RM3510_CAN_ID_203 (0x203U)
#define RM3510_CAN_ID_204 (0x204U)


// GM3510��� CAN ID(RX)
#define GM3510_CAN_ID_205 (0x205U)
#define GM3510_CAN_ID_206 (0x206U)
#define GM3510_CAN_ID_207 (0x207U)


//KT9015���  CAN ID(RX)
#define KT9015_CAN_ID_141 (0x141U)
#define KT9015_CAN_ID_142 (0x142U)
#define KT9015_CAN_ID_143 (0x143U)
#define KT9015_CAN_ID_144 (0x144U)
#define KT9015_CAN_ID_145 (0x145U)
#define KT9015_CAN_ID_146 (0x146U)
#define KT9015_CAN_ID_147 (0x147U)
#define KT9015_CAN_ID_148 (0x148U)


//KT9025���  CAN ID(RX)
#define KT9025_CAN_ID_141 (0x141U)
#define KT9025_CAN_ID_142 (0x142U)
#define KT9025_CAN_ID_143 (0x143U)
#define KT9025_CAN_ID_144 (0x144U)
#define KT9025_CAN_ID_145 (0x145U)
#define KT9025_CAN_ID_146 (0x146U)
#define KT9025_CAN_ID_147 (0x147U)
#define KT9025_CAN_ID_148 (0x148U)


/*-------------------------�������ID�������--------------------------*/




#define OFFLINE_LINE_CNT_MAX 100

//RM2006����RM3508����IDָ��
#define RM_F_ID 0x200
#define RM_B_ID 0x1FF

//GM6020����IDָ��
#define GM_F_ID 0x1FF
#define GM_B_ID 0x2FF


//KT�����������IDָ�Ӧע�⣺����ǵ��������IDָ��Ѿ��������˵����id�ṹ����
#define KT_MULTI_TX_ID   0x280



//KT�����͵����ֵ 
#define KT_TX_ENCODER_OFFSET_MAX    16383*4         //0~16383*4
#define KT_TX_POWER_CONTROL_MAX     1000            //-1000~1000
#define KT_TX_IQ_CONTROL_MAX        500             //-2000~2000    1A--->48��ֵ   930
#define KT_TX_ANGLE_SIGNLE_MAX      35999           //0~35999
#define K_CURRENT_TURN		          62.5f			      //����ֵ��������ֵ2000 ��Ӧ 32A  2000 / 32


//KT�����Ʒ�ͺ�
#define TORQUE_CONSTANT		           0.32f			    //Ť�س���N.M/A
#define MAX_TORQUE                   0.5f            //��ֵŤ��N.M    4.2
#define SPEED_CONSTANT 		           20 				    //ת�ٳ��� rpm/V
#define MAX_MOTOR_CURRENT            900					  //����������Ӧ��ֵ����15A //////////////
#define REDUCTION_RATIO              1					    //������ٱ�	 
#define ROTOR_INERTIA                0.0004656		  //9025ת�ӹ���kg.m^2    
#define MAX_CHAS_MOTOR_SPEED 	       710				    //rpm
#define MAX_CHAS_MOTOR_INIT_SPEED    4680		        //ԭʼ�����ٶ����ֵ  
#define ANGLE_UNIT_CONVERSION        0.021973f	    // 360/16384   //////////////////


/*----------------------------�Զ���ö�����Ϳ�ʼ--------------------------------*/
typedef enum motor_kt9025_command_e
{
	PID_RX_ID									 = 0x30, 
	PID_TX_RAM_ID							 = 0x31,  //�ϵ�ʧЧ
	PID_TX_ROM_ID							 = 0x32,  //�ϵ���Ч
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


/*----------------------------�Զ���ö�����ͽ���-------------------------------------*/











/*--------------------------RM���     �Զ���ṹ�����Ϳ�ʼ----------------------------------*/
typedef struct RM_motor_rx_info_t
{
	int16_t   angle;						//0~8191   ������ʼ����Ϣ
	int16_t 	speed;						//RPM      ����ٶ�
	int16_t 	current;					//         ʵ��ת��
	int16_t		temperature;			//��C       �¶�
	int16_t 	torque;						//         ����
	
	int16_t		angle_prev;				//         �ϴνǶ�
	int16_t		angle_offset;			//         �ǶȲ�
	int32_t		angle_sum;				//-2147683647~2147683647 �ϵ翪ʼ�����ڵĽǶȺ�

}RM_motor_rx_info_t;

typedef struct RM_motor_mec_info_t 
{
	int16_t       mec_mid;        //��е��ֵ
	
	motor_dir_e   dir;	          //����
	
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
	uint32_t   tx_id;   			//����id
	uint32_t   rx_id;   			//����id
	uint8_t    buff_p;  			//����/���գ�������λ��
	
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
	/* ���ò��� */
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	//�����Ч������ֵ
	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
	
}RM_motor_pid_set_t;

typedef struct RM_motor_pid_t 		//PID��λ
{
	RM_motor_pid_info_t info;
	RM_motor_pid_set_t  set;
	
}RM_motor_pid_t;


typedef struct RM_motor_pid_all_t 
{
	RM_motor_pid_t   speed;
	
	//�Ƕȴ���PID	
	RM_motor_pid_t   angle;	
	RM_motor_pid_t   angle_in;		
	
	//�ǶȺʹ���PID
	RM_motor_pid_t   position;
	RM_motor_pid_t   position_in;	
	
	RM_motor_pid_t   user_define;
	
}RM_motor_pid_all_t; 


/*--------------------------RM���      �Զ���ṹ�����ͽ���----------------------------------*/














/*-----------------------KT���         �Զ���ṹ�����Ϳ�ʼ---------------------------------*/

typedef struct KT_motor_pid_rx_info_t
{
	uint8_t angleKp;
	uint8_t angleKi;
	uint8_t speedKp;
	uint8_t speedKi;
	uint8_t iqKp;   //ת�ػ�
	uint8_t iqKi;
	
}KT_motor_pid_rx_info_t;

typedef struct KT_motor_pid_tx_info_t
{
	uint8_t angleKp;
	uint8_t angleKi;
	uint8_t speedKp;
	uint8_t speedKi;
	uint8_t iqKp;  //ת�ػ�
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
	int32_t 	accel;						 //���ٶ� 1dps/s
	
	uint16_t 	encoder;					 //������λ��             ��0~16383 * 4��
	uint16_t 	encoderRaw;				 //������ԭʼλ��         ��0~16383 * 4��
	uint16_t 	encoderOffset;		 //��������ƫ             ��0~16383 * 4��
	
	int8_t    temperature;       //�¶�    1��C/LSB
	uint16_t  voltage;					 //��ѹ    1V
  uint8_t		errorState;				 //����λ��Ч    ����λ0xx0��������0xx1�¶�����+��ѹ������1xx0���±���+��ѹ������1001���±���+��ѹ����
	
	int16_t   current;           //ת�ص���    ���ص�ֵ�޵�λ˵��   -2048 ~ 2048  ��-33~33A��
	int16_t   speed;             //���ת��    1dps/LSB
	
	int16_t 	current_A;				 //ABC�����������     1A/64LSB
	int16_t 	current_B;
	int16_t 	current_C;
		
	int16_t   powerControl;      //������ʣ�-1000~1000��  �޵�λ˵��
	
	int64_t   motorAngle;        //�����Ȧ�Ƕ�   0.01��/LSB  ��-2^63~2^63��    ˳ʱ�����ӣ���ʱ�����
	
	uint32_t  circleAngle;       //�����Ȧ�Ƕ�   0.01��/LSB  �� 0~36000 * ���ٱ�-1�� 
															 //�Ա��������Ϊ��ʼ�㣬˳ʱ�����ӣ��ٴε������ʱ��ֵ��Ϊ0
	


	int16_t   angle_add;         //-4096~4096
	
}KT_motor_rx_info_t;


typedef struct KT_motor_tx_info_t
{	
	int32_t 	accel;						 //���ٶ�     1dps/s
	
	uint16_t 	encoderOffset;		 //��������ƫ��0~16383*4��
	
	int16_t   powerControl;      //������� ��-1000~1000��     ������λ����Max Power����
	
	int16_t   iqControl;         //Ť�ص��� ��-2000~2000����Ӧ��ʵ��Ť�ص���-32A~32A�� ������λ������
	
	int32_t   speedControl;      //ʵ��ת��Ϊ0.01dps/LSB       ���ֵ����λ������
	
	int32_t   angle_sum_Control; //��Ȧ�Ƕȣ�  0.01degree/LSB     36000����360��      ���ֵ����λ������
	
	uint16_t  angle_sum_Control_maxSpeed; //��Ȧ�Ƕȵ�����ٶ�   1dps/LSB   ���ֵ����λ������
	
	uint16_t  angle_single_Control; //��Ȧ�Ƕ�    0.01degree/LSB       0~35999��Ӧʵ�ʽǶ�0~359.99��   ���ֵ����λ������
	
	uint16_t  angle_single_Control_maxSpeed; //��Ȧ�Ƕȵ�����ٶ�   1dps/LSB   ���ֵ����λ������
	
	uint8_t   angle_single_Control_spinDirection;     //��Ȧ�Ƕ���ת����0x00˳ʱ�룬0x01��ʱ��
	
	int32_t   angle_add_Control;     //�Ƕ�λ��������ת�������ɿ��������ž���   0.01degree/LSB     ���ֵ����λ������
	
	uint16_t  angle_add_Control_maxSpeed;  //�Ƕ�λ������������ٶ�  1dps/LSB   ���ֵ����λ������
	 
}KT_motor_tx_info_t;



typedef struct KT_motor_id_info_t
{
	uint32_t   tx_id;   			//����id
	uint32_t   rx_id;   			//����id
	
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


