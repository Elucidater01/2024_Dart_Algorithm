/**
 * @file        module.c/.h
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        5-January-2023
 * @brief       Module Status Control
 * @update
 */

/* Includes ------------------------------------------------------------------*/
#include "module.h"

//#include "drv_tim.h"
//#include "can_protocol.h"
#include "chassis.h"
#include "launcher.h"
#include "gimbal.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void module_info_update(module_t *mod);
void Module_StateCheck(void);
void Vision_TxInfoUpdate(void);
void Rc_RxInfoCheck(void);
void Key_RxInfoCheck(void);
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
symbal_t symbal = {
	.gim_sym.reset_start = 1,
	.gim_sym.reset_ok = 0,
	.gim_sym.turn_start = 0,
	.gim_sym.turn_ok = 1,
	.gim_sym.turn_right = 0,
	.gim_sym.turn_left = 0,
	.rc_update = 0,
};

status_t status = {
	.lch_state.fric_state = fric_reset,
	.lch_state.magz_state = magz_reset,
	.lch_state.shoot_state = shoot_reset,
	.gim_state = gim_reset,
	.chas_cmd = chas_reset,
//	.chas_state = chas_reset,
	.gim_mode = gyro,
	.heat_mode = heat_limit_on,
	.speed_cmd = speed_reset,
	.auto_cmd = auto_shoot_off,
	.autobuff_cmd = auto_buff_off,
	.buff_cmd = buff_reset,
	.tw_last_step[RC_TB_UP] = 0,
	.tw_last_step[RC_TB_MU] = 0,
	.tw_last_step[RC_TB_MD] = 0,
	.tw_last_step[RC_TB_DN] = 0,
};

module_t module = {
	.symbal = &symbal,
	.status = &status,
	.heartbeat = module_info_update,
};

/* Private functions ---------------------------------------------------------*/
void module_info_update(module_t *mod)
{
	Module_StateCheck();
	Vision_TxInfoUpdate();
	
	if(module.state != MODULE_STATE_NORMAL) 
	{
		module.remote_mode = RC;
		module.mode = MODULE_MODE_RESET;
		module.dir = MODULE_DIR_ARMOR;
		gimbal.info->gimbal_mode = gim_keep;
		launcher.info->launcher_mode = lch_keep;
		symbal.gim_sym.reset_start = 1;
		symbal.gim_sym.reset_ok = 0;
		status.chas_cmd = chas_reset;
		status.speed_cmd = speed_reset;
		status.gim_mode = gyro;
		status.lch_cmd.fric_cmd = fric_reset;
		status.lch_cmd.shoot_cmd = shoot_reset;
	}
	else 
	{
		/*  ģʽ�л�  */
		if (rc_sensor.info->s1 == RC_SW_DOWN)
		{
			module.remote_mode = RC;
			module.mode = MODULE_MODE_MACHINE;
			gimbal.info->gimbal_mode = gim_machine;
			launcher.info->launcher_mode = lch_machine;
		}
		else if (rc_sensor.info->s1 == RC_SW_MID)
		{
			module.remote_mode = RC;
			module.mode = MODULE_MODE_GYRO;
			gimbal.info->gimbal_mode = gim_gyro;
			launcher.info->launcher_mode = lch_gyro;
		}
		else if (rc_sensor.info->s1 == RC_SW_UP)
		{
			module.remote_mode = KEY;
			module.mode = MODULE_MODE_GYRO;
			gimbal.info->gimbal_mode = gim_gyro;
			launcher.info->launcher_mode = lch_gyro;
		}
		else 
		{
			module.mode = MODULE_MODE_RESET;
			gimbal.info->gimbal_mode = gim_keep;
			launcher.info->launcher_mode = lch_keep;
		}
		
		if (status.gim_mode == vision)
		{
			gimbal.info->gimbal_mode = gim_vision;
		}
	}
	
	/*  ���Ʒ�ʽ�л�  */
	if (module.remote_mode == RC)
	{
		gimbal.info->remote_mode = RC;
		launcher.info->remote_mode = RC;
		chassis.info->remote_mode = RC;
		Rc_RxInfoCheck();
	}
	else if (module.remote_mode == KEY)
	{
		gimbal.info->remote_mode = KEY;
		launcher.info->remote_mode = KEY;
		chassis.info->remote_mode = KEY;
		Key_RxInfoCheck();
	}
	
	memcpy(status.tw_last_step, rc_sensor.info->thumbwheel.step, 4);
}

