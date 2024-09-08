#include "KT_motor.h"

/* Exported variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


void KT_motor_class_heartbeat(KT_motor_t *motor);
void kt_motor_class_pid_init(KT_motor_t *motor);


void get_kt_motor_info(KT_motor_t *motor, uint8_t *rxBuf);
void tx_kt_motor_W_command(KT_motor_t *motor, uint8_t command);
void tx_kt_motor_R_command(KT_motor_t *motor, uint8_t command);



void write_kt_motor_pid_param(KT_motor_t *motor, uint8_t* buff);
void write_kt_motor_accel_param(KT_motor_t *motor, int32_t accel);
void write_kt_motor_encoderOffset_param(KT_motor_t *motor, uint16_t encoderOffset);
void write_kt_motor_powerControl_param(KT_motor_t *motor, int16_t powerControl);
void write_kt_motor_iqControl_param(KT_motor_t *motor, int16_t iqControl);
void write_kt_motor_speedControl_param(KT_motor_t *motor, int32_t speedControl);
void write_kt_motor_angle_sum_Control_param(KT_motor_t    *motor, 
																						int32_t       angle_sum_Control,
	                                          uint16_t      angle_sum_Control_maxSpeed);
void write_kt_motor_angle_single_Control_param(KT_motor_t   *motor, 
																							 uint16_t     angle_single_Control,
																							 uint8_t   	  angle_single_Control_spinDirection,
																							 uint16_t			angle_single_Control_maxSpeed);
void write_kt_motor_angle_add_Control_param(KT_motor_t   *motor, 
																						int32_t      angle_add_Control,
																			      uint16_t     angle_add_Control_maxSpeed);
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/













/**
 *	@brief	电机初始化，负责一些参数赋值、函数指针赋值、清零发送数组、清零发送的结构体参数
 */
void KT_motor_class_init(KT_motor_t *motor)
{
	
	if(motor == NULL)
		return;
	
	memset ( (uint8_t*)motor->tx_buff, 0, 8 );
	memset ( &motor->KT_motor_info.tx_info, 0, 8);
	
	motor->KT_motor_info.state_info.init_flag         = M_INIT;
	motor->KT_motor_info.state_info.offline_cnt_max   = OFFLINE_LINE_CNT_MAX;		
	motor->KT_motor_info.state_info.offline_cnt       = 0;
	motor->KT_motor_info.state_info.work_state        = M_OFFLINE;	
	motor->KT_motor_info.state_info.selfprotect_flag  = M_PROTECT_OFF;
	
	motor->heartbeat  = KT_motor_class_heartbeat;
	motor->pid_init   = kt_motor_class_pid_init;
	
	motor->get_info = get_kt_motor_info;
	motor->tx_W_cmd = tx_kt_motor_W_command;
	motor->tx_R_cmd = tx_kt_motor_R_command;
	
	motor->W_pid                  = write_kt_motor_pid_param;
	motor->W_accel                = write_kt_motor_accel_param;
	motor->W_encoderOffset        = write_kt_motor_encoderOffset_param;
	motor->W_powerControl         = write_kt_motor_powerControl_param;
	motor->W_iqControl            = write_kt_motor_iqControl_param;
	motor->W_speedControl         = write_kt_motor_speedControl_param;
	motor->W_angle_sum_Control    = write_kt_motor_angle_sum_Control_param;
	motor->W_angle_single_Control = write_kt_motor_angle_single_Control_param;
	motor->W_angle_add_Control    = write_kt_motor_angle_add_Control_param;
	
}

/**
 *	@brief	电机心跳，如果发生失联，下一次收到数据时，offline_cnt会在电机更新中进行清零
 */
void KT_motor_class_heartbeat(KT_motor_t *motor)
{	
	
	if(motor == NULL)	
		return;
			
	KT_motor_state_info_t *state_info = &motor->KT_motor_info.state_info;

	if(state_info->init_flag == M_DEINIT)
	{
		state_info->work_state = M_INIT_ERR;
		return;
	}
		
	state_info->offline_cnt++;
	
	
	if(state_info->offline_cnt > state_info->offline_cnt_max) 
	{
		state_info->offline_cnt = state_info->offline_cnt_max;
		state_info->work_state = M_OFFLINE;
	}
	else 
	{
		if(state_info->work_state == M_OFFLINE)
			state_info->work_state = M_ONLINE;
	}
}




/**
 *	@brief	初始化电机的PID发送、接收结构体
 */
