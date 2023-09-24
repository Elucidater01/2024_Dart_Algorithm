/**
 * @file        system_task.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        27-October-2020
 * @brief       Decision Center.
 */

/* Includes ------------------------------------------------------------------*/
#include "system_task.h"

#include "cmsis_os.h"
#include "rc_sensor.h"

#include "cmsis_armcc.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 int8_t  prev_sw2 = RC_SW_MID;				//开关设置标志
static uint8_t gyro2mech_pid_mode = false;	//切换许可标志

static uint8_t mouse_lock_R = false;				//按键锁定标志
static uint8_t rc_lock_SW1 = false;
static uint8_t key_lock_Ctrl = false;


/* Exported variables --------------------------------------------------------*/
//flag_t flag = {
//	.gimbal = {
//		.reset_start = false,
//		.reset_ok = false,
//	},
//};

system_t sys = {
	.remote_mode = RC,
	.state = SYS_STATE_RCLOST,
	.mode = SYS_MODE_NORMAL,
	.circle_mode.Stand_circle = ReSet,
	.circle_mode.Locat_circle = ReSet,
	.circle_mode.Turnaround_circle = ReSet,
	.circle_mode.FB_wheel = None,
};

/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	更新遥控器对系统模式的控制信息
 */
static void rc_update_info(void)
{
	if(sys.state != SYS_STATE_NORMAL) 
	{
		prev_sw2 = RC_SW_MID;	
		gyro2mech_pid_mode = false;
		
		mouse_lock_R = false;
		rc_lock_SW1 = false;
		key_lock_Ctrl = false;
	}
	else 
	{
		/* 遥控模式 */
		if(sys.remote_mode == RC) 
		{
			// 重置键盘模式下的参数
			mouse_lock_R = false;
			rc_lock_SW1 = false;
			key_lock_Ctrl = false;
		}
		if(!sys.init_stand_flag)//FB轮子初始化一次
		{
			sys.init_stand_flag = true;
			sys.circle_mode.FB_wheel = ReSet;
			sys.circle_mode.F_wheel = ReSet;
			sys.circle_mode.B_wheel = ReSet;
		}
	}
}

/**
 *	@brief	根据遥控器切换控制方式
 */
static void system_ctrl_mode_switch(void)
{
	uint8_t sw2 = RC_SW2_VALUE;
	
	if(sw2 == RC_SW_UP)	//键盘模式
	{
		if(prev_sw2 == RC_SW_MID) 
		{	
			gyro2mech_pid_mode = false;
			sys.remote_mode = KEY;
			sys.co_mode = CO_GYRO;
			// 刚切换过去的时候设置为常规模式
			sys.mode = SYS_MODE_NORMAL;	
		}
		else
		{
			
		}
		
	}
	else if(sw2 == RC_SW_MID)	//机械模式
	{
		sys.mode = SYS_MODE_NORMAL;	
		gyro2mech_pid_mode = false;
		sys.remote_mode = RC;
		sys.co_mode = CO_MECH;
		
		
		if(prev_sw2 != RC_SW_MID) //切换云台目标
		{	
			
//			Gimbal.Yaw_Motor.angle_turns = Gimbal.Yaw_Motor.angle_turns%2;
//			Gimbal.Yaw_Motor.tg_angle_turns = Gimbal.Yaw_Motor.angle_turns;
//			Gimbal.tg_PAngle = Gimbal.Pitch_Motor.angle;
//			Gimbal.tg_PTurns = Gimbal.Pitch_Motor.angle_turns;

//			Gimbal.Pitch_Motor.tg_angle = Gimbal.Pitch_Motor.angle;
//			Gimbal.Pitch_Motor.tg_angle_turns = Gimbal.Pitch_Motor.angle_turns;
			
		}

		
	}
	else if(sw2 == RC_SW_DOWN)	//陀螺仪模式
	{
		gyro2mech_pid_mode = false;
		sys.mode = SYS_MODE_NORMAL;
		sys.remote_mode = RC;
		sys.co_mode = CO_GYRO;
		
		//切换至云台模式首先应该要做的事情
			//初始化起始角度
//			Gimbal.Yaw_Motor.angle_turns = Gimbal.Yaw_Motor.angle_turns%2;
//			Gimbal.Yaw_Motor.tg_angle_turns = Gimbal.Yaw_Motor.angle_turns;
//			
//			Gimbal.tg_YAngle = Gimbal.YAngle;
//			Gimbal.tg_YTurns = Gimbal.YTurns;
//			
//			Gimbal.tg_PAngle = Gimbal.PAngle;
//			Gimbal.tg_PTurns = Gimbal.PTurns;
//			
//			Gimbal.Pitch_Motor.tg_angle = Gimbal.Pitch_Motor.angle;
//			Gimbal.Pitch_Motor.tg_angle_turns = Gimbal.Pitch_Motor.angle_turns;
		
	}
	
	
}

/**
 *	@brief	根据遥控器切换系统行为
 */
static void system_mode_act_switch(void)//键盘专用
{
	
}

static void system_state_machine(void)
{
	// 控制方式切换
	system_ctrl_mode_switch();
	// 系统模式切换(键盘模式下才允许切换)
	if(sys.remote_mode == KEY)
		system_mode_act_switch();
}

/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	系统决策任务
 */
void StartSystemTask(void const * argument)
{
	for(;;)
	{
		portENTER_CRITICAL();//进入临界区
		
		// 更新遥控信息，获取数据
		rc_update_info();
		
		//信息处理
		/* 遥控离线 */
		if(rc_sensor.work_state == DEV_OFFLINE) 
		{
			sys.state = SYS_STATE_RCLOST;
			RC_ResetData(&rc_sensor);
		} 
		/* 遥控在线 */
		else if(rc_sensor.work_state == DEV_ONLINE)
		{
			/* 遥控正常 */
			if(rc_sensor.errno == NONE_ERR) 
			{
				/* 失联恢复 */
				if(sys.state == SYS_STATE_RCLOST) 
				{
					// 可在此处同步云台复位标志位					
					// 系统参数复位
					sys.remote_mode = RC;
					sys.state = SYS_STATE_NORMAL;
					sys.mode = SYS_MODE_NORMAL;
				}
				
				// 可在此处等待云台复位后才允许切换状态
				system_state_machine();
			}
			/* 遥控错误 */
			else if(rc_sensor.errno == DEV_DATA_ERR) {
				sys.state = SYS_STATE_RCERR;
				//reset CPU
				__set_FAULTMASK(1);//关闭总中断
				NVIC_SystemReset();
			} else {
				sys.state = SYS_STATE_WRONG;
				//reset CPU
				__set_FAULTMASK(1);//关闭总中断
				NVIC_SystemReset();
			}
		}
		
		portEXIT_CRITICAL();//离开临界区
		
		osDelay(7);
	}
}
