/**
 * @file        chassis.c
 * @author      MarkVimy
 * @Version     V1.0
 * @date        23-October-2020
 * @brief       Chassis Module.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "chassis.h"

#include "can_potocol.h"
#include "rp_math.h"
#include "kalman.h"
#include "kalman_filter.h"
#include "drv_can.h"
#include "can.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CHASSIS_Init(void);
void CHASSIS_Ctrl(void);
void CHASSIS_Test(void);
void CHASSIS_SelfProtect(void);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// ���̵����������
drv_can_t				*chas_drv[CHAS_MOTOR_CNT];
chassis_motor_t			*chas_motor[CHAS_MOTOR_CNT];
chassis_motor_info_t	*chas_motor_info[CHAS_MOTOR_CNT];

/* Exported variables --------------------------------------------------------*/
// ���̵��PID������
chassis_motor_pid_t 	chas_motor_pid[CHAS_MOTOR_CNT] = {
	[CHAS_L] = {	
		//ֱ����
		.stand_anglepid.kp = 45.f,
		.stand_anglepid.ki = 0.f,
		.stand_anglepid.kd = 0.f,
		.stand_anglepid.integral_max = 0.f,
		.stand_anglepid.out_max = 2000.f,
		.stand_anglepid.blind_err = 0.f,
		
		.stand_speedpid.kp = 7.f,
		.stand_speedpid.ki = 0.09f,
		.stand_speedpid.kd = 0.f,
		.stand_speedpid.integral_max = 18000.f,
		.stand_speedpid.out_max = 8000.f,	
		.stand_speedpid.blind_err = 0.f,
		
		//��λ��
		.locat_pid.kp = 1.1f,
		.locat_pid.ki = 0.003f,
		.locat_pid.kd = 0.f,
		.locat_pid.integral_max = 12000.f,
		.locat_pid.out_max = 800.f,	
		.locat_pid.blind_err = 0.f,
		
		//ת��
		.TurnR_pid.kp = 0.08f,
		.TurnR_pid.ki = 0.f,
		.TurnR_pid.kd = 3.5f,
		.TurnR_pid.integral_max = 0.f,
		.TurnR_pid.out_max = 4000.f,	
		.TurnR_pid.blind_err = 0.f,
	},
	[CHAS_R] = {
		.stand_anglepid.kp = 0.f,
	},
	[CHAS_F] = {
		.speed.kp = 6.f,
		.speed.ki = 0.8f,
		.speed.kd = 8.f,
		.speed.integral_max = 5000.f,
		.speed.out_max = 8000.f,
		.speed.blind_err = 5.f,
		.angle.kp = 0.1f,
		.angle.ki = 0.f,
		.angle.kd = 0.f,
		.angle.integral_max = 0.f,
		.angle.out_max = 1000.f,
		.angle.blind_err = 0.f,
	},
	[CHAS_B] = {
		.speed.kp = 6.f,
		.speed.ki = 0.8f,
		.speed.kd = 8.f,
		.speed.integral_max = 5000.f,
		.speed.out_max = 8000.f,
		.speed.blind_err = 5.f,
		.angle.kp = 0.1f,
		.angle.ki = 0.f,
		.angle.kd = 0.f,
		.angle.integral_max = 0.f,
		.angle.out_max = 1000.f,
		.angle.blind_err = 0.f,
	},
};

// ����Z����Ƕȿ�����
chassis_z_pid_t		chas_z_pid = {
	.angle = {
		.target = CHAS_MECH_ANGLE_POS_MID,
		.measure = CHAS_MECH_ANGLE_POS_MID,
		.kp = 0.48,
		.ki = 0,
		.kd = 0,
		.integral_max = 0,
		.out_max = 8000,
	},
	.out = 0,
};

// ����ģ�������
chassis_ctrl_t		chas_ctrl = {
	.motor = &chas_motor_pid,
	.z_atti = &chas_z_pid,
};

