/**
 * @file        pid.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        11-September-2020
 * @brief       Pid Algorithm.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "rp_math.h"
#include "kalman.h"
#include "imu_sensor.h"
#include "rc_sensor.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void pid_val_init(pid_ctrl_t *pid)
{
//	pid->target = 0;
//	pid->measure = 0;
	pid->err = 0;
	pid->last_err = 0;
	pid->integral = 0;
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	pid->out = 0;
}

void single_pid_ctrl(pid_ctrl_t *pid)//直立环外环
{
	// 保存误差值(需要在外面自行计算误差)
//	pid->err = err;
	if(abs(pid->err)<=(pid->blind_err))
		pid->err = 0;
	// 积分
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max, +pid->integral_max);
	// p i d 输出项计算
	pid->pout = pid->kp * pid->err;
	pid->iout = pid->ki * pid->integral;
	pid->dout = pid->kd * (pid->err - pid->last_err);
	// 累加pid输出值
	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
	// 记录上次误差值
	pid->last_err = pid->err;
}

//void cascade_pid_ctrl(pid_ctrl_t *pid)
//{
//	pid_ctrl_t *outPid = pid;
//	pid_ctrl_t *inPid = outPid->next;
//	
//	// 外环PID
//	outPid->err = outPid->target - outPid->measure;
//	single_pid_ctrl(outPid);
//	// 内环期望 = 外环输出
//	inPid->target = outPid->out;
//	// 内环PID
//	inPid->err = inPid->target - inPid->measure;
//	single_pid_ctrl(inPid);
//}

//////////速度环////////////////////
void balan_locat_ctrl(pid_ctrl_t *pid)
{
	pid->err = pid->last_err * 0.7f + pid->target *0.3f;
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral,-pid->integral_max,pid->integral_max);
	pid->ki = pid->kp/200;
	
	pid->pout = KalmanFilter(&KAL_Pid[kal_locat_pout].info->Locat_speed_kal,
	                         pid->err * pid->kp);
	pid->iout = pid->integral * pid->ki;
	pid->out = pid->pout + pid->iout;
	
	pid->last_err = pid->err;
	pid->out = constrain(pid->out,-pid->out_max,pid->out_max);
}
//////////////转向环//////////////
void balan_turn_ctrl(pid_ctrl_t *pid)
{
	if(abs(pid->err)<=pid->blind_err)
		pid->err = 0;
	
	pid->deta_err = -imu_sensor.info->rate_yaw;
	pid->integral += rc_sensor.info->ch2 * 0.3;

	pid->pout = pid->err * pid->kp;
	pid->dout = pid->deta_err * pid->kd;
	pid->out = pid->pout + pid->dout;
	pid->out = constrain(pid->out,-pid->out_max,pid->out_max);
}


