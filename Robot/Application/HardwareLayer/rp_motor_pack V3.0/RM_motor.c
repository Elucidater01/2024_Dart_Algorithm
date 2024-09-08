#include "RM_motor.h"
/*
*	RP MOTOR_BAG
*	2022.11.6 
* AUTHOR CCB HXZP
*
*ʹ�õ��ǰ�������úõ�������͡�id�š�������ʽ,�Լ���ʼ������ָ��motor_class_init
*ʹ��pidǰҲ��Ҫ����pid��ʼ���������޷�ʹ��
*����ֱ��ʹ�����ú�������pid���� Ҳ����ֱ��ʹ�ÿ��ڶ��庯�������Լ�����ĺ���
*ktϵ�е����ʹ��ktϵ�е��ǰ��Ҫ��ʵ����motor_9025_info_t������motor�ڵ�ָ����ϵ�����ಽ������
*/


/* Exported variables --------------------------------------------------------*/
void motor_class_init(struct RM_motor_class_t *motor);
void motor_class_heartbeat(struct RM_motor_class_t *motor);
void motor_class_pid_init(struct RM_motor_pid_t *pid, float *buff);
void can_tx_buff(struct RM_motor_class_t *motor, int16_t *buff,uint8_t len);
void can_rx_buff(struct RM_motor_class_t *motor, uint8_t *buff);
float motor_pid_single(RM_motor_pid_t *out, float meas1, float tar);
float motor_pid_angle(struct RM_motor_class_t *motor,float target);
float motor_pid_speed(struct RM_motor_class_t *motor,float target);
float motor_pid_double(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode);
float motor_pid_single(RM_motor_pid_t *out, float meas1, float tar);
void  motor_judge_dir(struct RM_motor_class_t *motor,uint16_t range);
void  motor_offset(struct RM_motor_class_t *motor, uint16_t range);
uint8_t motor_class_stucking_flag(struct RM_motor_class_t *motor, float torque_limit);
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/




/**
 *	@brief	�����ʼ��
 */
void motor_class_init(struct RM_motor_class_t *motor)
{
	if(motor == NULL)
	{
		return;
	}

	motor->state.init_flag = M_INIT;	
	motor->state.work_state = M_OFFLINE;

	motor->state.offline_cnt = 0;
	motor->state.offline_cnt_max = OFFLINE_LINE_CNT_MAX;	
	

	if(motor->id.drive_type == M_CAN1 || motor->id.drive_type == M_CAN2)
	{
		get_rm_can_drvie(motor);
	}
	else if(motor->id.drive_type == M_USART1 || motor->id.drive_type == M_USART2)
	{

	}	
	
	motor->heartbeat    = motor_class_heartbeat;	
	motor->pid_init     = motor_class_pid_init;
	
	
	motor->tx           = can_tx_buff;
	motor->rx           = can_rx_buff;
	
	motor->ctr_posit    = motor_pid_position;	
	motor->ctr_angle    = motor_pid_angle;
	motor->ctr_speed    = motor_pid_speed;
	motor->ctr_pid2     = motor_pid_double;		
	motor->ctr_pid1     = motor_pid_single;
	
	motor->ctr_judge_dir  = motor_judge_dir;
	motor->ctr_offset     = motor_offset;
	
	motor->ctr_stuck_flag = motor_class_stucking_flag;
	
}

/**
 *	@brief	�������
 */
void motor_class_heartbeat(struct RM_motor_class_t *motor)
{
	if(motor == NULL)
	{
		return;
	}

	if(motor->state.init_flag == M_DEINIT)
	{
		return;	
	}
	
	motor->state.offline_cnt++;
	
	if(motor->state.offline_cnt > motor->state.offline_cnt_max) 
	{
		motor->state.offline_cnt = motor->state.offline_cnt_max;
		motor->state.work_state = M_OFFLINE;
	}
	else 
	{
		if(motor->state.work_state == M_OFFLINE)
			motor->state.work_state = M_ONLINE;
	}
}

/**
 *	@brief	���pid��ʼ��
 */