// ����ģ�鴫����
chassis_dev_t		chas_dev = {
	.chas_motor[CHAS_L] = &chassis_motor[CHAS_L],
	.chas_motor[CHAS_R] = &chassis_motor[CHAS_R],
	.chas_motor[CHAS_F] = &chassis_motor[CHAS_F],
	.chas_motor[CHAS_B] = &chassis_motor[CHAS_B],
//	.yaw_motor = &gimbal_motor[YAW],
	.imu_sensor = &imu_sensor,
	.rc_sensor = &rc_sensor,
};

// ����ģ����Ϣ
chassis_info_t 	chas_info = {
	.remote_mode = RC,
	.co_mode = CO_MECH,
	.local_mode = CHASSIS_MODE_NORMAL,
	.co_angle_logic = LOGIC_FRONT,
};

chassis_t chassis = {
	.controller = &chas_ctrl,
	.dev = &chas_dev,
	.info = &chas_info,
	.init = CHASSIS_Init,
	.test = CHASSIS_Test,
	.ctrl = CHASSIS_Ctrl,
	.self_protect = CHASSIS_SelfProtect,
};

/* Private functions ---------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* ��Ϣ�� --------------------------------------------------------------------*/
/**
 *	@brief	���̻�ȡϵͳ��Ϣ
 */
void CHASSIS_GetSysInfo(void)//ֻҪȷ����ʱ�л������⼴�ɾ���
{
	/*----���Ʒ�ʽ�޸�----*/
	if(sys.remote_mode == RC) {
		chas_info.remote_mode = RC;
	}
	else if(sys.remote_mode == KEY) {
		chas_info.remote_mode = KEY;
	}
	
	

	/*----����ģʽ�޸�----*/
	switch(sys.mode)	//�л�ģʽ
	{
		case SYS_MODE_LONGSHOOT: 
			if(chas_info.local_mode!=CHASSIS_MODE_SZUPUP)
			{
				chas_info.local_mode =  CHASSIS_MODE_LONGSHOOT;	
			}
			break;

		case SYS_MODE_AUTO:
		case SYS_MODE_NORMAL:		
			chas_info.local_mode = CHASSIS_MODE_NORMAL;
			break;
		case SYS_MODE_PARK: break;
		
		default: break;
	}
	
	chas_info.co_mode = sys.co_mode;
}

void CHASSIS_GetJudgeInfo(void)
{
	//�ޣ� �����ǲ���ϵͳ�����ݻ�ȡ
}

void CHASSIS_GetRcInfo(void)
{
	/* ϵͳ���� */
	if(sys.state == SYS_STATE_NORMAL)
	{
		if(sys.remote_mode == RC) {
			
			
		}
		else if(sys.remote_mode == KEY) {
			
		}
	}
	/* ϵͳ�쳣 ʧ�� ���� ���� */
	else 
	{
		
	}
}

void CHASSIS_GetTopGyroInfo(void)
{
	
}

void CHASSIS_GetSelfAttitude(void)
{
	
}

void CHASSIS_UpdateController(void)
{
	chas_motor_pid[CHAS_L].speed.measure = chas_motor_info[CHAS_L]->speed;
	chas_motor_pid[CHAS_R].speed.measure = chas_motor_info[CHAS_R]->speed;
	chas_motor_pid[CHAS_F].speed.measure = chas_motor_info[CHAS_F]->speed;
	chas_motor_pid[CHAS_B].speed.measure = chas_motor_info[CHAS_B]->speed;	
}

/* Ӧ�ò� --------------------------------------------------------------------*/
/* ����� --------------------------------------------------------------------*/

kalman_pid_t *chas_kal_algorithm[KAL_CNT];
void CHASSIS_Init(void)
{
	chas_drv[CHAS_L] = chas_dev.chas_motor[CHAS_L]->driver;
	chas_drv[CHAS_R] = chas_dev.chas_motor[CHAS_R]->driver;
	chas_drv[CHAS_F] = chas_dev.chas_motor[CHAS_F]->driver;
	chas_drv[CHAS_B] = chas_dev.chas_motor[CHAS_B]->driver;

	chas_motor[CHAS_L] = chas_dev.chas_motor[CHAS_L];
	chas_motor[CHAS_R] = chas_dev.chas_motor[CHAS_R];
	chas_motor[CHAS_F] = chas_dev.chas_motor[CHAS_F];
	chas_motor[CHAS_B] = chas_dev.chas_motor[CHAS_B];	
	
	chas_motor_info[CHAS_L] = chas_dev.chas_motor[CHAS_L]->info;
	chas_motor_info[CHAS_R] = chas_dev.chas_motor[CHAS_R]->info;
	chas_motor_info[CHAS_F] = chas_dev.chas_motor[CHAS_F]->info;
	chas_motor_info[CHAS_B] = chas_dev.chas_motor[CHAS_B]->info;
	
//	chas_kal_algorithm[kal_standI] = KAL_Pid[kal_standI].init;
}

