/**
 * @file        launcher.c
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        20-November-2022
 * @brief       Launcher Control Center
 */

/* Includes ------------------------------------------------------------------*/
#include "launcher.h"

#include "rp_math.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Launcher_Init(void);
void Launcher_Ctrl(void);
void Launcher_SelfProtect(void);

void Launcher_GetInfo(void);
void Launcher_GetBaseInfo(void);
void Judge_GetInfo(void);
void Judge_GetSpeedInfo(void);
void Judge_AdaptFricSpeed(void);
void Judge_AdaptDialSpeed(void);
void Launcher_GetCtrlInfo(void);
void Launcher_GetRcState(void);
void Launcher_GetKeyState(void);
void Get_LauncherStatus(void);
void Fric_StatusCheck(void);
void Dial_StatusCheck(void);

void Launcher_MotorCtrl(void);
void Fric_Ctrl(void);
void Dial_Ctrl(void);
//void Fric_

void Launcher_SendOut(void);

void Launcher_Stop(void);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t    launcher_out[3];
uint8_t shoot_cnt_limit = 2;

// 发射机构设备
launcher_dev_t		launcher_dev = {
	.fricl_motor = &RM_motor[FRIC_L],
	.fricr_motor = &RM_motor[FRIC_R],
	.dial_motor = &RM_motor[DIAL],
	.rc_sensor = &rc_sensor,
};

// 发射机构信息
launcher_info_t 	launcher_info = {
	.remote_mode = RC,
};

launcher_work_info_t  launcher_work_info = {
	.fric_status = WaitCommond_Fric,
	.dial_status = WaitCommond_Dial,
	.launcher_commond.Fric_cmd = Fric_Reset,
	.launcher_commond.Magz_cmd = Magz_Reset,
	.launcher_commond.Dial_cmd = Shoot_Reset,
	.shoot_cnt = 0,
	.swift_enable = 1,
};

launcher_conf_t   launcher_conf = {
	.Fric_15 = Fric_15_init,
	.Fric_18 = Fric_18_init,
	.Fric_20 = Fric_20_init,
	.Fric_22 = Fric_22_init,
	.Fric_30 = Fric_30_init,
	.dial_speed = -4000.0f,
	.dial_swiftspeed = -6750.0f,
	.dial_torque_limit = 2000.0f,
	.lock_angle_check = 0.5f,
	.Back_Angle = 45.0f,
	.Load_Angle = -45.0f,
//	.wait_time = 1000,  //发射间隔时间，单位ms
};

/* Exported variables --------------------------------------------------------*/
extern int16_t can2_send_buf[8];

launcher_t launcher = {
	.dev = &launcher_dev,
	.info = &launcher_info,
	.work_info = &launcher_work_info,
	.conf = &launcher_conf,
	.init = Launcher_Init,
	.ctrl = Launcher_Ctrl,
	.self_protect = Launcher_SelfProtect,
};

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  发射机构初始化
  * @param  
  * @retval 
  */
void Launcher_Init(void)
{
	if (rc_sensor.work_state == DEV_ONLINE)
	{
		launcher.info->init_s2 = rc_sensor.info->s2;
		launcher.info->last_s2 = rc_sensor.info->s2;
	}
	
	launcher.conf->fric_speed = launcher.conf->Fric_15;
	launcher.conf->fric_mode = 15;
	
	launcher.info->rc_work_state = rc_sensor.work_state;
	
}


/**
  * @brief  发射机构控制
  * @param  
  * @retval 
  */
void Launcher_Ctrl(void)
{
	Launcher_GetInfo();
	
	Launcher_MotorCtrl();
	
	Launcher_SendOut();
}


/**
  * @brief  发射机构控制离线保护
  * @param  
  * @retval 
  */
void Launcher_SelfProtect(void)
{
	Launcher_Stop();
	Launcher_GetInfo();
}


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  读取发射机构相关信息
  * @param  
  * @retval 
  */
void Launcher_GetInfo(void)
{
	Launcher_GetBaseInfo();
	
	Launcher_GetCtrlInfo();
	
	Get_LauncherStatus();
}


/**
  * @brief  读取发射机构电机信息
  * @param  
  * @retval 
  */
void Launcher_GetBaseInfo(void)
{
	launcher.info->measure_left_speed = RM_motor[FRIC_L].rx_info.speed;
	launcher.info->measure_right_speed = RM_motor[FRIC_R].rx_info.speed;
	launcher.info->measure_dial_speed = RM_motor[DIAL].rx_info.speed;
	launcher.info->measure_dial_angle = RM_motor[DIAL].rx_info.angle_sum * M2006_ECD_TO_ANGLE;
	
	/* 读取视觉打弹信息 */
	if ((vision_sensor.work_state == DEV_ONLINE) && (status.gim_mode == vision) && \
			(vision_sensor.info->is_hit_enable == 1) && (status.lch_state.fric_state == fric_on))
	{
		launcher.info->vision_shoot_cnt = vision_sensor.info->is_spin;
		if ((launcher.info->last_vision_shoot_cnt != launcher.info->vision_shoot_cnt) && (launcher.info->shoot_cnt < shoot_cnt_limit))
			launcher.info->shoot_cnt ++;
		launcher.info->last_vision_shoot_cnt = launcher.info->vision_shoot_cnt;
	}
	else
	{
		launcher.info->shoot_cnt = 0;
//		launcher.info->last_vision_shoot_cnt = vision_sensor.info->is_spin;
	}
	
	Judge_GetInfo();
}