void motor_class_pid_init(struct RM_motor_pid_t *pid, float *buff)
{
	if(pid == NULL || buff == NULL)
	{
		return;
	}
	
	pid->info.init_flag = M_INIT;
	
	pid->info.target   = 0;
	pid->info.measure  = 0;	
	pid->info.err      = 0;	
	pid->info.last_err = 0;
	pid->info.integral = 0;
	pid->info.pout     = 0;
	pid->info.iout     = 0;
	pid->info.dout     = 0;	
	pid->info.out      = 0;		
	
	pid->set.kp           = buff[0];
	pid->set.ki           = buff[1];	
	pid->set.kd           = buff[2];
	pid->set.blind_err    = buff[3];
	pid->set.integral_max = buff[4];
	pid->set.iout_max     = buff[5];	
	pid->set.out_max      = buff[6];
	
}	












/**
 *	@brief	���������Ϣ
 */
void can_tx_buff(struct RM_motor_class_t *motor, int16_t *buff,uint8_t len)
{
	if(motor == NULL || buff == NULL)
	{
		return;
	}
	
	if(motor->id.motor_type > 0 && motor->id.motor_type <= 3)
	{
		
	}
	
	
}

/**
 *	@brief	���������Ϣ
 */
void can_rx_buff(struct RM_motor_class_t *motor, uint8_t *buff)
{
	
	
	if(motor == NULL || buff == NULL)
	{
		return;
	}
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return;	
	}
	
	if(motor->id.motor_type > 0 && motor->id.motor_type <= 3)
	{
		get_rm_info(motor,buff);
	}
	else if(motor->id.motor_type > 3 && motor->id.motor_type <= 5)
	{
//		get_kt_9025_info(motor,buff);
	}
	
}





/**
*	@brief	��ת�жϣ����Ǳ�Ҫ������������һ����ס torque_limit:Ť����ֵ return:1Ϊ�� 0Ϊ��
 */
uint8_t motor_class_stucking_flag(struct RM_motor_class_t *motor, float torque_limit)
{
	uint8_t res = 0;
	
	
	if(motor->state.init_flag == M_DEINIT || motor->state.work_state == M_OFFLINE)return 0;	
	
	if(m_abs(motor->rx_info.torque) > torque_limit && m_abs(motor->rx_info.speed) < 5)
	{
		motor->base_info.lock_cnt++;
		
		if (motor->base_info.lock_cnt == 5)
			res = 1;
		else
			res = 0;
	}
	else
	{
		res = 0;
		motor->base_info.lock_cnt = 0;
	}
	
	return res;
	
}

/*-----------------------------------------------------------------
*���ݴ���
-----------------------------------------------------------------*/
/**
 *	@brief	���ڴ��� tar��Դ���� cycle:���ݷ�Χ
 */
float motor_cycle(float tar,float cycle)
{
	if(tar < 0)          tar += cycle;
	else if(tar >=cycle) tar -= cycle;
	
	return tar;
}
/**
 *	@brief	����Ȧ���� angle��Դ���� cycle:���ݷ�Χ
 */
float motor_half_cycle(float angle,float max)
{
	if(m_abs(angle) > (max/2))
	{	
		if(angle >= 0)
			angle += -max;		
		else
			angle +=  max;
	}
	return angle;
}
/**
 *	@brief	����������  limit:������ݷ�Χ
 */
void motor_judge_dir(struct RM_motor_class_t *motor,uint16_t range)
{
	int16_t angle = 0;
	
	if(motor->rx_info.angle < motor->mec_info.mec_mid)
		 angle = motor->rx_info.angle - motor->mec_info.mec_mid + range;
	else
	   angle = motor->rx_info.angle - motor->mec_info.mec_mid;

	if(m_abs(angle - range/2) < range/4)
	{
		motor->mec_info.dir = MOTOR_B;
	}
	else 
	{
		motor->mec_info.dir = MOTOR_F;
	}	
}
/**
 *	@brief	�Ե���Ƕ���ƫ�� ����yaw��������ԭʼ��ǰΪ1777���޸ĺ�ǰΪ0 
 *            limit:������ݷ�Χ
 */
void motor_offset(struct RM_motor_class_t *motor, uint16_t range)
{
	RM_motor_rx_info_t *info = &motor->rx_info;
	
	int16_t angle = 0;
	
	if(info->angle < motor->mec_info.mec_mid)
		 angle = info->angle - motor->mec_info.mec_mid + range;
	else
	   angle = info->angle - motor->mec_info.mec_mid;
	
  angle = -angle + range + range/4;
	
	if(angle > range)angle = angle - range;

	angle = motor_cycle(angle - range/4,range);
	
	info->angle_offset = angle;			
}


