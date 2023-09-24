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
 int8_t  prev_sw2 = RC_SW_MID;				//�������ñ�־
static uint8_t gyro2mech_pid_mode = false;	//�л���ɱ�־

static uint8_t mouse_lock_R = false;				//����������־
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
 *	@brief	����ң������ϵͳģʽ�Ŀ�����Ϣ
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
		/* ң��ģʽ */
		if(sys.remote_mode == RC) 
		{
			// ���ü���ģʽ�µĲ���
			mouse_lock_R = false;
			rc_lock_SW1 = false;
			key_lock_Ctrl = false;
		}
		if(!sys.init_stand_flag)//FB���ӳ�ʼ��һ��
		{
			sys.init_stand_flag = true;
			sys.circle_mode.FB_wheel = ReSet;
			sys.circle_mode.F_wheel = ReSet;
			sys.circle_mode.B_wheel = ReSet;
		}
	}
}

/**
 *	@brief	����ң�����л����Ʒ�ʽ
 */
static void system_ctrl_mode_switch(void)
{
	uint8_t sw2 = RC_SW2_VALUE;
	
	if(sw2 == RC_SW_UP)	//����ģʽ
	{
		if(prev_sw2 == RC_SW_MID) 
		{	
			gyro2mech_pid_mode = false;
			sys.remote_mode = KEY;
			sys.co_mode = CO_GYRO;
			// ���л���ȥ��ʱ������Ϊ����ģʽ
			sys.mode = SYS_MODE_NORMAL;	
		}
		else
		{
			
		}
		
	}
	else if(sw2 == RC_SW_MID)	//��еģʽ
	{
		sys.mode = SYS_MODE_NORMAL;	
		gyro2mech_pid_mode = false;
		sys.remote_mode = RC;
		sys.co_mode = CO_MECH;
		
		
		if(prev_sw2 != RC_SW_MID) //�л���̨Ŀ��
		{	
			
//			Gimbal.Yaw_Motor.angle_turns = Gimbal.Yaw_Motor.angle_turns%2;
//			Gimbal.Yaw_Motor.tg_angle_turns = Gimbal.Yaw_Motor.angle_turns;
//			Gimbal.tg_PAngle = Gimbal.Pitch_Motor.angle;
//			Gimbal.tg_PTurns = Gimbal.Pitch_Motor.angle_turns;

//			Gimbal.Pitch_Motor.tg_angle = Gimbal.Pitch_Motor.angle;
//			Gimbal.Pitch_Motor.tg_angle_turns = Gimbal.Pitch_Motor.angle_turns;
			
		}

		
	}
	else if(sw2 == RC_SW_DOWN)	//������ģʽ
	{
		gyro2mech_pid_mode = false;
		sys.mode = SYS_MODE_NORMAL;
		sys.remote_mode = RC;
		sys.co_mode = CO_GYRO;
		
		//�л�����̨ģʽ����Ӧ��Ҫ��������
			//��ʼ����ʼ�Ƕ�
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
 *	@brief	����ң�����л�ϵͳ��Ϊ
 */
static void system_mode_act_switch(void)//����ר��
{
	
}

static void system_state_machine(void)
{
	// ���Ʒ�ʽ�л�
	system_ctrl_mode_switch();
	// ϵͳģʽ�л�(����ģʽ�²������л�)
	if(sys.remote_mode == KEY)
		system_mode_act_switch();
}

/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	ϵͳ��������
 */
void StartSystemTask(void const * argument)
{
	for(;;)
	{
		portENTER_CRITICAL();//�����ٽ���
		
		// ����ң����Ϣ����ȡ����
		rc_update_info();
		
		//��Ϣ����
		/* ң������ */
		if(rc_sensor.work_state == DEV_OFFLINE) 
		{
			sys.state = SYS_STATE_RCLOST;
			RC_ResetData(&rc_sensor);
		} 
		/* ң������ */
		else if(rc_sensor.work_state == DEV_ONLINE)
		{
			/* ң������ */
			if(rc_sensor.errno == NONE_ERR) 
			{
				/* ʧ���ָ� */
				if(sys.state == SYS_STATE_RCLOST) 
				{
					// ���ڴ˴�ͬ����̨��λ��־λ					
					// ϵͳ������λ
					sys.remote_mode = RC;
					sys.state = SYS_STATE_NORMAL;
					sys.mode = SYS_MODE_NORMAL;
				}
				
				// ���ڴ˴��ȴ���̨��λ��������л�״̬
				system_state_machine();
			}
			/* ң�ش��� */
			else if(rc_sensor.errno == DEV_DATA_ERR) {
				sys.state = SYS_STATE_RCERR;
				//reset CPU
				__set_FAULTMASK(1);//�ر����ж�
				NVIC_SystemReset();
			} else {
				sys.state = SYS_STATE_WRONG;
				//reset CPU
				__set_FAULTMASK(1);//�ر����ж�
				NVIC_SystemReset();
			}
		}
		
		portEXIT_CRITICAL();//�뿪�ٽ���
		
		osDelay(7);
	}
}