/**
  * @brief  读取裁判系统信息
  * @param  
  * @retval 
  */
void Judge_GetInfo(void)
{
	Judge_GetSpeedInfo();
	
	Judge_AdaptFricSpeed();
	
	Judge_AdaptDialSpeed();
	
}


/**
  * @brief  读取限制弹速与测量弹速
  * @param  
  * @retval 
  */
void Judge_GetSpeedInfo(void)
{
	launcher.info->limit_speed = (float)judge.info->game_robot_status.shooter_id1_17mm_speed_limit;
	launcher.info->measure_launcher_speed = judge.info->shoot_data.bullet_speed;
	
	if (judge.work_state == DEV_OFFLINE)
	{
		launcher.conf->fric_speed = launcher.conf->Fric_30;
		launcher.conf->fric_mode = 30;
	}
	else
	{
		switch (judge.info->game_robot_status.shooter_id1_17mm_speed_limit)
		{
			case 0:
				launcher.conf->fric_speed = 0;
				launcher.conf->fric_mode = 0;
				break;
			case 15:
				launcher.conf->fric_speed = launcher.conf->Fric_15;
				launcher.conf->fric_mode = 15;
				break;
			case 18:
				launcher.conf->fric_speed = launcher.conf->Fric_18;
				launcher.conf->fric_mode = 18;
				break;
			case 20:
				launcher.conf->fric_speed = launcher.conf->Fric_20;
				launcher.conf->fric_mode = 20;
				break;
			case 22:
				launcher.conf->fric_speed = launcher.conf->Fric_22;
				launcher.conf->fric_mode = 22;
				break;
			case 30:
				launcher.conf->fric_speed = launcher.conf->Fric_30;
				launcher.conf->fric_mode = 30;
				break;
			default:
				launcher.conf->fric_speed = 0;
				launcher.conf->fric_mode = 0;
				break;
		}
	}
}


/**
  * @brief  调整摩擦轮转速
  * @param  
  * @retval 
  */
int8_t cnt = 0, low_cnt = 0;
float last_measure_speed = 0.f;
void Judge_AdaptFricSpeed(void)
{
	static int8_t speed_adapt = 0, adapt_k = 5;
	
	if (judge.work_state == DEV_ONLINE)
	{
		if ((launcher.info->measure_launcher_speed != last_measure_speed) && (launcher.info->measure_launcher_speed > 0.f))
		{
			if (launcher.info->measure_launcher_speed > (launcher.info->limit_speed - 0.15f)) //0.45f
			{
				low_cnt = 0;
				cnt = 0;
				speed_adapt = -15;
			}
			else if (launcher.info->measure_launcher_speed > (launcher.info->limit_speed - 1.05f))
			{
				low_cnt = 0;
				cnt++;
				if (cnt >= 2)
				{
					cnt = 0;
					speed_adapt = -1;
				}
			}
			else if (launcher.info->measure_launcher_speed < (launcher.info->limit_speed - 2.45f))
			{
				low_cnt++;
//				cnt = 0;
				if (low_cnt >= 3)
				{
					low_cnt = 0;
					speed_adapt = 15;
				}
			}
			else if (launcher.info->measure_launcher_speed < (launcher.info->limit_speed - 1.95f))
			{
				low_cnt = 0;
				cnt--;
				if (cnt <= -2)
				{
					cnt = 0;
					speed_adapt = 1;
				}
			}
			else
			{
				low_cnt = 0;
				cnt = 0;
				speed_adapt = 0;
			}
			launcher.conf->fric_speed = launcher.conf->fric_speed + speed_adapt * adapt_k;
			
			switch (judge.info->game_robot_status.shooter_id1_17mm_speed_limit)
			{
				case 0:
					launcher.conf->fric_speed = 0;
					break;
				case 15:
					launcher.conf->Fric_15 = launcher.conf->fric_speed;
					break;
				case 18:
					launcher.conf->Fric_18 = launcher.conf->fric_speed;
					break;
				case 20:
					launcher.conf->Fric_20 = launcher.conf->fric_speed;
					break;
				case 22:
					launcher.conf->Fric_22 = launcher.conf->fric_speed;
					break;
				case 30:
					launcher.conf->Fric_30 = launcher.conf->fric_speed;
					break;
				default:
					launcher.conf->fric_speed = 0;
					break;
			}
		}
		last_measure_speed = launcher.info->measure_launcher_speed;
	}
	else
		last_measure_speed = 0.f;
	
}