void CHASSIS_GetInfo(void)	//���±��ز���ģʽ
{
	CHASSIS_GetSysInfo();								//��ȡϵͳ����ģʽ״̬
	CHASSIS_GetJudgeInfo();							//�жϵ�����Ч��
	CHASSIS_GetRcInfo();								//��ȡң������Ϣ
	CHASSIS_GetTopGyroInfo();						//�ж��Ƿ�����ת״̬
	CHASSIS_GetSelfAttitude();					//��ȡ��������	
	CHASSIS_UpdateController();					//���¿�������Ϣ����
}

void CHASSIS_SelfProtect(void)
{
// ��������
//	CHASSIS_Stop(chas_motor_pid);
//	CHASSIS_PidParamsInit(chas_motor_pid, CHAS_MOTOR_CNT);
//	CHASSIS_Z_PidParamsInit(&chas_z_pid);
//	CHASSIS_GetInfo();
}
/*pid���ֵ*/
float     stand_out_L,stand_out_R,
	        locat_out_L,locat_out_R,
          turnR_out_L,turnR_out_R,
          stand_wheel_F,stand_wheel_B;\
/*can����ֵ*/
short int send_outCHAS_L,send_outCHAS_R,
	        send_outWheel_F,send_outWheel_B;
/*ң��������ֵ*/
float chassis_remoteforward;
void CHASSIS_PidCtrl(void)
{
	//ֱ�������ۿ�����������
	pid_ctrl_t *L_anglepid = & chas_motor_pid[CHAS_L].stand_anglepid;//pid�⻷����
	//�⻷
	
	L_anglepid->err = imu_sensor.info->Longtime_imu_Array[pitch_longtime].imu_angle_sum
	                  - rc_sensor.info->ch3 * chassis_remoteforward;//Ŀ��ƫ�ƽǶ�
	
	single_pid_ctrl(L_anglepid);
	//�ڻ�
	pid_ctrl_t *L_speedpid = & chas_motor_pid[CHAS_L].stand_speedpid;
	
	L_speedpid->measure = KalmanFilter(&KAL_Pid[kal_standI].info->Stand_speed_kal,imu_sensor.info->rate_pitch+10);//�������˲�-gyroy
	L_speedpid->err = L_anglepid->out - L_speedpid->measure;
	
	single_pid_ctrl(L_speedpid);
	
	stand_out_L = L_speedpid->out;
	stand_out_R = -stand_out_L;
	//����pid
	/////////////////////////////////////////////////////
	//�ٶȻ�
	chassis_motor_info_t *L_speed = chassis_motor[CHAS_L].info;
	chassis_motor_info_t *R_speed = chassis_motor[CHAS_R].info;
	pid_ctrl_t *L_locat_pid = &chas_motor_pid[CHAS_L].locat_pid;//pid����
	
	L_locat_pid->target = - L_speed->speed + R_speed->speed;//��Ŀ��ʵ����

	balan_locat_ctrl(L_locat_pid);
	
	locat_out_L = L_locat_pid->out * L_speedpid->kp * 1;
	locat_out_R = -locat_out_L;
	///////////////////////////////////////////////////////
	//ת��
	pid_ctrl_t *L_turnR_pid = &chas_motor_pid[CHAS_L].TurnR_pid;//pid����
	
	L_turnR_pid ->err += L_speed->speed + R_speed->speed;
	
	balan_turn_ctrl(L_turnR_pid);
	
	turnR_out_L = L_turnR_pid->out;
	turnR_out_R = -turnR_out_L;
	
}