void kt_motor_class_pid_init(KT_motor_t *motor)
{
	if(motor == NULL)	
		return;
	
	KT_motor_pid_t *pid = &motor->KT_motor_info.pid_info;
	
	pid->init_flag     = M_INIT;
	
	pid->rx.angleKp    = 0;
	pid->rx.angleKi    = 0;
	pid->rx.speedKp    = 0;
	pid->rx.speedKi    = 0;
	pid->rx.iqKp       = 0;
	pid->rx.iqKi       = 0;

	
	pid->tx.angleKp    = 0;
	pid->tx.angleKi    = 0;
	pid->tx.speedKp    = 0;
	pid->tx.speedKi    = 0;
	pid->tx.iqKp       = 0;
	pid->tx.iqKi       = 0;

}	


/*--------------------------多电机命令不需要再数组中填入命令-------------------------*/


/** 
 *	@brief 多电机控制，需要在外部把最多4个电机的扭矩电流按照ID号的顺序组成一个数组
					 如果传入的电机数小于4，会自动检查传入的数组后部分是否是0，如果不是0，会设置为0
 */
void kt_motor_multi_control(int16_t* iqControl, char kt_motor_num, motor_drive_e drive_type)
{
	//判断电流
	for(int i = 0; i < kt_motor_num; i ++)
	{
		if( within_or_not(iqControl[i], -KT_TX_IQ_CONTROL_MAX, KT_TX_IQ_CONTROL_MAX) == Flase )
			return;
	}
	
	for(int i = kt_motor_num; i < 4 ; i ++)
	{
		if( iqControl[i] != 0 )
			iqControl[i] = 0;
	}
	
	uint8_t tx_buff[8] = {0};
	
	tx_buff[0] = (uint8_t) iqControl[0];
	tx_buff[1] = (uint8_t) iqControl[0] >> 8;
	tx_buff[2] = (uint8_t) iqControl[1];
	tx_buff[3] = (uint8_t) iqControl[1] >> 8;
	tx_buff[4] = (uint8_t) iqControl[2];
	tx_buff[5] = (uint8_t) iqControl[2] >> 8;
	tx_buff[6] = (uint8_t) iqControl[3];
	tx_buff[7] = (uint8_t) iqControl[3] >> 8;
	
	if(drive_type == M_CAN1)
	{
		CAN_SendData(&hcan1, KT_MULTI_TX_ID, tx_buff);
	}
	else if(drive_type == M_CAN2)
	{
		CAN_SendData(&hcan2, KT_MULTI_TX_ID, tx_buff);
	}
	else
		return;
	
}









/*---------------------------以下函数都是针对单电机控制----------------------------*/


/** 
 *	@brief 给电机发送有关  写参数或者想要的控制模式的命令，内部已经有can的发送函数
 */