/**
  * @brief  调整拨盘转速
  * @param  
  * @retval 
  */
uint8_t heat_low = 0, heat_high = 0;
void Judge_AdaptDialSpeed(void)
{
	static uint8_t judge_cnt = 0;
	static uint16_t last_measure_heat = 0;
	
	last_measure_heat = launcher.info->measure_launcher_heat;
	launcher.info->limit_heat = judge.info->game_robot_status.shooter_id1_17mm_cooling_limit;
	launcher.info->measure_launcher_heat = judge.info->power_heat_data.shooter_id1_17mm_cooling_heat;
	launcher.info->remain_heat = launcher.info->limit_heat - launcher.info->measure_launcher_heat;
	// 一次打7颗,多了会拨空枪
	if (launcher.info->remain_heat > 80)
		launcher.info->remain_heat = 80;
	// 裁判系统未更新时不打弹
	if (launcher.work_info->swift_enable == 0)
	{
		if (launcher.info->measure_launcher_heat == last_measure_heat)
		{
			launcher.info->remain_heat = 0;
		}
		else
		{
			judge_cnt = 0;
			launcher.work_info->swift_enable = 1;
		}
		
		if (++ judge_cnt >= 100)
		{
			judge_cnt = 0;
			launcher.work_info->swift_enable = 1;
		}
	}
	
	if (status.heat_mode == heat_limit_off)
		launcher.info->measure_launcher_heat = 0;
	
	if ((launcher.info->limit_heat - launcher.info->measure_launcher_heat) > 50)
	{
		heat_low = 1;
	}
	else 
	{
		heat_low = 0;
	}
	if ((launcher.info->limit_heat - launcher.info->measure_launcher_heat) <= 20)
	{
		heat_high = 1;
	}
	else
	{
		heat_high = 0;
	}
	
	
	if (judge.work_state == DEV_OFFLINE)
	{
		launcher.conf->dial_speed = -3000.0f;
		//test
		launcher.info->limit_heat = 40;
		launcher.info->measure_launcher_heat = 0;
		launcher.info->remain_heat = launcher.info->limit_heat - launcher.info->measure_launcher_heat;
	}
	else
	{
		if (heat_low == 1)
		{
			launcher.conf->dial_speed = -4000.0f;
		}
		else
		{
			launcher.conf->dial_speed = -3000.0f;
		}
		if (heat_high == 1)
		{
			launcher.conf->dial_speed = -0.0f;
			status.lch_cmd.shoot_cmd = shoot_reset;
		}
		else
		{
			launcher.conf->dial_speed = -3000.0f;
		}
	}
}

/**
  * @brief  读取遥控器控制信息
  * @param  
  * @retval 
  */
void Launcher_GetCtrlInfo(void)
{
	if (launcher.info->remote_mode == RC)
	{
		Launcher_GetRcState();
	}
	else if(launcher.info->remote_mode == KEY) {
		Launcher_GetKeyState();
	}
}


/**
  * @brief  读取遥控器信息
  * @param  
  * @retval 
  */
									int16_t shoot_cnt;
