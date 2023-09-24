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
// 底盘电机本地驱动
drv_can_t				*chas_drv[CHAS_MOTOR_CNT];
chassis_motor_t			*chas_motor[CHAS_MOTOR_CNT];
chassis_motor_info_t	*chas_motor_info[CHAS_MOTOR_CNT];

/* Exported variables --------------------------------------------------------*/
// 底盘电机PID控制器
chassis_motor_pid_t 	chas_motor_pid[CHAS_MOTOR_CNT] = {
	[CHAS_L] = {	
		//直立环
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
		
		//定位环
		.locat_pid.kp = 1.1f,
		.locat_pid.ki = 0.003f,
		.locat_pid.kd = 0.f,
		.locat_pid.integral_max = 12000.f,
		.locat_pid.out_max = 800.f,	
		.locat_pid.blind_err = 0.f,
		
		//转向环
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

// 底盘Z方向角度控制器
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

// 底盘模块控制器
chassis_ctrl_t		chas_ctrl = {
	.motor = &chas_motor_pid,
	.z_atti = &chas_z_pid,
};

// 底盘模块传感器
chassis_dev_t		chas_dev = {
	.chas_motor[CHAS_L] = &chassis_motor[CHAS_L],
	.chas_motor[CHAS_R] = &chassis_motor[CHAS_R],
	.chas_motor[CHAS_F] = &chassis_motor[CHAS_F],
	.chas_motor[CHAS_B] = &chassis_motor[CHAS_B],
//	.yaw_motor = &gimbal_motor[YAW],
	.imu_sensor = &imu_sensor,
	.rc_sensor = &rc_sensor,
};

// 底盘模块信息
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
/* 驱动层 --------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* 信息层 --------------------------------------------------------------------*/
/**
 *	@brief	底盘获取系统信息
 */
void CHASSIS_GetSysInfo(void)//只要确保随时切换无问题即可就行
{
	/*----控制方式修改----*/
	if(sys.remote_mode == RC) {
		chas_info.remote_mode = RC;
	}
	else if(sys.remote_mode == KEY) {
		chas_info.remote_mode = KEY;
	}
	
	

	/*----本地模式修改----*/
	switch(sys.mode)	//切换模式
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
	//无？ 或者是裁判系统的数据获取
}

void CHASSIS_GetRcInfo(void)
{
	/* 系统正常 */
	if(sys.state == SYS_STATE_NORMAL)
	{
		if(sys.remote_mode == RC) {
			
			
		}
		else if(sys.remote_mode == KEY) {
			
		}
	}
	/* 系统异常 失联 操作 其他 */
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

/* 应用层 --------------------------------------------------------------------*/
/* 任务层 --------------------------------------------------------------------*/

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

void CHASSIS_GetInfo(void)	//更新本地操作模式
{
	CHASSIS_GetSysInfo();								//获取系统整体模式状态
	CHASSIS_GetJudgeInfo();							//判断底盘有效性
	CHASSIS_GetRcInfo();								//获取遥控器信息
	CHASSIS_GetTopGyroInfo();						//判断是否处于旋转状态
	CHASSIS_GetSelfAttitude();					//获取自身？？？	
	CHASSIS_UpdateController();					//更新控制器信息内容
}

void CHASSIS_SelfProtect(void)
{
// 断连保护
//	CHASSIS_Stop(chas_motor_pid);
//	CHASSIS_PidParamsInit(chas_motor_pid, CHAS_MOTOR_CNT);
//	CHASSIS_Z_PidParamsInit(&chas_z_pid);
//	CHASSIS_GetInfo();
}
/*pid输出值*/
float     stand_out_L,stand_out_R,
	        locat_out_L,locat_out_R,
          turnR_out_L,turnR_out_R,
          stand_wheel_F,stand_wheel_B;\
/*can发出值*/
short int send_outCHAS_L,send_outCHAS_R,
	        send_outWheel_F,send_outWheel_B;
/*遥控器控制值*/
float chassis_remoteforward;
void CHASSIS_PidCtrl(void)
{
	//直立环无论开不开都计算
	pid_ctrl_t *L_anglepid = & chas_motor_pid[CHAS_L].stand_anglepid;//pid外环参数
	//外环
	
	L_anglepid->err = imu_sensor.info->Longtime_imu_Array[pitch_longtime].imu_angle_sum
	                  - rc_sensor.info->ch3 * chassis_remoteforward;//目标偏移角度
	
	single_pid_ctrl(L_anglepid);
	//内环
	pid_ctrl_t *L_speedpid = & chas_motor_pid[CHAS_L].stand_speedpid;
	
	L_speedpid->measure = KalmanFilter(&KAL_Pid[kal_standI].info->Stand_speed_kal,imu_sensor.info->rate_pitch+10);//卡尔曼滤波-gyroy
	L_speedpid->err = L_anglepid->out - L_speedpid->measure;
	
	single_pid_ctrl(L_speedpid);
	
	stand_out_L = L_speedpid->out;
	stand_out_R = -stand_out_L;
	//计算pid
	/////////////////////////////////////////////////////
	//速度环
	chassis_motor_info_t *L_speed = chassis_motor[CHAS_L].info;
	chassis_motor_info_t *R_speed = chassis_motor[CHAS_R].info;
	pid_ctrl_t *L_locat_pid = &chas_motor_pid[CHAS_L].locat_pid;//pid参数
	
	L_locat_pid->target = - L_speed->speed + R_speed->speed;//假目标实错误

	balan_locat_ctrl(L_locat_pid);
	
	locat_out_L = L_locat_pid->out * L_speedpid->kp * 1;
	locat_out_R = -locat_out_L;
	///////////////////////////////////////////////////////
	//转向环
	pid_ctrl_t *L_turnR_pid = &chas_motor_pid[CHAS_L].TurnR_pid;//pid参数
	
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
		if(sys.circle_mode.F_wheel == ReSet)//配置前轮初始化
		{
			chassis_motor_info_t *F_speed = chassis_motor[CHAS_F].info;
			pid_ctrl_t *F_wheel_pid = &chas_motor_pid[CHAS_F].speed;//前轮速度环pid
			F_wheel_pid ->err = 2200 - F_speed->speed;//2000是目标转速
			single_pid_ctrl(F_wheel_pid);
			if(abs(F_wheel_pid->err - F_wheel_pid->last_err)<=2
				 &&  abs(send_outWheel_F)>0)//电机有力且堵转
			{
						i_F++;
						if(i_F>=150)
						{
							sys.circle_mode.F_wheel = Set;
							chassis_motor[CHAS_F].info->angle_sum = 0;//设置零点
						}
			}
			
			stand_wheel_F = F_wheel_pid->out;//前轮准备好了依旧保持此值输出
		}
		//////////////////////////////////////////////////////////////////////////
		if(sys.circle_mode.B_wheel == ReSet)//后轮初始化
		{
			chassis_motor_info_t *B_speed = chassis_motor[CHAS_B].info;//后轮转速
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
							chassis_motor[CHAS_B].info->angle_sum = 0;//因为双环刚开始的目标值为0,先进行清零
						}
			}
			
			stand_wheel_B = B_wheel_pid->out;
		}
		//////////////////////////////////////////////////////////////////////
		send_outCHAS_L = 0;
		send_outCHAS_R = 0;//初始化时底盘轮子不输出
		if(sys.circle_mode.F_wheel == Set && sys.circle_mode.B_wheel == Set)
		{
			sys.circle_mode.FB_wheel = Set;//此函数不作用了
			co_sequent.FBco_sequent = level_1;
			pid_val_init(&chas_motor_pid[CHAS_F].speed);
			pid_val_init(&chas_motor_pid[CHAS_B].speed);//清理pid内环，之后都是双环嵌套
		}
	}
}//第一级
float F_angle_target,B_angle_target;//全局变量，可以在lowcontrol_task中改变
void FBwheel_PidangleCtrl(float F_angle_targ,float B_angle_targ)//在高于或等于前后轮2级使用
{
	//前轮
	pid_ctrl_t *Fangle_pid = &chas_motor_pid[CHAS_F].angle;
	Fangle_pid->err = F_angle_targ - chassis_motor[CHAS_F].info->angle_sum;
	
	single_pid_ctrl(Fangle_pid);

	pid_ctrl_t *Fspeed_pid = &chas_motor_pid[CHAS_F].speed;//内环参数
	chassis_motor_info_t *F_speed = chassis_motor[CHAS_F].info;//前轮转速
	Fspeed_pid->err = Fangle_pid->out - F_speed->speed;
	
	single_pid_ctrl(Fspeed_pid);
	stand_wheel_F = Fspeed_pid->out;
	///////////////////////////////////////////////////////////////////////////////////
	//后轮
	pid_ctrl_t *Bangle_pid = &chas_motor_pid[CHAS_B].angle;
	Bangle_pid->err = B_angle_targ - chassis_motor[CHAS_B].info->angle_sum;
	
	single_pid_ctrl(Bangle_pid);
	
	pid_ctrl_t *Bspeed_pid = &chas_motor_pid[CHAS_B].speed;
	chassis_motor_info_t *B_speed = chassis_motor[CHAS_B].info;
	Bspeed_pid->err = Bangle_pid->out - B_speed->speed;
	
	single_pid_ctrl(Bspeed_pid);
	stand_wheel_B = Bspeed_pid->out;
}