void Module_StateCheck(void)
{
	/*  ң����״̬���  */
	if(rc_sensor.work_state == DEV_OFFLINE) 
	{
		module.state = MODULE_STATE_RCLOST;
		RC_ResetData(&rc_sensor);
	} 
	/* ң������ */
	else if(rc_sensor.work_state == DEV_ONLINE)
	{
		/* ң������ */
		if(rc_sensor.errno == NONE_ERR) 
		{
			/* ʧ���ָ� */
			if(module.state == MODULE_STATE_RCLOST)
			{
				// ���ڴ˴�ͬ����̨��λ��־λ					
				// ϵͳ������λ
				module.remote_mode = RC;
				module.state = MODULE_STATE_RCINIT;
				module.mode = MODULE_MODE_RESET;
			}
			
			if ((module.state == MODULE_STATE_RCINIT) || (module.state == MODULE_STATE_KILLED))
			{
				if (RC_IsChannelReset())
				{
					module.state = MODULE_STATE_NORMAL;
					status.lch_cmd.magz_cmd = magz_close;
				}
			}
			
		}
	}
	
	/* Ѫ����� */
	if ((judge.info->remain_HP == 0) && (motor_offline_check() == 1))
	{
		module.state = MODULE_STATE_KILLED;
	}
	
	/*  ������״̬���  */
	if ((imu_sensor.work_state.err_code != IMU_NONE_ERR) && \
			(imu_sensor.work_state.err_code != IMU_DATA_CALI))
	{
		module.state = MODULE_STATE_IMUERR;
	}
}

void Vision_TxInfoUpdate(void)
{
	vision_sensor.info->measure_pitch_angle = imu_sensor.info->base_info.pitch * ANGLE_TO_ECD + (float)HALF_ECD_RANGE;
	vision_sensor.info->measure_yaw_angle = (imu_sensor.info->base_info.yaw + 180.f) * ANGLE_TO_ECD;
	vision_sensor.info->measure_shoot_speed = launcher.conf->fric_mode;
	
	vision_sensor.info->tx_info->pitch_angle = vision_sensor.info->measure_pitch_angle;
	vision_sensor.info->tx_info->yaw_angle = vision_sensor.info->measure_yaw_angle;
	vision_sensor.info->tx_info->shoot_speed = vision_sensor.info->measure_shoot_speed;
	vision_sensor.info->tx_info->my_color = judge.info->my_color;
	
	vision_sensor.info->tx_info->mode = AIM_ON;
	if (status.buff_cmd == small_buff_on)
	{
		vision_sensor.info->tx_info->mode = AIM_SMALL_BUFF;
	}
	if (status.buff_cmd == big_buff_on)
	{
		vision_sensor.info->tx_info->mode = AIM_BIG_BUFF;
	}
	vision_sensor.info->cmd_mode = vision_sensor.info->tx_info->mode;
	
}