void tx_kt_motor_W_command(KT_motor_t *motor, uint8_t command)
{
	
	if( motor == NULL )
		return;
	
	KT_motor_pid_t       *pid      = &motor->KT_motor_info.pid_info;
	
	KT_motor_tx_info_t   *tx_info  = &motor->KT_motor_info.tx_info;
	
	memset( (uint8_t*)motor->tx_buff, 0, 8 );
	
	switch(command)
	{
		case PID_TX_RAM_ID:  //写PID参数到RAM
			motor->tx_buff[0] = PID_TX_RAM_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = pid->tx.angleKp;
			motor->tx_buff[3] = pid->tx.angleKi;
			motor->tx_buff[4] = pid->tx.speedKp;
			motor->tx_buff[5] = pid->tx.speedKi;
			motor->tx_buff[6] = pid->tx.iqKp;
			motor->tx_buff[7] = pid->tx.iqKi;
		break;
		
		case PID_TX_ROM_ID:  //写PID参数到ROM
			motor->tx_buff[0] = PID_TX_RAM_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = pid->tx.angleKp;
			motor->tx_buff[3] = pid->tx.angleKi;
			motor->tx_buff[4] = pid->tx.speedKp;
			motor->tx_buff[5] = pid->tx.speedKi;
			motor->tx_buff[6] = pid->tx.iqKp;
			motor->tx_buff[7] = pid->tx.iqKi;
		break;
		
		case ACCEL_TX_ID:  //写加速度到RAM
			motor->tx_buff[0] = ACCEL_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->accel;
			motor->tx_buff[5] = (uint8_t) (tx_info->accel >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->accel >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->accel >> 24);
		break;
		
		case ZERO_ENCODER_TX_ID:  //写编码器值到ROM作为电机零点
			motor->tx_buff[0] = ZERO_ENCODER_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = 0x00;
			motor->tx_buff[5] = 0x00;
			motor->tx_buff[6] = (uint8_t) tx_info->encoderOffset;
			motor->tx_buff[7] = (uint8_t) (tx_info->encoderOffset >> 8);
		break;
		
		case ZERO_POSNOW_TX_ID:  //写当前位置值到ROM作为电机零点，减少使用
			motor->tx_buff[0] = ZERO_POSNOW_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = 0x00;
			motor->tx_buff[5] = 0x00;
			motor->tx_buff[6] = (uint8_t) tx_info->encoderOffset;
			motor->tx_buff[7] = (uint8_t) (tx_info->encoderOffset >> 8);
		break;
		
		case MOTOR_CLOSE_ID:     //电机关闭命令，清除运行状态和之前收到的指令
			motor->tx_buff[0] = MOTOR_CLOSE_ID;
		break;
		
		case MOTOR_STOP_ID:     //电机停止命令，清除运行状态和之前收到的指令
			motor->tx_buff[0] = MOTOR_STOP_ID;
		break;
		
		case MOTOR_RUN_ID:      //电机运行，从停止中恢复
			motor->tx_buff[0] = MOTOR_RUN_ID;
		break;
		
		case TORQUE_OPEN_LOOP_ID:  //开环转矩控制，控制输出功率
			motor->tx_buff[0] = TORQUE_OPEN_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->powerControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->powerControl >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case TORQUE_CLOSE_LOOP_ID:  //闭环转矩控制，控制扭矩电流
			motor->tx_buff[0] = TORQUE_CLOSE_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->iqControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->iqControl >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case SPEED_CLOSE_LOOP_ID:    //速度闭环控制
			motor->tx_buff[0] = SPEED_CLOSE_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->speedControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->speedControl >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->speedControl >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->speedControl >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID1:    //角度总和闭环，速度不限制
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID1;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_sum_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_sum_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_sum_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_sum_Control >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID2:    //角度总和闭环，速度限制
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID2;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = (uint8_t) tx_info->angle_sum_Control_maxSpeed;
			motor->tx_buff[3] = (uint8_t) (tx_info->angle_sum_Control_maxSpeed >> 8);
			motor->tx_buff[4] = (uint8_t) tx_info->angle_sum_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_sum_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_sum_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_sum_Control >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID3:    //单圈角度，有方向
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID3;
			motor->tx_buff[1] = (uint8_t) tx_info->angle_single_Control_spinDirection;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_single_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_single_Control >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case POSI_CLOSE_LOOP_ID4:    //单圈角度，有方向，有最高转速
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID4;
			motor->tx_buff[1] = (uint8_t) tx_info->angle_single_Control_spinDirection;
		  motor->tx_buff[2] = (uint8_t) tx_info->angle_single_Control_maxSpeed;
			motor->tx_buff[3] = (uint8_t) (tx_info->angle_single_Control_maxSpeed >> 8);
			motor->tx_buff[4] = (uint8_t) tx_info->angle_single_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_single_Control >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case POSI_CLOSE_LOOP_ID5:    //角度增量，无速度限制
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID5;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_add_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_add_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_add_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_add_Control >> 24);
		break;
				
		case POSI_CLOSE_LOOP_ID6:    //角度增量，有速度限制
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID6;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = (uint8_t) tx_info->angle_add_Control_maxSpeed;
			motor->tx_buff[3] = (uint8_t) (tx_info->angle_add_Control_maxSpeed >> 8);
			motor->tx_buff[4] = (uint8_t) tx_info->angle_add_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_add_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_add_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_add_Control >> 24);
		break;
		
		
		default:
			break;
	}
	
	if(motor->KT_motor_info.id.drive_type == M_CAN1)
	{
		CAN_SendData(&hcan1, motor->KT_motor_info.id.tx_id, motor->tx_buff);
	}
	else if(motor->KT_motor_info.id.drive_type == M_CAN2)
	{
		CAN_SendData(&hcan2, motor->KT_motor_info.id.tx_id, motor->tx_buff);
	}
	else
		return;
}
	



/** 
 *	@brief 给电机发送主动读取某些参数的命令，内部已经有can的发送函数
 */