co_sequent_list co_sequent;

static void CHASSIS_FBSetinit(void)
{
	if(sys.circle_mode.FB_wheel == ReSet)
	{
		static int i_F,j_B;	
		////////////////////////////////////////////////////////////////
		if(sys.circle_mode.F_wheel == ReSet)//����ǰ�ֳ�ʼ��
		{
			chassis_motor_info_t *F_speed = chassis_motor[CHAS_F].info;
			pid_ctrl_t *F_wheel_pid = &chas_motor_pid[CHAS_F].speed;//ǰ���ٶȻ�pid
			F_wheel_pid ->err = 2200 - F_speed->speed;//2000��Ŀ��ת��
			single_pid_ctrl(F_wheel_pid);
			if(abs(F_wheel_pid->err - F_wheel_pid->last_err)<=2
				 &&  abs(send_outWheel_F)>0)//��������Ҷ�ת
			{
						i_F++;
						if(i_F>=150)
						{
							sys.circle_mode.F_wheel = Set;
							chassis_motor[CHAS_F].info->angle_sum = 0;//�������
						}
			}
			
			stand_wheel_F = F_wheel_pid->out;//ǰ��׼���������ɱ��ִ�ֵ���
		}
		//////////////////////////////////////////////////////////////////////////
		if(sys.circle_mode.B_wheel == ReSet)//���ֳ�ʼ��
		{
			chassis_motor_info_t *B_speed = chassis_motor[CHAS_B].info;//����ת��
			pid_ctrl_t *B_wheel_pid = &chas_motor_pid[CHAS_B].speed;
			B_wheel_pid ->err = 2200 - B_speed->speed;
			single_pid_ctrl(B_wheel_pid);
			if(abs(B_wheel_pid->err - B_wheel_pid->last_err)<=2
				&& abs(send_outWheel_B)>0)
			{
				    j_B++;
						if(j_B>=150)
						{
							sys.circle_mode.B_wheel = Set;
							chassis_motor[CHAS_B].info->angle_sum = 0;//��Ϊ˫���տ�ʼ��Ŀ��ֵΪ0,�Ƚ�������
						}
			}
			
			stand_wheel_B = B_wheel_pid->out;
		}
		//////////////////////////////////////////////////////////////////////
		send_outCHAS_L = 0;
		send_outCHAS_R = 0;//��ʼ��ʱ�������Ӳ����
		if(sys.circle_mode.F_wheel == Set && sys.circle_mode.B_wheel == Set)
		{
			sys.circle_mode.FB_wheel = Set;//�˺�����������
			co_sequent.FBco_sequent = level_1;
			pid_val_init(&chas_motor_pid[CHAS_F].speed);
			pid_val_init(&chas_motor_pid[CHAS_B].speed);//����pid�ڻ���֮����˫��Ƕ��
		}
	}
}//��һ��
float F_angle_target,B_angle_target;//ȫ�ֱ�����������lowcontrol_task�иı�
void FBwheel_PidangleCtrl(float F_angle_targ,float B_angle_targ)//�ڸ��ڻ����ǰ����2��ʹ��
{
	//ǰ��
	pid_ctrl_t *Fangle_pid = &chas_motor_pid[CHAS_F].angle;
	Fangle_pid->err = F_angle_targ - chassis_motor[CHAS_F].info->angle_sum;
	
	single_pid_ctrl(Fangle_pid);

	pid_ctrl_t *Fspeed_pid = &chas_motor_pid[CHAS_F].speed;//�ڻ�����
	chassis_motor_info_t *F_speed = chassis_motor[CHAS_F].info;//ǰ��ת��
	Fspeed_pid->err = Fangle_pid->out - F_speed->speed;
	
	single_pid_ctrl(Fspeed_pid);
	stand_wheel_F = Fspeed_pid->out;
	///////////////////////////////////////////////////////////////////////////////////
	//����
	pid_ctrl_t *Bangle_pid = &chas_motor_pid[CHAS_B].angle;
	Bangle_pid->err = B_angle_targ - chassis_motor[CHAS_B].info->angle_sum;
	
	single_pid_ctrl(Bangle_pid);
	
	pid_ctrl_t *Bspeed_pid = &chas_motor_pid[CHAS_B].speed;
	chassis_motor_info_t *B_speed = chassis_motor[CHAS_B].info;
	Bspeed_pid->err = Bangle_pid->out - B_speed->speed;
	
	single_pid_ctrl(Bspeed_pid);
	stand_wheel_B = Bspeed_pid->out;
}