void Rc_RxInfoCheck(void)
{
	/*  ң��������С����  */
	if (module.mode == MODULE_MODE_MACHINE)
	{
		status.chas_cmd = chas_reset;
	}
	if (module.mode == MODULE_MODE_GYRO)
	{
		if ((rc_sensor.info->thumbwheel.step[RC_TB_DN] == 0) && (status.tw_last_step[RC_TB_DN] == 1))
		{
			status.chas_cmd = chas_reset;
		}
		else if ((rc_sensor.info->thumbwheel.step[RC_TB_DN] == 1) && (status.tw_last_step[RC_TB_DN] == 0))
		{
			status.chas_cmd = gyro_on;
		}
	}
	/* ң�����ı���̨���� */
	if (module.mode == MODULE_MODE_GYRO)
	{
		if (rc_sensor.info->thumbwheel.step[RC_TB_UP] != status.tw_last_step[RC_TB_UP])
		{
			if (module.dir == MODULE_DIR_ARMOR)
			{
				module.dir = MODULE_DIR_WHEEL;
			}
			else
			{
				module.dir = MODULE_DIR_ARMOR;
			}
		}
	}
	/*  ң��������  */
	if (module.mode == MODULE_MODE_GYRO)
	{
		if ((rc_sensor.info->thumbwheel.step[RC_TB_MD] == 0) && (status.tw_last_step[RC_TB_MD] == 1))
		{
			status.speed_cmd = rapid_off;
		}
		else if ((rc_sensor.info->thumbwheel.step[RC_TB_MD] == 1) && (status.tw_last_step[RC_TB_MD] == 0))
		{
			status.speed_cmd = rapid_on;
		}
	}
	// ң�����Ӿ�
	if (module.mode == MODULE_MODE_MACHINE)
	{
		if ((rc_sensor.info->thumbwheel.step[RC_TB_UP] == 0) && (status.tw_last_step[RC_TB_UP] == 1))
		{
			status.gim_mode = gyro;
			gimbal.info->gimbal_mode = gim_gyro;
		}
		else if ((rc_sensor.info->thumbwheel.step[RC_TB_UP] == 1) && (status.tw_last_step[RC_TB_UP] == 0))
		{
			if (vision_sensor.work_state == DEV_ONLINE)
			{
				status.gim_mode = vision;
				gimbal.info->gimbal_mode = gim_vision;
			}
			else
			{
				status.gim_mode = gyro;
				gimbal.info->gimbal_mode = gim_gyro;
			}
		}
	}
//		if ((rc_sensor.info->thumbwheel.status == RC_TB_MID) && (status.tw_last_state == RC_TB_DOWN))
//		{
//			status.gim_mode = gyro;
//			gimbal.info->gimbal_mode = gim_gyro;
//		}
	/*  ң����������У��  */
	if (module.state == MODULE_STATE_IMUTMP)
	{
		if ((rc_sensor.info->s2 == RC_SW_DOWN) && (rc_sensor.info->thumbwheel.step[RC_TB_UP] != status.tw_last_step[RC_TB_UP]))
		{
			imu_sensor.work_state.err_code = IMU_DATA_CALI;
		}
		if ((rc_sensor.info->s2 != RC_SW_DOWN) && (imu_sensor.work_state.err_code != IMU_DATA_CALI))
		{
			module.state = MODULE_STATE_NORMAL;
		}
	}
	/*  �ص���  */
	if (status.chas_cmd == gyro_on)
	{
		status.lch_cmd.magz_cmd = magz_close;
	}
}