void tx_kt_motor_R_command(KT_motor_t *motor, uint8_t command)
{
	if( motor == NULL )
		return;
	
	memset( (uint8_t*)motor->tx_buff, 0, 8 );
	
	switch(command)
	{
		case PID_RX_ID:   	//读取发送PID结构体参数
			motor->tx_buff[0] = PID_RX_ID;
		break;
		
		case ACCEL_RX_ID:   //读取发送的结构体中的加速度参数
			motor->tx_buff[0] = ACCEL_RX_ID;
		break;
		
		case ENCODER_RX_ID:   //读取发送结构体中的编码器数据
			motor->tx_buff[0] = ENCODER_RX_ID;
		break;
		
		case MOTOR_ANGLE_ID:  //读取电机多圈绝对角度
		 motor->tx_buff[0] = MOTOR_ANGLE_ID;
		break;
		
		case CIRCLE_ANGLE_ID:  //读取电机单圈角度
			motor->tx_buff[0] = CIRCLE_ANGLE_ID;
		break;
		
		case STATE1_ID:        //读取电机状态1和错误标志位
			motor->tx_buff[0] = STATE1_ID;
		break;
		
		case STATE2_ID:        //读取电机状态2
			motor->tx_buff[0] = STATE2_ID;
		break;
		
		case STATE3_ID:        //读取电机状态3
			motor->tx_buff[0] = STATE3_ID;
		break;
		
		default:
			break;
	}
	
	if(motor->KT_motor_info.id.drive_type == M_CAN1)
	{
		CAN_SendData(&hcan1, motor->KT_motor_info.id.tx_id, motor->tx_buff);
	}
	else if(motor->KT_motor_info.id.drive_type == M_CAN2)
	{
		CAN_SendData(&hcan2, motor->KT_motor_info.id.tx_id, motor->tx_buff);
	}
	else
		return;
}



/**
 *	@brief	接收电机发来的信息并自动更新，需要注意，大部分发送给电机的指令，电机也会返回一些数据
						如果传入空指针，认为电机数据出错，并返回
						最后面会根据接收结构体的errorState判断是否要自我保护
 *  @return
 */