void Launcher_GetRcState(void)
{
	/**    读取遥控器上电时刻信息    **/
	if ((launcher.info->rc_work_state == DEV_OFFLINE) \
		&& (rc_sensor.work_state == DEV_ONLINE))
	{
		launcher.info->init_s2 = rc_sensor.info->s2;
		launcher.info->last_s2 = rc_sensor.info->s2;
	}
	
	
	if (rc_sensor.work_state == DEV_ONLINE)
	{
		/**    模式切换保护    **/
		if (launcher.info->last_s1 != rc_sensor.info->s1)
		{
			launcher.info->init_s2 = rc_sensor.info->s2;
		}
		
		/**    遥控器上电保护    **/
		if (launcher.info->init_s2 == rc_sensor.info->s2)
		{
			launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
			launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
			launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
		}
		else
		{
			launcher.info->init_s2 = 0;
			
			/**    读取遥控器拨杆跳变信息    **/
			if (launcher.info->launcher_mode == lch_gyro)
			{
				
				if (rc_sensor.info->s2 == RC_SW_MID)
				{
					status.lch_cmd.shoot_cmd = shoot_reset;
					/* 跳变开关摩擦轮 */
					launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
					if (launcher.info->last_s2 == RC_SW_DOWN)
						launcher.work_info->launcher_commond.Fric_cmd = Fric_Toggle;
					
					/* 清空热量 */
					if (status.lch_state.shoot_state != swift_shoot)
						launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
					if (status.lch_cmd.shoot_cmd == swift_shoot)
					{
						/*  判断裁判系统数据是否更新  */
						if (launcher.work_info->swift_enable == 1)
						{
							if (status.lch_state.shoot_state != swift_shoot)
							{
								status.lch_state.shoot_state = swift_shoot;
								launcher.work_info->launcher_commond.Dial_cmd = Swift_Shoot;
								launcher.info->target_dial_speed = launcher.conf->dial_swiftspeed;
								launcher.info->target_heat_angle = launcher.info->measure_dial_angle + (launcher.info->remain_heat/10 - 1) * launcher.conf->Load_Angle;
							}
						}
					}
				}
				else if (rc_sensor.info->s2 == RC_SW_UP)
				{
					/* 遥控器连发 */
					if (status.gim_mode != vision)
					{
						status.lch_cmd.shoot_cmd = keep_shoot;
						if (status.lch_state.shoot_state != keep_shoot)
						{
							status.lch_state.shoot_state = keep_shoot;
							launcher.work_info->launcher_commond.Dial_cmd = Keep_Shoot;
						}
					}
					else
					{
						/*  遥控器自动打弹  */
//						static uint16_t auto_step = 0;
//						if ((vision_sensor.work_state == DEV_ONLINE) && (vision_sensor.info->is_spin == 1) &&\
//								(status.gim_mode == vision) && (vision_sensor.info->is_hit_enable == 1) && (status.lch_state.fric_state == fric_on))
//						{
//							status.lch_cmd.shoot_cmd = keep_shoot;
//							launcher.work_info->launcher_commond.Dial_cmd = Keep_Shoot;
//							if (!auto_step)
//							{
//								auto_step = 1;
//								status.lch_cmd.shoot_cmd = single_shoot;
//								launcher.work_info->launcher_commond.Dial_cmd = Single_Shoot;
//								launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
//							}
//						}
//						else
//						{
//							auto_step = 0;
//							status.lch_cmd.shoot_cmd = shoot_reset;
//							launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
//						}
						/*  遥控器变频自动打弹  */
						if ((vision_sensor.work_state == DEV_ONLINE) && (status.gim_mode == vision) && \
							(vision_sensor.info->is_hit_enable == 1) && (status.lch_state.fric_state == fric_on) && (vision_sensor.info->shoot_type == 0))
						{
							/*  判断裁判系统数据是否更新  */
							if (launcher.work_info->swift_enable == 1)
							{
								if (status.lch_state.shoot_state != swift_shoot)
								{
									status.lch_state.shoot_state = swift_shoot;
									launcher.work_info->launcher_commond.Dial_cmd = Swift_Shoot;
									launcher.info->target_dial_speed = launcher.conf->dial_swiftspeed;
									if (launcher.info->shoot_cnt >= (launcher.info->remain_heat/10 - 1))
									{
										
										shoot_cnt = launcher.info->remain_heat/10 - 1;
										if (shoot_cnt < 0)
											shoot_cnt = 0;
										launcher.info->target_heat_angle = launcher.info->measure_dial_angle + shoot_cnt * launcher.conf->Load_Angle;
										launcher.info->shoot_cnt -= shoot_cnt;
									}
									else
									{
										launcher.info->target_heat_angle = launcher.info->measure_dial_angle + launcher.info->shoot_cnt * launcher.conf->Load_Angle;
										launcher.info->shoot_cnt = 0;
									}
								}
							}
						}
						/* 遥控器单发自动打弹 */
						if ((vision_sensor.work_state == DEV_ONLINE) && (status.gim_mode == vision) && (vision_sensor.info->is_hit_enable == 1) \
							&& (status.lch_state.fric_state == fric_on) && (heat_high == 0) && (vision_sensor.info->shoot_type == 1))
						{
							if (launcher.info->shoot_cnt > 0)
							{
								if (status.lch_state.shoot_state != single_shoot)
								{
									status.lch_state.shoot_state = single_shoot;
									launcher.work_info->launcher_commond.Dial_cmd = Single_Shoot;
									launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
									launcher.info->shoot_cnt--;
								}
							}
						}
					}


				}
				else if (rc_sensor.info->s2 == RC_SW_DOWN)
				{
					launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
				}
				
			}
			else if ((launcher.info->launcher_mode == lch_machine) || (launcher.info->launcher_mode == lch_gyro2))
			{
				
				if (rc_sensor.info->s2 == RC_SW_MID)
				{
					launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
					launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
				}
				else if (rc_sensor.info->s2 == RC_SW_UP)
				{
					status.lch_state.shoot_state = single_shoot;
					if (launcher.info->last_s2 != rc_sensor.info->s2)
					{
						launcher.work_info->launcher_commond.Dial_cmd = Single_Shoot;
						launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
					}
				}
				else if (rc_sensor.info->s2 == RC_SW_DOWN)
				{
					launcher.work_info->launcher_commond.Magz_cmd = Magz_Open;
				}
				
			}
			else 
			{
				/**    launcher_mode == lch_keep    **/
				launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
				launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
				launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
				launcher.work_info->fric_status = WaitCommond_Fric;
				launcher.work_info->dial_status = WaitCommond_Dial;
			}
		}
	}
	else if(launcher.info->rc_work_state == DEV_ONLINE)
	{
		/**    遥控器离线跳变信息    **/
		launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
		launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
		launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
		launcher.work_info->fric_status = Off_Fric;
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
	else 
	{
		/**    遥控器离线信息    **/
		launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
		launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
		launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
		launcher.work_info->fric_status = WaitCommond_Fric;
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
	
}


/**
  * @brief  读取键盘信息
  * @param  
  * @retval 
  */
void Launcher_GetKeyState(void)
{
	static uint16_t buff_cnt = 0;
	
	launcher.work_info->launcher_commond.Fric_cmd = Fric_Reset;
	launcher.work_info->launcher_commond.Magz_cmd = Magz_Reset;
	
	/*  遥控器切换目标测试  */
	if (rc_sensor.info->s2 == RC_SW_UP)
	{
		if (launcher.info->last_s2 != rc_sensor.info->s2)
			vision_sensor.info->tx_info->is_change_target ++;
	}
	
	/*  键盘自动打弹  */
	if ((vision_sensor.work_state == DEV_ONLINE) && (status.gim_mode == vision) && (vision_sensor.info->is_hit_enable == 1) \
		&& (status.lch_state.fric_state == fric_on) && (status.auto_cmd == auto_shoot_on) && (vision_sensor.info->shoot_type == 0))
	{
		/*  判断裁判系统数据是否更新  */
		if (launcher.work_info->swift_enable == 1)
		{
			if (status.lch_state.shoot_state != swift_shoot)
			{
				status.lch_state.shoot_state = swift_shoot;
				launcher.work_info->launcher_commond.Dial_cmd = Swift_Shoot;
				launcher.info->target_dial_speed = launcher.conf->dial_swiftspeed;
				if (launcher.info->shoot_cnt >= (launcher.info->remain_heat/10 - 1))
				{
					int16_t shoot_cnt;
					shoot_cnt = launcher.info->remain_heat/10 - 1;
					if (shoot_cnt < 0)
						shoot_cnt = 0;
					launcher.info->target_heat_angle = launcher.info->measure_dial_angle + shoot_cnt * launcher.conf->Load_Angle;
					launcher.info->shoot_cnt -= shoot_cnt;
				}
				else
				{
					launcher.info->target_heat_angle = launcher.info->measure_dial_angle + launcher.info->shoot_cnt * launcher.conf->Load_Angle;
					launcher.info->shoot_cnt = 0;
				}
			}
		}
	}
	else if ((vision_sensor.work_state == DEV_ONLINE) && (status.gim_mode == vision) && (vision_sensor.info->is_hit_enable == 1) \
		&& (status.lch_state.fric_state == fric_on) && (status.auto_cmd == auto_shoot_on) && (heat_high == 0) && (vision_sensor.info->shoot_type == 1))
	{
		if (launcher.info->shoot_cnt > 0)
		{
			if (status.lch_state.shoot_state != single_shoot)
			{
				status.lch_state.shoot_state = single_shoot;
				launcher.work_info->launcher_commond.Dial_cmd = Single_Shoot;
				launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
				launcher.info->shoot_cnt--;
			}
		}
	}
	
	/*  键盘自动打符  */
	if ((status.autobuff_cmd == auto_buff_on) && (vision_sensor.work_state == DEV_ONLINE) && (status.buff_cmd == small_buff_on) &&\
			(status.gim_mode == vision) && (vision_sensor.info->is_find_buff == 1) && (status.lch_state.fric_state == fric_on))
	{
		if (vision_sensor.info->is_hit_enable == 1)
			if (++buff_cnt >= 650)
			{
				buff_cnt = 0;
				status.lch_cmd.shoot_cmd = single_shoot;
			}
	}
	else if ((status.autobuff_cmd == auto_buff_on) && (vision_sensor.work_state == DEV_ONLINE) && (status.buff_cmd == big_buff_on) &&\
			(status.gim_mode == vision) && (vision_sensor.info->is_find_buff == 1) && (status.lch_state.fric_state == fric_on))
	{
		if (vision_sensor.info->is_hit_enable == 1)
			if (++buff_cnt >= 1000)
			{
				buff_cnt = 0;
				status.lch_cmd.shoot_cmd = single_shoot;
			}
	}
	else
	{
		buff_cnt = 0;
	}
	
	/*  拨盘指令  */
	if (status.lch_cmd.shoot_cmd == single_shoot)
	{
		if (status.lch_state.shoot_state != single_shoot)
		{
			status.lch_state.shoot_state = single_shoot;
			launcher.work_info->launcher_commond.Dial_cmd = Single_Shoot;
			launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
		}
	}
	if (status.lch_cmd.shoot_cmd == keep_shoot)
	{
		if ((status.lch_state.shoot_state != keep_shoot) && (status.lch_state.shoot_state != single_shoot))
		{
			status.lch_state.shoot_state = keep_shoot;
			launcher.work_info->launcher_commond.Dial_cmd = Keep_Shoot;
		}
	}
	if (status.lch_cmd.shoot_cmd == swift_shoot)
	{
		/*  判断裁判系统数据是否更新  */
		if (launcher.work_info->swift_enable == 1)
		{
			if ((status.lch_state.shoot_state != swift_shoot) && (status.lch_state.shoot_state != single_shoot))
			{
				status.lch_state.shoot_state = swift_shoot;
				launcher.work_info->launcher_commond.Dial_cmd = Swift_Shoot;
				launcher.info->target_dial_speed = launcher.conf->dial_swiftspeed;
				launcher.info->target_heat_angle = launcher.info->measure_dial_angle + (launcher.info->remain_heat/10 - 1) * launcher.conf->Load_Angle;
			}
		}
	}
	if (status.lch_cmd.shoot_cmd == shoot_reset)
	{
		if (status.lch_state.shoot_state == keep_shoot)
		{
			status.lch_state.shoot_state = shoot_reset;
			launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
		}
	}
	
	/*  弹仓指令  */
	if (status.lch_cmd.magz_cmd == magz_open)
	{
		launcher.work_info->launcher_commond.Magz_cmd = Magz_Open;
	}
	else if (status.lch_cmd.magz_cmd == magz_close)
	{
		launcher.work_info->launcher_commond.Magz_cmd = Magz_Close;
	}
	
	/*  摩擦轮指令  */
	if (status.lch_cmd.fric_cmd == fric_on)
	{
		launcher.work_info->launcher_commond.Fric_cmd = Fric_Open;
	}
	else if (status.lch_cmd.fric_cmd == fric_off)
	{
		launcher.work_info->launcher_commond.Fric_cmd = Fric_Close;
	}
	
}


/**
  * @brief  读取发射机构当前状态
  * @param  
  * @retval 
  */
void Get_LauncherStatus(void)
{
	Fric_StatusCheck();
	
	Dial_StatusCheck();
	
	launcher.info->rc_work_state = rc_sensor.work_state;
	launcher.info->last_s1 = rc_sensor.info->s1;
	launcher.info->last_s2 = rc_sensor.info->s2;
}



/**
  * @brief  摩擦轮开启跳变检测与弹仓盖开关
  * @param  
  * @retval 
  */
void Fric_StatusCheck(void)
{
	/*  遥控器开关摩擦轮  */
	if ((launcher.work_info->launcher_commond.Fric_cmd == Fric_Toggle) \
		&& (launcher.info->last_s2 != rc_sensor.info->s2))
	{
		if (launcher.work_info->fric_status == On_Fric)
		{
			launcher.work_info->fric_status = Off_Fric;
			status.lch_state.fric_state = fric_off;
		}
		else
		{
			launcher.work_info->fric_status = On_Fric;
		}
	}
	
	/*  键盘开关摩擦轮  */
	if (launcher.work_info->launcher_commond.Fric_cmd == Fric_Open)
	{
		launcher.work_info->fric_status = On_Fric;
	}
	if (launcher.work_info->launcher_commond.Fric_cmd == Fric_Close)
	{
		launcher.work_info->fric_status = Off_Fric;
		status.lch_state.fric_state = fric_off;
	}
	
	if (launcher.work_info->fric_status == On_Fric)
	{
		if((( (uint16_t)launcher.conf->fric_speed - launcher.info->measure_right_speed) < 200) && \
			 ((-(uint16_t)launcher.conf->fric_speed - launcher.info->measure_left_speed) > -200))
		{
			status.lch_state.fric_state = fric_on;
		}
	}
	
}


/**
  * @brief  更新拨盘控制信息
  * @param  
  * @retval 
  */
void Dial_StatusCheck(void)
{
	/*  电机离线检测  */
	if ((RM_motor[FRIC_L].state.work_state == M_OFFLINE) || (RM_motor[FRIC_R].state.work_state == M_OFFLINE) || (RM_motor[DIAL].state.work_state == M_OFFLINE))
	{
		launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
	}
	
	/**    摩擦轮状态检测    **/
	if (launcher.work_info->fric_status != On_Fric)
	{
		launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
	}
	
	/**    拨盘功能复位    **/
	if (launcher.work_info->launcher_commond.Dial_cmd == Shoot_Reset)
	{
		status.lch_state.shoot_state = shoot_reset;
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
	
	/*  拨盘未堵转  */
	if (launcher.work_info->dial_status != Lock_Dial)
	{
		/*  单发  */
		if (launcher.work_info->launcher_commond.Dial_cmd == Single_Shoot)
		{
			launcher.work_info->dial_status = Reload_Dial;
		}
		
		/*  连发  */
		if (launcher.work_info->launcher_commond.Dial_cmd == Keep_Shoot)
		{
			launcher.info->target_dial_speed = launcher.conf->dial_speed;
			launcher.work_info->dial_status = SpeedKeep_Dial;
		}
		
		/*  清空热量  */
		if (launcher.work_info->launcher_commond.Dial_cmd == Swift_Shoot)
		{
			if (launcher.info->remain_heat > 10)
			{
				launcher.work_info->dial_status = SwiftKeep_Dial;
			}
			else
			{
				launcher.info->target_dial_speed = 0.f;
				launcher.work_info->dial_status = WaitCommond_Dial;
			}
		}
		
			
		/*  拨盘堵转检测  */
		if (RM_motor[DIAL].ctr_stuck_flag(&RM_motor[DIAL], launcher.conf->dial_torque_limit))
		{
			launcher.work_info->dial_status = Lock_Dial;
			launcher.info->target_dial_angle = launcher.conf->Back_Angle + launcher.info->measure_dial_angle;
		}
		else
		{
			if (status.lch_state.shoot_state == shoot_reset)
				launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
			/*  单发完成后复位  */
			if (status.lch_state.shoot_state == single_shoot)
			{
				if (launcher.info->measure_dial_angle < (launcher.info->target_dial_angle + launcher.conf->lock_angle_check))
				{
					status.lch_state.shoot_state = shoot_reset;
					launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
				}
			}
			/*  清空热量完成后复位  */
			if (status.lch_state.shoot_state == swift_shoot)
			{
				if (launcher.info->measure_dial_angle < (launcher.info->target_heat_angle + launcher.conf->lock_angle_check))
				{
					launcher.work_info->swift_enable = 0;
					status.lch_state.shoot_state = shoot_reset;
					launcher.work_info->launcher_commond.Dial_cmd = Shoot_Reset;
				}
			}
		}
		
	}
	else
	{
		/*  拨盘堵转处理完成  */
		if (launcher.info->measure_dial_angle > launcher.info->target_dial_angle - launcher.conf->lock_angle_check)
		{
			if (status.lch_state.shoot_state == shoot_reset)
			{
				launcher.work_info->dial_status = WaitCommond_Dial;
			}
			if (status.lch_state.shoot_state == single_shoot)
			{
				launcher.work_info->dial_status = WaitCommond_Dial;
			}
			if (status.lch_state.shoot_state == keep_shoot)
			{
				launcher.work_info->dial_status = SpeedKeep_Dial;
			}
			if (status.lch_state.shoot_state == swift_shoot)
			{
				launcher.work_info->dial_status = SwiftKeep_Dial;
			}
		}
		/*  拨盘反转时堵转  */
		if (RM_motor[DIAL].ctr_stuck_flag(&RM_motor[DIAL], launcher.conf->dial_torque_limit))
		{
			if (status.lch_state.shoot_state == single_shoot)
			{
				launcher.work_info->dial_status = Reload_Dial;
				launcher.info->target_dial_angle = -launcher.conf->Back_Angle + launcher.info->measure_dial_angle;
			}
			if (status.lch_state.shoot_state == keep_shoot)
			{
				launcher.work_info->dial_status = SpeedKeep_Dial;
			}
			if (status.lch_state.shoot_state == swift_shoot)
			{
				launcher.work_info->dial_status = SwiftKeep_Dial;
			}
		}
	}
	
}



/**
  * @brief  发射机构电机控制
  * @param  
  * @retval 
  */
void Launcher_MotorCtrl(void)
{
	Fric_Ctrl();
	Dial_Ctrl();
}


/**
  * @brief  摩擦轮电机控制
  * @param  
  * @retval 
  */
void Fric_Ctrl(void)
{
	if (launcher.work_info->fric_status == On_Fric)
	{
		launcher.info->target_left_speed = -launcher.conf->fric_speed;
		launcher.info->target_right_speed = launcher.conf->fric_speed;
		
		if ((abs(launcher.info->target_left_speed - launcher.info->measure_left_speed) < 30) \
			&& (abs(launcher.info->target_right_speed - launcher.info->measure_right_speed) < 30))
		{
			RM_motor[FRIC_L].pid.speed.set.kp = 15.f;
			RM_motor[FRIC_L].pid.speed.set.ki = 0.5f;
			RM_motor[FRIC_L].pid.speed.set.kd = 0.5f;
			
			RM_motor[FRIC_R].pid.speed.set.kp = 15.f;
			RM_motor[FRIC_R].pid.speed.set.ki = 0.5f;
			RM_motor[FRIC_R].pid.speed.set.kd = 0.5f;
		}
		else
		{
			RM_motor[FRIC_L].pid.speed.set.kp = 20.f;
			RM_motor[FRIC_L].pid.speed.set.ki = 0.f;
			RM_motor[FRIC_L].pid.speed.set.kd = 20.f;
			
			RM_motor[FRIC_R].pid.speed.set.kp = 20.f;
			RM_motor[FRIC_R].pid.speed.set.ki = 0.f;
			RM_motor[FRIC_R].pid.speed.set.kd = 20.f;
		}
	
		launcher_out[RM_motor[FRIC_L].id.buff_p] = RM_motor[FRIC_L].ctr_speed(&RM_motor[FRIC_L], 
		                                        launcher.info->target_left_speed);
		
		launcher_out[RM_motor[FRIC_R].id.buff_p] = RM_motor[FRIC_R].ctr_speed(&RM_motor[FRIC_R], 
		                                        launcher.info->target_right_speed);
		
	}
	else if (launcher.work_info->fric_status == Off_Fric || launcher.work_info->fric_status == WaitCommond_Fric)
	{
		launcher.info->target_left_speed = 0.0f;
		launcher.info->target_right_speed = 0.0f;
		
		launcher_out[RM_motor[FRIC_L].id.buff_p] = RM_motor[FRIC_L].ctr_speed(&RM_motor[FRIC_L], 
		                                        launcher.info->target_left_speed);
		
		launcher_out[RM_motor[FRIC_R].id.buff_p] = RM_motor[FRIC_R].ctr_speed(&RM_motor[FRIC_R], 
		                                        launcher.info->target_right_speed);
		
	}
	
	/*  开关弹仓  */
	if (launcher.work_info->launcher_commond.Magz_cmd == Magz_Open)
	{
		Magazine_Open();
		launcher.work_info->fric_status = Off_Fric;
		status.lch_state.magz_state = magz_open;
		status.lch_cmd.fric_cmd = fric_off;
	}
	else
	{
		Magazine_Close();
		status.lch_state.magz_state = magz_close;
	}
	
}

/**
  * @brief  拨盘电机控制
  * @param  
  * @retval 
  */
void Dial_Ctrl(void)
{
	if ((launcher.work_info->dial_status == SpeedKeep_Dial) || (launcher.work_info->dial_status == SwiftKeep_Dial))
	{
		
		launcher_out[RM_motor[DIAL].id.buff_p] = RM_motor[DIAL].ctr_pid1(&RM_motor[DIAL].pid.position_in, 
		                                                         RM_motor[DIAL].rx_info.speed,
                                                      		   launcher.info->target_dial_speed);
		
	}
	else if (launcher.work_info->dial_status != WaitCommond_Dial)
	{
		// 过零点处理
		if (launcher.info->target_dial_angle < -360.0f)
		{
			launcher.info->target_dial_angle += 360.0f;
		}
		else if (launcher.info->target_dial_angle > 360.0f)
		{
			launcher.info->target_dial_angle -= 360.0f;
		}
		
		if (launcher.info->target_dial_angle - launcher.info->measure_dial_angle > 180.0f)
		{
			launcher.info->target_dial_angle -= 360.0f;
		}
		else if (launcher.info->target_dial_angle - launcher.info->measure_dial_angle < -180.0f)
		{
			launcher.info->target_dial_angle += 360.0f;
		}
		
		launcher_out[RM_motor[DIAL].id.buff_p] = RM_motor[DIAL].ctr_posit(&RM_motor[DIAL], 
		                                      launcher.info->target_dial_angle / M2006_ECD_TO_ANGLE);
		
	}
	else 
	{
		launcher_out[RM_motor[DIAL].id.buff_p] = RM_motor[DIAL].ctr_posit(&RM_motor[DIAL], 
		                                      launcher.info->measure_dial_angle / M2006_ECD_TO_ANGLE);
		
	}
	
}




/**
  * @brief  发送发射机构电机控制数据
  * @param  
  * @retval 
  */
void Launcher_SendOut(void)
{
	if(RM_motor[FRIC_L].state.work_state == M_ONLINE)
		can2_send_buf[RM_motor[FRIC_L].id.buff_p] = launcher_out[RM_motor[FRIC_L].id.buff_p];
	else
		can2_send_buf[RM_motor[FRIC_L].id.buff_p] = 0;
	
	if(RM_motor[FRIC_R].state.work_state == M_ONLINE)
		can2_send_buf[RM_motor[FRIC_R].id.buff_p] = launcher_out[RM_motor[FRIC_R].id.buff_p];
	else
		can2_send_buf[RM_motor[FRIC_R].id.buff_p] = 0;
	
	if(RM_motor[DIAL].state.work_state == M_ONLINE)
	{
		can2_send_buf[RM_motor[DIAL].id.buff_p] = launcher_out[RM_motor[DIAL].id.buff_p];
	}
	else
		can2_send_buf[RM_motor[DIAL].id.buff_p] = 0;
}



/**
  * @brief  发射机构卸力
  * @param  
  * @retval 
  */
void Launcher_Stop(void)
{
	launcher_out[0] = 0;
	launcher_out[1] = 0;
	launcher_out[2] = 0;
	
	can2_send_buf[0] = launcher_out[0];
	can2_send_buf[1] = launcher_out[1];
	can2_send_buf[2] = launcher_out[2];
	
	/*  弹仓卸力  */
	Magazine_Sleep();
	status.lch_state.magz_state = magz_reset;
	
}