void Key_RxInfoCheck(void)
{
	Key_StateUpdate();
	
	status.lch_cmd.fric_cmd = fric_reset;
//	keyboard.lch_cmd.magz_cmd = lch_reset;
	status.lch_cmd.shoot_cmd = shoot_reset;
	status.heat_mode = heat_limit_on;
	status.gim_mode = gyro;
	/*  Ctrl(ͬʱ):�ش�� �ص��� �ز��� ��С���� �ػ�еģʽ  */
	if (keyboard.state.Ctrl == down_K)
	{
		status.buff_cmd = buff_reset;
		status.autobuff_cmd = auto_buff_off;
		status.chas_cmd = chas_reset;
		if (status.lch_state.magz_state == magz_open)
		{
			status.lch_cmd.magz_cmd = magz_close;
		}
		module.mode = MODULE_MODE_GYRO;
		gimbal.info->gimbal_mode = gim_gyro;
		launcher.info->launcher_mode = lch_gyro;
//		if (status.lch_state.fric_state == fric_on)
//		{
//			status.lch_cmd.fric_cmd = fric_off;
//		}
//		if (status.chas_state == gyro_on) 
//		{
//			status.chas_cmd = chas_reset;
//		}
	}
	/*  Q/E:תͷ90��  */
	if ((keyboard.state.Q == down_K) && (status.lch_state.magz_state == magz_close))
	{
		symbal.gim_sym.turn_left = 1;
		module.mode = MODULE_MODE_GYRO;
		gimbal.info->gimbal_mode = gim_gyro;
		launcher.info->launcher_mode = lch_gyro;
	}
	if ((keyboard.state.E == down_K) && (status.lch_state.magz_state == magz_close))
	{
		symbal.gim_sym.turn_right = 1;
		module.mode = MODULE_MODE_GYRO;
		gimbal.info->gimbal_mode = gim_gyro;
		launcher.info->launcher_mode = lch_gyro;
	}
	/*  R:����  */
	if (keyboard.state.R == down_K)
	{
		status.lch_cmd.fric_cmd = fric_on;
		status.lch_cmd.magz_cmd = magz_close;
	}
	if ((keyboard.state.R == down_K) && (status.lch_state.fric_state == fric_on))
	{
		status.lch_cmd.shoot_cmd = swift_shoot;
	}
	/*  F:С����  */
	if (keyboard.state.F == down_K)
	{
		status.chas_cmd = gyro_on;
		module.mode = MODULE_MODE_GYRO;
		gimbal.info->gimbal_mode = gim_gyro;
		launcher.info->launcher_mode = lch_gyro;
	}
	/*  G:��������  */
	if ((keyboard.state.G == short_press_K) || (keyboard.state.G == long_press_K))
	{
		status.heat_mode = heat_limit_off;
	}
	/*  X:С��  */
	if (keyboard.state.X == down_K)
	{
		status.buff_cmd = small_buff_on;
		status.gim_mode = vision;
	}
	/*  Z:���  */
	if (keyboard.state.Z == down_K)
	{
		status.buff_cmd = big_buff_on;
		status.gim_mode = vision;
	}
	if ((status.buff_cmd == big_buff_on) || (status.buff_cmd == small_buff_on))
	{
		status.gim_mode = vision;
	}
	/*  �Զ����  */
	if ((keyboard.state.X == down_K) || (keyboard.state.Z == down_K))
	{
		status.autobuff_cmd = auto_buff_on;
		status.lch_cmd.fric_cmd = fric_on;
		status.lch_cmd.magz_cmd = magz_close;
	}
	
//	if ((keyboard.state.X == up_K) || (keyboard.state.Z == up_K))
//	{
//		status.chas_cmd = chas_reset;
//		status.autobuff_cmd = auto_buff_off;
//	}
//	if ((keyboard.state.X == relax_K) && (keyboard.state.Z == relax_K))
//	{
//		status.autobuff_cmd = auto_buff_off;
//	}
	
	/*  ZX:����  */
//	if ((keyboard.state.X != relax_K) && (keyboard.state.Z != relax_K))
//	{
//		status.buff_cmd = debuff_on;
//	}
	
	/*  V:�л�Ŀ��  */
	if (keyboard.state.V == down_K)
	{
		vision_sensor.info->tx_info->is_change_target ++;
	}
	
	
	if (status.lch_state.fric_state == fric_on)
	{
		status.lch_cmd.magz_cmd = magz_close;
	}
	/*  B:������  */
	if ((keyboard.state.B == down_K) && (symbal.gim_sym.turn_ok == 1))
	{
		status.lch_cmd.magz_cmd = magz_open;
		status.lch_cmd.fric_cmd = fric_off;
	}
	/*  ����B:�ص���  */
	if (keyboard.state.B == long_press_K)
	{
		status.lch_cmd.magz_cmd = magz_close;
	}
	if (status.chas_cmd == gyro_on)
	{
		status.lch_cmd.magz_cmd = magz_close;
	}
	
	/*  mouse_btn_l  */
	if (keyboard.state.mouse_btn_l == down_K)
	{
		status.lch_cmd.fric_cmd = fric_on;
		status.lch_cmd.magz_cmd = magz_close;
	}
	if ((keyboard.state.mouse_btn_l == down_K) && (status.lch_state.fric_state == fric_on))
	{
		status.lch_cmd.shoot_cmd = single_shoot;
	}
	if ((keyboard.state.mouse_btn_l == long_press_K) && (status.lch_state.fric_state == fric_on))
	{
		status.lch_cmd.shoot_cmd = keep_shoot;
	}
	
	
	/*  ����Ctrl:��еģʽ  */
	if (keyboard.state.Ctrl == long_press_K)
	{
//		module.dir = MODULE_DIR_ARMOR;
		module.mode = MODULE_MODE_MACHINE;
		gimbal.info->gimbal_mode = gim_machine;
		launcher.info->launcher_mode = lch_machine;
	}
	/*  ����Shift:����  */
	status.speed_cmd = speed_reset;
	if (keyboard.state.Shift != relax_K)
		status.speed_cmd = rapid_on;
	
	status.gim_cmd = gim_reset;
	/*  C:��ͷ  */
	if ((keyboard.state.C == down_K) && (status.lch_state.magz_state == magz_close))
	{
		symbal.gim_sym.turn_start = 1;
		symbal.gim_sym.turn_ok = 0;
		status.gim_cmd = gim_turn;
		module.mode = MODULE_MODE_GYRO;
		gimbal.info->gimbal_mode = gim_gyro;
		launcher.info->launcher_mode = lch_gyro;
	}
	
	/*  mouse_btn_r:�Զ���  */
	if (keyboard.state.mouse_btn_r == down_K)
	{
		status.auto_cmd = auto_shoot_on;
//		status.lch_cmd.fric_cmd = fric_on;
		status.buff_cmd = buff_reset;
		status.autobuff_cmd = auto_buff_off;
	}
	if (keyboard.state.mouse_btn_r == relax_K)
	{
		status.auto_cmd = auto_shoot_off;
	}
	if (keyboard.state.mouse_btn_l != relax_K)
	{
		status.auto_cmd = auto_shoot_off;
		status.autobuff_cmd = auto_buff_off;
	}
	
	/*  mouse_btn_r:����  */
	if ((keyboard.state.mouse_btn_r == short_press_K) || (keyboard.state.mouse_btn_r == long_press_K))
	{
		status.gim_mode = vision;
	}
	
	/*  ZXV:�������ظ�λ  */
	if ((keyboard.state.Z != relax_K) && (keyboard.state.X != relax_K) && (keyboard.state.V != relax_K))
	{
			__set_FAULTMASK(1); 
			NVIC_SystemReset();
	}
	
	
	
	/*  ң�����������  */
//	if (module.mode == MODULE_MODE_GYRO)
//	{
//		status.lch_cmd.shoot_cmd = shoot_reset;
//		if (rc_sensor.info->thumbwheel.step[RC_TB_UP] != status.tw_last_step[RC_TB_UP])
//		{
//			if (status.lch_state.magz_state == magz_close)
//			{
//				status.lch_cmd.shoot_cmd = swift_shoot;
//			}
//		}
//	}
	
	/*  ң������λ  */
	if ((rc_sensor.info->s2 == RC_SW_UP) && (rc_sensor.info->thumbwheel.step[RC_TB_UP] != status.tw_last_step[RC_TB_UP]))
	{
			__set_FAULTMASK(1);
			NVIC_SystemReset();
	}
	
	/*  ң����������У��  */
	if ((rc_sensor.info->s2 == RC_SW_DOWN) && (rc_sensor.info->thumbwheel.step[RC_TB_DN] != status.tw_last_step[RC_TB_DN]))
	{
		module.state = MODULE_STATE_IMUTMP;
		imu_sensor.info->offset_info.gx_offset = 0.f;
		imu_sensor.info->offset_info.gy_offset = 0.f;
		imu_sensor.info->offset_info.gz_offset = 0.f;
	}
	
}