void get_kt_motor_info(KT_motor_t *motor, uint8_t *rxBuf)
{
	if( motor == NULL || rxBuf == NULL )
	{
		motor->KT_motor_info.state_info.work_state = M_DATA_ERR;
		return;
	}	
	
	uint8_t ID = rxBuf[0];
	
	KT_motor_pid_rx_info_t *pid_rx_info = &motor->KT_motor_info.pid_info.rx;
	KT_motor_rx_info_t     *rx_info     = &motor->KT_motor_info.rx_info;
	KT_motor_state_info_t  *state_info  = &motor->KT_motor_info.state_info;
	
	state_info->offline_cnt = 0;
	state_info->work_state = M_ONLINE;
	
	switch (ID)
	{
		case PID_RX_ID:                      //主动读取PID
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case PID_TX_RAM_ID:                  //发送PID参数到RAM时会返回
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case PID_TX_ROM_ID:                  //发送PID参数到ROM时会返回
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case ACCEL_RX_ID:                    //主动读取加速度
			rx_info->accel  = (int32_t)rxBuf[7];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[6];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[5];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[4];
			rx_info->accel <<= 8;
		break;
		
		case ACCEL_TX_ID:                    //发送加速度参数到RAM会返回
			rx_info->accel  = (int32_t)rxBuf[7];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[6];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[5];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[4];
			rx_info->accel <<= 8;
		break;	
		
		case ENCODER_RX_ID:                   //主动读取编码器
			rx_info->encoder = (uint16_t)rxBuf[3];
			rx_info->encoder <<= 8;
			rx_info->encoder |= (uint16_t)rxBuf[2];
			rx_info->encoderRaw = (uint16_t)rxBuf[5];
			rx_info->encoderRaw <<= 8;
			rx_info->encoderRaw |= (uint16_t)rxBuf[4];
			rx_info->encoderOffset = (uint16_t)rxBuf[7];
			rx_info->encoderOffset <<= 8;
			rx_info->encoderOffset |= (uint16_t)rxBuf[6];
		break;
		
		case ZERO_ENCODER_TX_ID:              //写入编码器值到ROM作为电机零点会返回
			rx_info->encoderOffset = (uint16_t)rxBuf[7];
			rx_info->encoderOffset <<= 8;
			rx_info->encoderOffset |= (uint16_t)rxBuf[6];
		break;
		
		case ZERO_POSNOW_TX_ID:              //写入编码器值到RAM作为电机零点会返回
			rx_info->encoderOffset = (uint16_t)rxBuf[7];
			rx_info->encoderOffset <<= 8;
			rx_info->encoderOffset |= (uint16_t)rxBuf[6];
		break;
		
		
		case MOTOR_ANGLE_ID:                  //主动读取电机多圈绝对角度，正值顺时针累计角度
			rx_info->motorAngle  = (int64_t)rxBuf[7];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[6];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[5];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[4];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[3];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[2];
			rx_info->motorAngle <<= 8;
			rx_info->motorAngle |= (int64_t)rxBuf[1];
		break;
		
		case CIRCLE_ANGLE_ID:                 //主动读取电机单圈角度
			rx_info->circleAngle  = (uint32_t)rxBuf[7];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[6];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[5];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[4];
		break;
		
		case STATE1_ID:                       //主动读取电机状态1和错误标志位
			rx_info->temperature = (int8_t)rxBuf[1]; 
			rx_info->voltage = (uint16_t)rxBuf[4];
			rx_info->voltage <<= 8;
			rx_info->voltage |= (uint16_t)rxBuf[3];
			rx_info->errorState = rxBuf[7];
		break;
		
		case STATE2_ID:                       //主动读取电机状态2
			rx_info->temperature = (int8_t)rxBuf[1];
			rx_info->current = (int16_t)rxBuf[3];
			rx_info->current <<= 8;
			rx_info->current |= (int16_t)rxBuf[2];
			rx_info->speed = (int16_t)rxBuf[5];
			rx_info->speed <<= 8;
			rx_info->speed |= (int16_t)rxBuf[4];
			rx_info->encoder = (uint16_t)rxBuf[7];
			rx_info->encoder <<= 8;
			rx_info->encoder |= (uint16_t)rxBuf[6];
		break;
		
		case STATE3_ID:                        //主动读取电机状态3
			rx_info->temperature = (int8_t)rxBuf[1];
			rx_info->current_A = (int16_t)rxBuf[3];
			rx_info->current_A <<= 8;
			rx_info->current_A |= (int16_t)rxBuf[2];
			rx_info->current_B = (int16_t)rxBuf[5];
			rx_info->current_B <<= 8;
			rx_info->current_B |= (int16_t)rxBuf[4];
			rx_info->current_C = (int16_t)rxBuf[7];
			rx_info->current_C <<= 8;
			rx_info->current_C |= (int16_t)rxBuf[6];
		break;
		
		case TORQUE_OPEN_LOOP_ID:              //扭矩开环控制会自动返回
			rx_info->temperature = (int8_t)rxBuf[1]; 
			rx_info->powerControl = (int16_t)rxBuf[3];
			rx_info->powerControl <<= 8;	
			rx_info->powerControl |= (int16_t)rxBuf[2];
			rx_info->speed = (int16_t)rxBuf[5];
			rx_info->speed <<= 8;
			rx_info->speed |= (int16_t)rxBuf[4];
			rx_info->encoder = (uint16_t)rxBuf[7];
			rx_info->encoder <<= 8;
			rx_info->encoder |= (uint16_t)rxBuf[6];
		break;
		
		case  TORQUE_CLOSE_LOOP_ID:	//扭矩闭环控制、速度闭环、所有位置闭环都会返回
		case  SPEED_CLOSE_LOOP_ID :
		case  POSI_CLOSE_LOOP_ID1 :
		case  POSI_CLOSE_LOOP_ID2 :
		case  POSI_CLOSE_LOOP_ID3 :
		case  POSI_CLOSE_LOOP_ID4 :	
		case  POSI_CLOSE_LOOP_ID5 :
		case  POSI_CLOSE_LOOP_ID6 :
			rx_info->temperature = (int8_t)rxBuf[1]; 
			rx_info->current = (int16_t)rxBuf[3];
			rx_info->current <<= 8;	
			rx_info->current |= (int16_t)rxBuf[2];
			rx_info->speed = (int16_t)rxBuf[5];
			rx_info->speed <<= 8;
			rx_info->speed |= (int16_t)rxBuf[4];
			rx_info->encoder = (uint16_t)rxBuf[7];
			rx_info->encoder <<= 8;
			rx_info->encoder |= (uint16_t)rxBuf[6];
		break;
		
		
		default:
			break;
	}

	
}













/**
 *	@brief写电机的发送PID结构体参数，整型数组结构{angleKp，angleKi，speedKp，speedKi，iqKp，iqKi，0，0}
 */
