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
 *	@brief	�����ʼ��������һЩ������ֵ������ָ�븳ֵ�����㷢�����顢���㷢�͵Ľṹ�����
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
 *	@brief	����������������ʧ������һ���յ�����ʱ��offline_cnt���ڵ�������н�������
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
 *	@brief	��ʼ�������PID���͡����սṹ��
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


/*--------------------------���������Ҫ����������������-------------------------*/


/** 
 *	@brief �������ƣ���Ҫ���ⲿ�����4�������Ť�ص�������ID�ŵ�˳�����һ������
					 �������ĵ����С��4�����Զ���鴫�������󲿷��Ƿ���0���������0��������Ϊ0
 */
void kt_motor_multi_control(int16_t* iqControl, char kt_motor_num, motor_drive_e drive_type)
{
	//�жϵ���
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









/*---------------------------���º���������Ե��������----------------------------*/


/** 
 *	@brief ����������й�  д����������Ҫ�Ŀ���ģʽ������ڲ��Ѿ���can�ķ��ͺ���
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
		case PID_TX_RAM_ID:  //дPID������RAM
			motor->tx_buff[0] = PID_TX_RAM_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = pid->tx.angleKp;
			motor->tx_buff[3] = pid->tx.angleKi;
			motor->tx_buff[4] = pid->tx.speedKp;
			motor->tx_buff[5] = pid->tx.speedKi;
			motor->tx_buff[6] = pid->tx.iqKp;
			motor->tx_buff[7] = pid->tx.iqKi;
		break;
		
		case PID_TX_ROM_ID:  //дPID������ROM
			motor->tx_buff[0] = PID_TX_RAM_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = pid->tx.angleKp;
			motor->tx_buff[3] = pid->tx.angleKi;
			motor->tx_buff[4] = pid->tx.speedKp;
			motor->tx_buff[5] = pid->tx.speedKi;
			motor->tx_buff[6] = pid->tx.iqKp;
			motor->tx_buff[7] = pid->tx.iqKi;
		break;
		
		case ACCEL_TX_ID:  //д���ٶȵ�RAM
			motor->tx_buff[0] = ACCEL_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->accel;
			motor->tx_buff[5] = (uint8_t) (tx_info->accel >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->accel >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->accel >> 24);
		break;
		
		case ZERO_ENCODER_TX_ID:  //д������ֵ��ROM��Ϊ������
			motor->tx_buff[0] = ZERO_ENCODER_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = 0x00;
			motor->tx_buff[5] = 0x00;
			motor->tx_buff[6] = (uint8_t) tx_info->encoderOffset;
			motor->tx_buff[7] = (uint8_t) (tx_info->encoderOffset >> 8);
		break;
		
		case ZERO_POSNOW_TX_ID:  //д��ǰλ��ֵ��ROM��Ϊ�����㣬����ʹ��
			motor->tx_buff[0] = ZERO_POSNOW_TX_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = 0x00;
			motor->tx_buff[5] = 0x00;
			motor->tx_buff[6] = (uint8_t) tx_info->encoderOffset;
			motor->tx_buff[7] = (uint8_t) (tx_info->encoderOffset >> 8);
		break;
		
		case MOTOR_CLOSE_ID:     //����ر�����������״̬��֮ǰ�յ���ָ��
			motor->tx_buff[0] = MOTOR_CLOSE_ID;
		break;
		
		case MOTOR_STOP_ID:     //���ֹͣ����������״̬��֮ǰ�յ���ָ��
			motor->tx_buff[0] = MOTOR_STOP_ID;
		break;
		
		case MOTOR_RUN_ID:      //������У���ֹͣ�лָ�
			motor->tx_buff[0] = MOTOR_RUN_ID;
		break;
		
		case TORQUE_OPEN_LOOP_ID:  //����ת�ؿ��ƣ������������
			motor->tx_buff[0] = TORQUE_OPEN_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->powerControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->powerControl >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case TORQUE_CLOSE_LOOP_ID:  //�ջ�ת�ؿ��ƣ�����Ť�ص���
			motor->tx_buff[0] = TORQUE_CLOSE_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->iqControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->iqControl >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case SPEED_CLOSE_LOOP_ID:    //�ٶȱջ�����
			motor->tx_buff[0] = SPEED_CLOSE_LOOP_ID;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->speedControl;
			motor->tx_buff[5] = (uint8_t) (tx_info->speedControl >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->speedControl >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->speedControl >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID1:    //�Ƕ��ܺͱջ����ٶȲ�����
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID1;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_sum_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_sum_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_sum_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_sum_Control >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID2:    //�Ƕ��ܺͱջ����ٶ�����
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID2;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = (uint8_t) tx_info->angle_sum_Control_maxSpeed;
			motor->tx_buff[3] = (uint8_t) (tx_info->angle_sum_Control_maxSpeed >> 8);
			motor->tx_buff[4] = (uint8_t) tx_info->angle_sum_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_sum_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_sum_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_sum_Control >> 24);
		break;
		
		case POSI_CLOSE_LOOP_ID3:    //��Ȧ�Ƕȣ��з���
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID3;
			motor->tx_buff[1] = (uint8_t) tx_info->angle_single_Control_spinDirection;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_single_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_single_Control >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case POSI_CLOSE_LOOP_ID4:    //��Ȧ�Ƕȣ��з��������ת��
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID4;
			motor->tx_buff[1] = (uint8_t) tx_info->angle_single_Control_spinDirection;
		  motor->tx_buff[2] = (uint8_t) tx_info->angle_single_Control_maxSpeed;
			motor->tx_buff[3] = (uint8_t) (tx_info->angle_single_Control_maxSpeed >> 8);
			motor->tx_buff[4] = (uint8_t) tx_info->angle_single_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_single_Control >> 8);
			motor->tx_buff[6] = 0x00;
			motor->tx_buff[7] = 0x00;
		break;
		
		case POSI_CLOSE_LOOP_ID5:    //�Ƕ����������ٶ�����
			motor->tx_buff[0] = POSI_CLOSE_LOOP_ID5;
			motor->tx_buff[1] = 0x00;
		  motor->tx_buff[2] = 0x00;
			motor->tx_buff[3] = 0x00;
			motor->tx_buff[4] = (uint8_t) tx_info->angle_add_Control;
			motor->tx_buff[5] = (uint8_t) (tx_info->angle_add_Control >> 8);
			motor->tx_buff[6] = (uint8_t) (tx_info->angle_add_Control >> 16);
			motor->tx_buff[7] = (uint8_t) (tx_info->angle_add_Control >> 24);
		break;
				
		case POSI_CLOSE_LOOP_ID6:    //�Ƕ����������ٶ�����
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
 *	@brief ���������������ȡĳЩ����������ڲ��Ѿ���can�ķ��ͺ���
 */
void tx_kt_motor_R_command(KT_motor_t *motor, uint8_t command)
{
	if( motor == NULL )
		return;
	
	memset( (uint8_t*)motor->tx_buff, 0, 8 );
	
	switch(command)
	{
		case PID_RX_ID:   	//��ȡ����PID�ṹ�����
			motor->tx_buff[0] = PID_RX_ID;
		break;
		
		case ACCEL_RX_ID:   //��ȡ���͵Ľṹ���еļ��ٶȲ���
			motor->tx_buff[0] = ACCEL_RX_ID;
		break;
		
		case ENCODER_RX_ID:   //��ȡ���ͽṹ���еı���������
			motor->tx_buff[0] = ENCODER_RX_ID;
		break;
		
		case MOTOR_ANGLE_ID:  //��ȡ�����Ȧ���ԽǶ�
		 motor->tx_buff[0] = MOTOR_ANGLE_ID;
		break;
		
		case CIRCLE_ANGLE_ID:  //��ȡ�����Ȧ�Ƕ�
			motor->tx_buff[0] = CIRCLE_ANGLE_ID;
		break;
		
		case STATE1_ID:        //��ȡ���״̬1�ʹ����־λ
			motor->tx_buff[0] = STATE1_ID;
		break;
		
		case STATE2_ID:        //��ȡ���״̬2
			motor->tx_buff[0] = STATE2_ID;
		break;
		
		case STATE3_ID:        //��ȡ���״̬3
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
 *	@brief	���յ����������Ϣ���Զ����£���Ҫע�⣬�󲿷ַ��͸������ָ����Ҳ�᷵��һЩ����
						��������ָ�룬��Ϊ������ݳ���������
						��������ݽ��սṹ���errorState�ж��Ƿ�Ҫ���ұ���
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
		case PID_RX_ID:                      //������ȡPID
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case PID_TX_RAM_ID:                  //����PID������RAMʱ�᷵��
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case PID_TX_ROM_ID:                  //����PID������ROMʱ�᷵��
			pid_rx_info->angleKp = rxBuf[2];
			pid_rx_info->angleKi = rxBuf[3];
			pid_rx_info->speedKp = rxBuf[4];
			pid_rx_info->speedKi = rxBuf[5];
			pid_rx_info->iqKp	   = rxBuf[6];
			pid_rx_info->iqKi	   = rxBuf[7];
		break;
		
		case ACCEL_RX_ID:                    //������ȡ���ٶ�
			rx_info->accel  = (int32_t)rxBuf[7];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[6];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[5];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[4];
			rx_info->accel <<= 8;
		break;
		
		case ACCEL_TX_ID:                    //���ͼ��ٶȲ�����RAM�᷵��
			rx_info->accel  = (int32_t)rxBuf[7];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[6];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[5];
			rx_info->accel <<= 8;
			rx_info->accel |= (int32_t)rxBuf[4];
			rx_info->accel <<= 8;
		break;	
		
		case ENCODER_RX_ID:                   //������ȡ������
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
		
		case ZERO_ENCODER_TX_ID:              //д�������ֵ��ROM��Ϊ������᷵��
			rx_info->encoderOffset = (uint16_t)rxBuf[7];
			rx_info->encoderOffset <<= 8;
			rx_info->encoderOffset |= (uint16_t)rxBuf[6];
		break;
		
		case ZERO_POSNOW_TX_ID:              //д�������ֵ��RAM��Ϊ������᷵��
			rx_info->encoderOffset = (uint16_t)rxBuf[7];
			rx_info->encoderOffset <<= 8;
			rx_info->encoderOffset |= (uint16_t)rxBuf[6];
		break;
		
		
		case MOTOR_ANGLE_ID:                  //������ȡ�����Ȧ���ԽǶȣ���ֵ˳ʱ���ۼƽǶ�
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
		
		case CIRCLE_ANGLE_ID:                 //������ȡ�����Ȧ�Ƕ�
			rx_info->circleAngle  = (uint32_t)rxBuf[7];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[6];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[5];
			rx_info->circleAngle <<= 8;
			rx_info->circleAngle |= (uint32_t)rxBuf[4];
		break;
		
		case STATE1_ID:                       //������ȡ���״̬1�ʹ����־λ
			rx_info->temperature = (int8_t)rxBuf[1]; 
			rx_info->voltage = (uint16_t)rxBuf[4];
			rx_info->voltage <<= 8;
			rx_info->voltage |= (uint16_t)rxBuf[3];
			rx_info->errorState = rxBuf[7];
		break;
		
		case STATE2_ID:                       //������ȡ���״̬2
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
		
		case STATE3_ID:                        //������ȡ���״̬3
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
		
		case TORQUE_OPEN_LOOP_ID:              //Ť�ؿ������ƻ��Զ�����
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
		
		case  TORQUE_CLOSE_LOOP_ID:	//Ť�رջ����ơ��ٶȱջ�������λ�ñջ����᷵��
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
 *	@briefд����ķ���PID�ṹ���������������ṹ{angleKp��angleKi��speedKp��speedKi��iqKp��iqKi��0��0}
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
 *	@briefд����ķ��ͽṹ��ļ��ٶȲ���
 */

void write_kt_motor_accel_param(KT_motor_t *motor, int32_t accel)
{
	if(motor == NULL)
		return;
	
	motor->KT_motor_info.tx_info.accel = accel;
}


/** 
 *	@brief д����ķ��ͽṹ��ĵ��������
 */

void write_kt_motor_encoderOffset_param(KT_motor_t *motor, uint16_t encoderOffset)
{
	if(motor == NULL)
		return;
	
	//�޷������ݵ���within_or_not�ᱨ����
	if( encoderOffset > KT_TX_ENCODER_OFFSET_MAX )
	  return;
	
	motor->KT_motor_info.tx_info.encoderOffset = encoderOffset;
}

/** 
 *	@brief д����ķ��ͽṹ���������ʿ���Ŀ�����
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
 *	@brief д����ķ��ͽṹ���Ť�ص�������Ŀ�����
 */


void write_kt_motor_iqControl_param(KT_motor_t *motor, int16_t iqControl)
{
	if(motor == NULL)
		return;
	
	iqControl = constrain(iqControl, -KT_TX_IQ_CONTROL_MAX, KT_TX_IQ_CONTROL_MAX);
	
	motor->KT_motor_info.tx_info.iqControl = iqControl;
}


/** 
 *	@brief д����ķ��ͽṹ����ٶȿ���Ŀ�����
 */

void write_kt_motor_speedControl_param(KT_motor_t *motor, int32_t speedControl)
{
	if(motor == NULL)
		return;
		
	motor->KT_motor_info.tx_info.speedControl = speedControl;
}

/** 
 *	@brief д����ķ��ͽṹ��Ķ�Ȧ�Ƕȿ����漰���Ĳ���
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
 *	@brief д����ķ��ͽṹ��ĵ�Ȧ�Ƕȿ����漰���Ĳ���
 */

void write_kt_motor_angle_single_Control_param(KT_motor_t   *motor, 
																							 uint16_t     angle_single_Control,
																							 uint8_t   	  angle_single_Control_spinDirection,
																							 uint16_t			angle_single_Control_maxSpeed)
{
	if(motor == NULL)
		return;
	
	//�޷��ŵ���within_or_not�ᱨ����
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
 *	@brief д����ķ��ͽṹ��ĽǶ����������漰���Ĳ���
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