/*-----------------------------------------------------------------
*���pid
-----------------------------------------------------------------*/

/**
 *	@brief	��ȡ��� tar - mea
 */
float motor_pid_err(RM_motor_pid_t *pid,float measure)
{
	RM_motor_pid_info_t *pid_info = &pid->info;
	
	if(pid->info.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	pid_info->measure = measure;
	pid_info->err = pid_info->target - pid_info->measure;
	return pid_info->err;

}

/**
 *	@brief	pid���� ������err����
 */
float motor_pid_cal(RM_motor_pid_t *pid)
{
	
	if(pid->info.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	RM_motor_pid_info_t *pid_info = &pid->info;
	RM_motor_pid_set_t  *pid_set = &pid->set;	
	
	//�������ֵ(��Ҫ���������м������)
	//pid->err = err;
	if(m_abs(pid_info->err)<=(pid_set->blind_err))
		pid_info->err = 0;
	
	//����
	pid_info->integral += pid_info->err;
	pid_info->integral = m_constrain(pid_info->integral, -pid_set->integral_max, +pid_set->integral_max);
	
	//pid ��������
	pid_info->pout = pid_set->kp * pid_info->err;
	pid_info->iout = pid_set->ki * pid_info->integral;
	pid_info->dout = pid_set->kd * (pid_info->err - pid_info->last_err);
	
	pid_info->iout = m_constrain(pid_info->iout, -pid_set->iout_max, pid_set->iout_max);	
	//�ۼ�pid���ֵ
	pid_info->out = pid_info->pout + pid_info->iout + pid_info->dout;
	pid_info->out = m_constrain(pid_info->out, -pid_set->out_max, pid_set->out_max);

	pid_info->last_err = pid_info->err;
	
	return pid_info->out;
}

/**
 *	@brief	pid�ܿ��� �������⻷ �ڻ� �⻷�۲�ֵ �ڻ��۲�ֵ err����ʽ
 *          err_cal_mode��err����ʽ ��Ȧ�����ķ�֮һȦ 0��1��2 �ٶȻ�ʹ��0 yaw��ʹ��1
 *          ���ڻ�ΪNULL��ֻ�����⻷���⻷����ΪNULL
 *
 *  @return ���ؼ�����
 */
float motor_pid_ctrl(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, char err_cal_mode)
{
	if(out == NULL)return 0;
	
	if(inn == NULL)
	{
		motor_pid_err(out , meas1);	
		switch(err_cal_mode)
		{
			case 0:			
				break;
			
			case 1:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				break;				
			
			case 2:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				out->info.err = motor_half_cycle(out->info.err, 4095);
				break;			
		}
		motor_pid_cal(out);
		
		return out->info.out;	
	}
	else 
	{
		/*--�⻷����--*/
		motor_pid_err(out , meas1);	
		switch(err_cal_mode)
		{
			case 0:			
				break;
			
			case 1:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				break;				
			
			case 2:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				out->info.err = motor_half_cycle(out->info.err, 4095);
				break;			
		}
		motor_pid_cal(out);
		
		inn->info.target = out->info.out;	//Ŀ��ֵת�Ƶ��ٶȻ�
		
		/*--�ڻ�����--*/
		motor_pid_err(inn , meas2);  
		motor_pid_cal(inn);	
		
		return inn->info.out;	
	}
}

/**
 *	@brief	˫��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_double(RM_motor_pid_t *out, RM_motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode)
{

	out->info.target = tar;

	return motor_pid_ctrl(out,inn,meas1,meas2,err_cal_mode);

}


/**
 *	@brief	��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_single(RM_motor_pid_t *out, float meas1, float tar)
{

	out->info.target = tar;

	return motor_pid_ctrl(out,NULL,meas1,NULL,0);

}

/**
 *	@brief	λ��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_position(struct RM_motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	if(motor->pid.position.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.position.info.target = target;
	
	
	motor->base_info.motor_out = motor_pid_ctrl(&motor->pid.position,&motor->pid.position_in,motor->rx_info.angle_sum,motor->rx_info.speed,0);
	
	return motor->base_info.motor_out;
	
}

/**
 *	@brief	�Ƕ�pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_angle(struct RM_motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}

	if(motor->pid.angle.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.angle.info.target = target;
	
	motor->base_info.motor_out = motor_pid_ctrl(&motor->pid.angle,&motor->pid.angle_in,motor->rx_info.angle,motor->rx_info.speed,1);
	
	return motor->base_info.motor_out;
	
}

/**
 *	@brief	�ٶ�pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_speed(struct RM_motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}	

	if(motor->pid.speed.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.speed.info.target = target;
	
	motor->base_info.motor_out = motor_pid_ctrl(&motor->pid.speed,NULL,motor->rx_info.speed,NULL,0);  
	
	return motor->base_info.motor_out;
	
}




/*-----------------------------------------------------------------
*�󽮵������
-----------------------------------------------------------------*/

/**
 *	@brief	��CAN�����ж�ȡ�����λ�÷���
 */
static uint16_t CAN_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = ((uint16_t)rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ת��ת�ٷ���
 */
static int16_t CAN_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = ((uint16_t)rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ��ת�ص�������
 */
static int16_t CAN_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = ((int16_t)rxData[4] << 8 | rxData[5]);
	return current;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ�����ת��
 */
static int16_t CAN_GetMotorTorque(uint8_t *rxData)
{
	int16_t torque;
	torque = ((int16_t)rxData[4] << 8 | rxData[5]);
	return torque;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ���¶�
 */
static uint8_t CAN_GetMotorTemperature(uint8_t *rxData)
{
	uint8_t temperature;
	temperature = rxData[6];
	return temperature;
}

/**
 *	@brief	�󽮵��can������Ϣ
 *  @return 
 */
void get_rm_can_drvie(struct RM_motor_class_t *motor)
{
	if(motor == NULL)
	{
		return;
	}
	
	motor->rx = can_rx_buff;
	motor->tx = can_tx_buff;			
	
	if(motor->id.motor_type == GM6020)
	{
			if((motor->id.rx_id - 0x205U) < 4)
				motor->id.tx_id = 0x1FF;
			else
				motor->id.tx_id = 0x2FF;
			
			motor->id.buff_p = (motor->id.rx_id - 0x205U)%4;
	}
	else if(motor->id.motor_type == RM3508)
	{
			if((motor->id.rx_id - 0x201U) < 4)
				motor->id.tx_id = 0x200;
			else
				motor->id.tx_id = 0x1FF;
			
			motor->id.buff_p = (motor->id.rx_id - 0x201U)%4;
	}		
	else if(motor->id.motor_type == RM2006)
	{
			if((motor->id.rx_id - 0x201U) < 4)
				motor->id.tx_id = 0x200;
			else
				motor->id.tx_id = 0x1FF;
			
			motor->id.buff_p = (motor->id.rx_id - 0x201U)%4;
	}			

}

/**
 *	@brief	can������Ϣ����
 *  @return
 */
void get_rm_info(struct RM_motor_class_t *motor, uint8_t *rxBuf)
{
	int16_t err;

	RM_motor_rx_info_t *motor_info = &motor->rx_info;
	
	motor_info->angle   = CAN_GetMotorAngle(rxBuf);	
	motor_info->speed   = CAN_GetMotorSpeed(rxBuf);
	motor_info->current = CAN_GetMotorCurrent(rxBuf);
	motor_info->torque  = CAN_GetMotorTorque(rxBuf);	
	motor_info->temperature = CAN_GetMotorTemperature(rxBuf);

	if(!motor_info->angle_prev && !motor_info->angle_sum)
		err = 0;
	else
		err = motor_info->angle - motor_info->angle_prev;
	
	/* ����� */
	if(m_abs(err) > 4095)
	{
		/* 0�� -> 8191 */
		if(err >= 0)
			motor_info->angle_sum += -8191 + err;
		/* 8191�� -> 0 */
		else
			motor_info->angle_sum += 8191 + err;
	}
	/* δ����� */
	else
	{
		motor_info->angle_sum += err;
	}
	
	motor_info->angle_prev = motor_info->angle;		
	
	motor->state.offline_cnt = 0;
}