static void CHASSIS_FBstandinit(void)//第二三级
{
	if(co_sequent.FBco_sequent == level_1)
	{
		F_angle_target = -30000;//!!!!!!!!!!!!!后面要改
		B_angle_target = -34600;
		FBwheel_PidangleCtrl(F_angle_target,B_angle_target);
		co_sequent.FBco_sequent = level_2;
		send_outCHAS_L = 0;
		send_outCHAS_R = 0;//初始化时底盘轮子不输出
	}
	if(co_sequent.FBco_sequent == level_2)
	{
		static int stand_i;
		FBwheel_PidangleCtrl(F_angle_target,B_angle_target);//设了目标值也是要不断计算的
		if (abs(imu_sensor.info->Longtime_imu_Array[pitch_longtime].imu_angle_sum)<=6)
		{
			stand_i++;
			if(stand_i>=80)
			{
				co_sequent.FBco_sequent = level_3;
				F_angle_target = -4000;//!!!!!!!!!!!!!后面要改
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
	/*----信息读入----*/
	CHASSIS_GetInfo();
	/*----期望修改----*/ 
	if(chas_info.remote_mode == RC) {
		CHASSIS_RcCtrl();
	}
	else if(chas_info.remote_mode == KEY) {
		CHASSIS_KeyCtrl();
	}
	/*----最终输出----*/
	CHASSIS_PidCtrl();	
}

void CHASSIS_RCnormal_output(void)//此函数放在前后轮之前，在初始化前后轮的时候令send_outCHAS_Lsend_outCHAS_R=0
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