static void CHASSIS_FBstandinit(void)//�ڶ�����
{
	if(co_sequent.FBco_sequent == level_1)
	{
		F_angle_target = -30000;//!!!!!!!!!!!!!����Ҫ��
		B_angle_target = -34600;
		FBwheel_PidangleCtrl(F_angle_target,B_angle_target);
		co_sequent.FBco_sequent = level_2;
		send_outCHAS_L = 0;
		send_outCHAS_R = 0;//��ʼ��ʱ�������Ӳ����
	}
	if(co_sequent.FBco_sequent == level_2)
	{
		static int stand_i;
		FBwheel_PidangleCtrl(F_angle_target,B_angle_target);//����Ŀ��ֵҲ��Ҫ���ϼ����
		if (abs(imu_sensor.info->Longtime_imu_Array[pitch_longtime].imu_angle_sum)<=6)
		{
			stand_i++;
			if(stand_i>=80)
			{
				co_sequent.FBco_sequent = level_3;
				F_angle_target = -4000;//!!!!!!!!!!!!!����Ҫ��
				B_angle_target = -5000;
//				FBwheel_PidangleCtrl(F_angle_target,B_angle_target);
			}
		}
	}
}
void CHASSIS_FBinit(void)
{
	if(co_sequent.FBco_sequent < level_3)
	{
		CHASSIS_FBSetinit();
		CHASSIS_FBstandinit();
	}
}


void CHASSIS_NormalCtrl(void)
{
	
}

void CHASSIS_ReloadBulletCtrl(void)
{
}

void CHASSIS_SzuPupCtrl(void)
{
}

void CHASSIS_BuffCtrl(void)
{
}

void CHASSIS_RcCtrl(void)//!!!!!
{
	
}

void CHASSIS_KeyCtrl(void)
{

}

void CHASSIS_Ctrl(void)
{
	/*----��Ϣ����----*/
	CHASSIS_GetInfo();
	/*----�����޸�----*/ 
	if(chas_info.remote_mode == RC) {
		CHASSIS_RcCtrl();
	}
	else if(chas_info.remote_mode == KEY) {
		CHASSIS_KeyCtrl();
	}
	/*----�������----*/
	CHASSIS_PidCtrl();	
}

void CHASSIS_RCnormal_output(void)//�˺�������ǰ����֮ǰ���ڳ�ʼ��ǰ���ֵ�ʱ����send_outCHAS_Lsend_outCHAS_R=0
{
	if(sys.circle_mode.Stand_circle!=Set)
	{
		stand_out_L = stand_out_R = 0;
	}
	if(sys.circle_mode.Locat_circle!=Set)
	{
		locat_out_L = locat_out_R = 0;
	}
	if(sys.circle_mode.Turnaround_circle!=Set)
	{
		turnR_out_L = turnR_out_R = 0;
	}
	send_outCHAS_L = (short int)(-stand_out_L-locat_out_L-turnR_out_L);
	send_outCHAS_R = (short int)(-stand_out_R-locat_out_R+turnR_out_R);
	
	send_outCHAS_L = constrain(send_outCHAS_L,-9000,9000);
	send_outCHAS_R = constrain(send_outCHAS_R,-9000,9000);
}
void CHASSIS_RClost_output(void)
{
	send_outCHAS_L = 0;
	send_outCHAS_R = 0;
}
int16_t Send_CHAS_Array[4]; 
void CHASSIS_output(void)
{
	Send_CHAS_Array[0] = send_outCHAS_R;//0x201
	Send_CHAS_Array[1] = send_outCHAS_L;//0x202
  CAN_SendData(&hcan1,0x200,Send_CHAS_Array);
}
void CHASSIS_Test(void)
{
}