void write_kt_motor_pid_param(KT_motor_t *motor, uint8_t* buff)
{
	if(motor == NULL || buff == NULL)
		return;
	
	KT_motor_pid_t *pid = &motor->KT_motor_info.pid_info;
	
	pid->tx.angleKp    = buff[0];
	pid->tx.angleKi    = buff[1];
	pid->tx.speedKp    = buff[2];
	pid->tx.speedKi    = buff[3];
	pid->tx.iqKp       = buff[4];
	pid->tx.iqKi       = buff[5];
}


/**
 *	@brief写电机的发送结构体的加速度参数
 */

void write_kt_motor_accel_param(KT_motor_t *motor, int32_t accel)
{
	if(motor == NULL)
		return;
	
	motor->KT_motor_info.tx_info.accel = accel;
}


/** 
 *	@brief 写电机的发送结构体的电机零点参数
 */

void write_kt_motor_encoderOffset_param(KT_motor_t *motor, uint16_t encoderOffset)
{
	if(motor == NULL)
		return;
	
	//无符号数据调用within_or_not会报警告
	if( encoderOffset > KT_TX_ENCODER_OFFSET_MAX )
	  return;
	
	motor->KT_motor_info.tx_info.encoderOffset = encoderOffset;
}

/** 
 *	@brief 写电机的发送结构体的输出功率控制目标参数
 */


void write_kt_motor_powerControl_param(KT_motor_t *motor, int16_t powerControl)
{
	if(motor == NULL)
		return;
	
	if( within_or_not(powerControl, -KT_TX_POWER_CONTROL_MAX, KT_TX_POWER_CONTROL_MAX) == Flase )
	  return;
	
	motor->KT_motor_info.tx_info.powerControl = powerControl;
}

/** 
 *	@brief 写电机的发送结构体的扭矩电流控制目标参数
 */


void write_kt_motor_iqControl_param(KT_motor_t *motor, int16_t iqControl)
{
	if(motor == NULL)
		return;
	
	iqControl = constrain(iqControl, -KT_TX_IQ_CONTROL_MAX, KT_TX_IQ_CONTROL_MAX);
	
	motor->KT_motor_info.tx_info.iqControl = iqControl;
}


/** 
 *	@brief 写电机的发送结构体的速度控制目标参数
 */

void write_kt_motor_speedControl_param(KT_motor_t *motor, int32_t speedControl)
{
	if(motor == NULL)
		return;
		
	motor->KT_motor_info.tx_info.speedControl = speedControl;
}

/** 
 *	@brief 写电机的发送结构体的多圈角度控制涉及到的参数
 */


void write_kt_motor_angle_sum_Control_param(KT_motor_t    *motor, 
																						int32_t       angle_sum_Control,
	                                          uint16_t      angle_sum_Control_maxSpeed)
{
	if(motor == NULL)
		return;
		
	motor->KT_motor_info.tx_info.angle_sum_Control = angle_sum_Control;
	
	motor->KT_motor_info.tx_info.angle_sum_Control_maxSpeed = angle_sum_Control_maxSpeed;
}


/** 
 *	@brief 写电机的发送结构体的单圈角度控制涉及到的参数
 */

void write_kt_motor_angle_single_Control_param(KT_motor_t   *motor, 
																							 uint16_t     angle_single_Control,
																							 uint8_t   	  angle_single_Control_spinDirection,
																							 uint16_t			angle_single_Control_maxSpeed)
{
	if(motor == NULL)
		return;
	
	//无符号调用within_or_not会报警告
	if( angle_single_Control > KT_TX_ANGLE_SIGNLE_MAX )
		return;
	
	if( angle_single_Control_spinDirection != CLOCK_WISE ||
	   	angle_single_Control_spinDirection != N_CLOCK_WISE )
		return;
	
	
	motor->KT_motor_info.tx_info.angle_single_Control = angle_single_Control;
	
	motor->KT_motor_info.tx_info.angle_single_Control_spinDirection = angle_single_Control_spinDirection;
	
	motor->KT_motor_info.tx_info.angle_single_Control_maxSpeed = angle_single_Control_maxSpeed;
}

/** 
 *	@brief 写电机的发送结构体的角度增量控制涉及到的参数
 */

void write_kt_motor_angle_add_Control_param(KT_motor_t   *motor, 
																						int32_t      angle_add_Control,
																			      uint16_t     angle_add_Control_maxSpeed)
{
	if(motor == NULL)
		return;
	
	motor->KT_motor_info.tx_info.angle_add_Control = angle_add_Control;
	
	motor->KT_motor_info.tx_info.angle_add_Control_maxSpeed = angle_add_Control_maxSpeed;
	
}


