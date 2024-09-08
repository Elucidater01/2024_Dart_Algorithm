#include "launcher.h"

int16_t    shoot_out[3];

void Shoot_Init(void);
void Shoot_Ctrl(void);
void Shoot_SelfProtect(void);

// 发射机构设备
shoot_dev_t		shoot_dev = {
	.fric_left = &fric_motor[FRIC_L],
	.fric_right = &fric_motor[FRIC_R],
	.dial_motor = &dial_motor,
	.rc_sensor = &rc_sensor,
};

// 发射机构信息
shoot_info_t 	shoot_info = {
	.remote_mode = RC,
};

shoot_work_info_t  shoot_work_info = {
	.fric_status = WaitCommond_Fric,
	.dial_status = WaitCommond_Dial,
	.launcher_commond = WaitCommond_L,
	.lock_cnt = 0,
};

shoot_conf_t   shoot_conf = {
	.fric_speed = Fric_15,
	.dial_speed = -2500.0f,
	.lock_angle_check = 1.5f,
	.lock_cnt = 50,
	.Back_Angle = 45.0f,
	.Load_Angle = -45.0f,
};

launcher_t launcher = {
	.dev = &shoot_dev,
	.info = &shoot_info,
	.work_info = &shoot_work_info,
	.conf = &shoot_conf,
	.init = Shoot_Init,
	.ctrl = Shoot_Ctrl,
	.self_protect = Shoot_SelfProtect,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  发射机构初始化
  * @param  
  * @retval 
  */
void Shoot_Init(void)
{
	if (rc_sensor.work_state == DEV_ONLINE)
	{
		launcher.info->init_s2 = rc_sensor.info->s2;
		launcher.info->last_s2 = rc_sensor.info->s2;
	}
	
	fric_motor[FRIC_L].init(&fric_motor[FRIC_L]);
	fric_motor[FRIC_R].init(&fric_motor[FRIC_R]);
	dial_motor.init(&dial_motor);
	launcher.info->rc_work_state = rc_sensor.work_state;
}



void Shoot_GetInfo(void);
void Launcher_Ctrl(void);
void Launcher_SendOut(void);
/**
  * @brief  发射机构控制
  * @param  
  * @retval 
  */
void Shoot_Ctrl(void)
{
	Shoot_GetInfo();
	
	Launcher_Ctrl();
	
	Launcher_SendOut();
}



void Shoot_GetBaseInfo(void);
void Shoot_GetCtrlInfo(void);
void Get_LauncherStatus(void);
/**
  * @brief  读取发射机构相关信息
  * @param  
  * @retval 
  */
void Shoot_GetInfo(void)
{
	Shoot_GetBaseInfo();
	
	Shoot_GetCtrlInfo();
	
	Get_LauncherStatus();
}



void Judge_GetInfo(void);

void Shoot_GetBaseInfo(void)
{
	launcher.info->measure_left_speed = fric_motor[FRIC_L].info->speed_rpm;
	launcher.info->measure_right_speed = fric_motor[FRIC_R].info->speed_rpm;
	launcher.info->measure_dial_speed = dial_motor.info->speed_rpm;
	launcher.info->measure_dial_angle = dial_motor.info->angle;
	
	Judge_GetInfo();
}



void Judge_GetSpeedInfo(void);
void Judge_AdaptFricSpeed(void);
void Judge_AdaptDialSpeed(void);

void Judge_GetInfo(void)
{
	Judge_GetSpeedInfo();
	
	Judge_AdaptFricSpeed();
	
	Judge_AdaptDialSpeed();
	
}

void Judge_GetSpeedInfo(void)
{
	launcher.info->limit_speed = (float)judge.info->game_robot_status.shooter_id1_17mm_speed_limit;
	launcher.info->measure_shoot_speed = judge.info->shoot_data.bullet_speed;
	
	if (judge.work_state == DEV_OFFLINE)
	{
		launcher.conf->fric_speed = Fric_15;
	}
	else
	{
		switch (judge.info->game_robot_status.shooter_id1_17mm_speed_limit)
		{
			case 0:
				launcher.conf->fric_speed = 0;
				break;
			case 15:
				launcher.conf->fric_speed = Fric_15;
				break;
			case 18:
				launcher.conf->fric_speed = Fric_18;
				break;		
			case 20:
				launcher.conf->fric_speed = Fric_20;
				break;	
			case 22:
				launcher.conf->fric_speed = Fric_22;
				break;			
			case 30:
				launcher.conf->fric_speed = Fric_30;
				break;	
			default:
				launcher.conf->fric_speed = 0;
				break;
		}
	}
}

void Judge_AdaptFricSpeed(void)
{
	static int8_t cnt = 0, speed_adapt = 0, adapt_k = 5;
	
	if (launcher.info->measure_shoot_speed > (launcher.info->limit_speed - 0.95f))
	{
		cnt++;
		if (cnt > 2)
		{
			cnt = 0;
			speed_adapt = 1;
		}
	}
	else if (launcher.info->measure_shoot_speed < (launcher.info->limit_speed - 1.95f))
	{
		cnt--;
		if (cnt < -2)
		{
			cnt = 0;
			speed_adapt = -1;
		}
	}
	else
	{
		cnt = 0;
		speed_adapt = 0;
	}
	
	launcher.conf->fric_speed = launcher.conf->fric_speed + speed_adapt * adapt_k;
}

void Judge_AdaptDialSpeed(void)
{
	static uint8_t heat_low = 0, heat_high = 0;
	
	launcher.info->limit_heat = judge.info->game_robot_status.shooter_id1_17mm_cooling_limit;
	launcher.info->measure_shoot_heat = judge.info->power_heat_data.shooter_id1_17mm_cooling_heat;
	
	if ((launcher.info->limit_heat - launcher.info->measure_shoot_heat) > 50)
	{
		heat_low = 1;
	}
	else 
	{
		heat_low = 0;
	}
	if ((launcher.info->limit_heat - launcher.info->measure_shoot_heat) < 20)
	{
		heat_high = 1;
	}
	else
	{
		heat_high = 0;
	}
	
	
	if (judge.work_state == DEV_OFFLINE)
	{
		launcher.conf->Load_Angle = -5.0f;
	}
	else
	{
		if (heat_low == 1)
		{
			launcher.conf->Load_Angle = -45.0f;
		}
		else
		{
			launcher.conf->Load_Angle = -22.5f;
		}
		if (heat_high == 1)
		{
			launcher.conf->Load_Angle = -0.0f;
		}
		else
		{
			launcher.conf->Load_Angle = -22.5f;
		}
	}
}




void Shoot_GetRcState(void);
void Shoot_GetKeyState(void);

void Shoot_GetCtrlInfo(void)
{
	if (sys.remote_mode == RC)
	{
		launcher.info->remote_mode = RC;
	}
	else if (sys.remote_mode == KEY)
	{
		launcher.info->remote_mode = KEY;
	}
	
	if (launcher.info->remote_mode == RC)
	{
		Shoot_GetRcState();
	}
	else if (launcher.info->remote_mode == KEY)
	{
		Shoot_GetKeyState();
	}
}

void Shoot_GetRcState(void)
{
	if ((launcher.info->rc_work_state == DEV_OFFLINE)
		&& rc_sensor.work_state == DEV_ONLINE)
	{
		launcher.info->init_s2 = rc_sensor.info->s2;
		launcher.info->last_s2 = rc_sensor.info->s2;
	}
	
	if (rc_sensor.work_state == DEV_ONLINE)
	{
		if (launcher.info->init_s2 == rc_sensor.info->s2)
		{
			launcher.work_info->launcher_commond = WaitCommond_L;
		}
		else
		{
			launcher.info->init_s2 = 0;
			if (gimbal.info->gimbal_mode == gim_gyro)
			{
				if (rc_sensor.info->s2 == RC_SW_MID)
				{
					launcher.work_info->launcher_commond = Func_Reset;
				}
				else if (rc_sensor.info->s2 == RC_SW_DOWN)
				{
					launcher.work_info->launcher_commond = Fric_Toggle;
				}
				else if (rc_sensor.info->s2 == RC_SW_UP)
				{
					launcher.work_info->launcher_commond = Keep_Shoot;
					if (launcher.info->last_s2 != rc_sensor.info->s2)
					{
						launcher.work_info->dial_status = Reload_Dial;
						launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
					}
				}
			}
			else if (gimbal.info->gimbal_mode == gim_machine)
			{
				if (rc_sensor.info->s2 == RC_SW_MID)
				{
					launcher.work_info->launcher_commond = Func_Reset;
				}
				else if (rc_sensor.info->s2 == RC_SW_DOWN)
				{
					launcher.work_info->launcher_commond = Magz_Open;
				}
				else if (rc_sensor.info->s2 == RC_SW_UP)
				{
					launcher.work_info->launcher_commond = Single_Shoot;
					if (launcher.info->last_s2 != rc_sensor.info->s2)
					{
						launcher.work_info->dial_status = Reload_Dial;
						launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
					}
				}
			}
		}
	}
	else 
	{
		launcher.work_info->launcher_commond = WaitCommond_L;
		launcher.work_info->fric_status = WaitCommond_Fric;
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
}




void Key_UpdateState(void);

void Shoot_GetKeyState(void)
{
	if (rc_sensor.work_state == DEV_ONLINE)
	{
		Key_UpdateState();
	}
	else 
	{
		launcher.work_info->launcher_commond = WaitCommond_L;
		launcher.work_info->fric_status = WaitCommond_Fric;
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
}

void Key_UpdateState(void)
{
	if (keyboard.Mouse_L.state == short_down)
	{
		if (launcher.work_info->fric_status == On_Fric)
		{
			launcher.work_info->launcher_commond = Single_Shoot;
			if (keyboard.Mouse_L.last_state != keyboard.Mouse_L.state)
			{
				launcher.work_info->dial_status = Reload_Dial;
				launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
			}
		}
	}
	if (keyboard.Mouse_L.state == down)
	{
		if (launcher.work_info->fric_status == On_Fric)
		{
			launcher.work_info->launcher_commond = Keep_Shoot;
			if (keyboard.Mouse_L.last_state != keyboard.Mouse_L.state)
			{
				launcher.work_info->dial_status = Reload_Dial;
				launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
			}
		}
	}
	if (keyboard.Mouse_L.state == up)
	{
		launcher.work_info->launcher_commond = Func_Reset;
	}
	
	if (keyboard.Mouse_L.state == press)
	{
		if (launcher.work_info->fric_status != On_Fric)
		{
			launcher.work_info->fric_status = On_Fric;
		}
	}
	
	if (keyboard.B.state == press)
	{
		launcher.work_info->fric_status = Off_Fric;
		launcher.work_info->launcher_commond = Magz_Open;
	}
	if (keyboard.B.state == down)
	{
		launcher.work_info->launcher_commond = Magz_Close;
	}
}




void Fric_StepCheck(void);
void Dial_StatusCheck(void);

void Get_LauncherStatus(void)
{
	Fric_StepCheck();
	
	Dial_StatusCheck();
	
}

// 摩擦轮跳变检测
void Fric_StepCheck(void)
{
	if ((launcher.work_info->launcher_commond == Fric_Toggle)
		&& (launcher.info->last_s2 != rc_sensor.info->s2))
	{
		if (launcher.work_info->fric_status == WaitCommond_Fric)
		{
			launcher.work_info->fric_status = On_Fric;
		}
		else if (launcher.work_info->fric_status == On_Fric)
		{
			launcher.work_info->fric_status = Off_Fric;
		}
		else if (launcher.work_info->fric_status == Off_Fric)
		{
			launcher.work_info->fric_status = On_Fric;
		}
	}
}

// 拨盘堵转检测
void Dial_StatusCheck(void)
{
	if (launcher.work_info->dial_status == Reload_Dial)
	{
		if (launcher.info->measure_dial_angle < launcher.info->target_dial_angle + launcher.conf->lock_angle_check)
		{
			if (launcher.work_info->launcher_commond == Keep_Shoot)
			{
				launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
			}
			else if (launcher.work_info->launcher_commond == Single_Shoot)
			{
				launcher.work_info->dial_status = WaitCommond_Dial;
			}
		}
		else 
		{
			if (abs(launcher.info->measure_dial_speed) < 5)
			{
				launcher.work_info->lock_cnt ++;
				if (launcher.work_info->lock_cnt > launcher.conf->lock_cnt)
				{
					launcher.work_info->dial_status = F_Lock_Dial;
					launcher.info->target_dial_angle = launcher.conf->Back_Angle + launcher.info->measure_dial_angle;
					launcher.work_info->lock_cnt = 0;
				}
			}
			else 
			{
				launcher.work_info->lock_cnt = 0;
			}
		}
	}
	else if (launcher.work_info->dial_status == F_Lock_Dial)
	{
		if (launcher.info->measure_dial_angle > launcher.info->target_dial_angle - launcher.conf->lock_angle_check)
		{
			launcher.info->target_dial_angle = launcher.conf->Load_Angle + launcher.info->measure_dial_angle;
			launcher.work_info->dial_status = Reload_Dial;
		}
		else 
		{
			if (abs(launcher.info->measure_dial_speed) < 5)
			{
				launcher.work_info->lock_cnt ++;
				if (launcher.work_info->lock_cnt > launcher.conf->lock_cnt)
				{
					launcher.work_info->dial_status = Reload_Dial;
					launcher.info->target_dial_angle = -launcher.conf->Back_Angle + launcher.info->measure_dial_angle;
					launcher.work_info->lock_cnt = 0;
				}
			}
			else 
			{
				launcher.work_info->lock_cnt = 0;
			}
		}
	}
	else
	{
		launcher.work_info->dial_status = WaitCommond_Dial;
	}
}



void Fric_Ctrl(void);
void Dial_Ctrl(void);
/**
  * @brief  发射机构电机控制
  * @param  
  * @retval 
  */
void Launcher_Ctrl(void)
{
	Fric_Ctrl();
	Dial_Ctrl();
}


void Fric_Ctrl(void)
{
	if (launcher.work_info->fric_status == On_Fric)
	{
		launcher.info->target_left_speed = -launcher.conf->fric_speed;
		launcher.info->target_right_speed = launcher.conf->fric_speed;
		
		fric_motor[FRIC_L].pid->speed_out = PID_Plc_Calc(&fric_motor[FRIC_L].pid->speed, 
																										 launcher.info->measure_left_speed, 
																										 launcher.info->target_left_speed);
		fric_motor[FRIC_R].pid->speed_out = PID_Plc_Calc(&fric_motor[FRIC_R].pid->speed, 
																										 launcher.info->measure_right_speed, 
																										 launcher.info->target_right_speed);
	}
	else if (launcher.work_info->fric_status == Off_Fric || launcher.work_info->fric_status == WaitCommond_Fric)
	{
		launcher.info->target_left_speed = 0.0f;
		launcher.info->target_right_speed = 0.0f;
		
		fric_motor[FRIC_L].pid->speed_out = PID_Plc_Calc(&fric_motor[FRIC_L].pid->speed, 
																										 launcher.info->measure_left_speed, 
																										 launcher.info->target_left_speed);
		fric_motor[FRIC_R].pid->speed_out = PID_Plc_Calc(&fric_motor[FRIC_R].pid->speed, 
																										 launcher.info->measure_right_speed, 
																										 launcher.info->target_right_speed);
	}
	shoot_out[0] = (int16_t)fric_motor[FRIC_L].pid->speed_out;
	shoot_out[1] = (int16_t)fric_motor[FRIC_R].pid->speed_out;
}

void Dial_Ctrl(void)
{
	if (launcher.work_info->dial_status != WaitCommond_Dial)
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
		
		dial_motor.pid->angle_out = PID_Plc_Calc(&dial_motor.pid->angle, 
																						 launcher.info->measure_dial_angle, 
																						 launcher.info->target_dial_angle);
		dial_motor.pid->speed_out = PID_Plc_Calc(&dial_motor.pid->speed, 
																						 launcher.info->measure_dial_speed, 
																						 dial_motor.pid->angle_out);
	}
	else 
	{
		dial_motor.pid->angle_out = PID_Plc_Calc(&dial_motor.pid->angle, 
																						 launcher.info->measure_dial_angle, 
																						 launcher.info->measure_dial_angle);
		dial_motor.pid->speed_out = PID_Plc_Calc(&dial_motor.pid->speed, 
																						 launcher.info->measure_dial_speed, 
																						 dial_motor.pid->angle_out);
	}
	shoot_out[2] = (int16_t)dial_motor.pid->speed_out;
	
	if (launcher.info->remote_mode == RC)
	{
		if (launcher.work_info->launcher_commond == Magz_Open)
		{
			COVER_PwmOut(180);
		}
		else if (launcher.work_info->launcher_commond == Func_Reset)
		{
			COVER_PwmOut(250);
		}
	}
	else if (launcher.info->remote_mode == KEY)
	{
		if (launcher.work_info->launcher_commond == Magz_Open)
		{
			COVER_PwmOut(180);
		}
		else if (launcher.work_info->launcher_commond == Magz_Close)
		{
			COVER_PwmOut(250);
		}
	}
}




/**
  * @brief  发送发射机构电机控制数据
  * @param  
  * @retval 
  */
void Launcher_SendOut(void)
{
	// 摩擦轮电机离线保护
	if (fric_motor[FRIC_L].work_state == DEV_ONLINE)
	{
		can2_tx_buf[0] = (shoot_out[fric_motor[FRIC_L].driver->rx_id - 0x201] >> 8) & 0xFF;
		can2_tx_buf[1] = shoot_out[fric_motor[FRIC_L].driver->rx_id - 0x201] & 0xFF;
	}
	else
	{
		can2_tx_buf[0] = 0;
		can2_tx_buf[1] = 0;
	}
	
	if (fric_motor[FRIC_R].work_state == DEV_ONLINE)
	{
		can2_tx_buf[2] = (shoot_out[fric_motor[FRIC_R].driver->rx_id - 0x201] >> 8) & 0xFF;
		can2_tx_buf[3] = shoot_out[fric_motor[FRIC_R].driver->rx_id - 0x201] & 0xFF;
	}
	else
	{
		can2_tx_buf[2] = 0;
		can2_tx_buf[3] = 0;
	}
	
	// 拨盘电机离线保护
	if (dial_motor.work_state == DEV_ONLINE)
	{
		can2_tx_buf[4] = (shoot_out[dial_motor.driver->rx_id - 0x201] >> 8) & 0xFF;
		can2_tx_buf[5] = shoot_out[dial_motor.driver->rx_id - 0x201] & 0xFF;
	}
	else
	{
		can2_tx_buf[4] = 0;
		can2_tx_buf[5] = 0;
	}
	
	launcher.info->rc_work_state = rc_sensor.work_state;
	launcher.info->last_s2 = rc_sensor.info->s2;
}



void Shoot_Stop(void);
/**
  * @brief  发射机构控制离线保护
  * @param  
  * @retval 
  */
void Shoot_SelfProtect(void)
{
	Shoot_Stop();
	Shoot_GetInfo();
}

// 遥控器失联保护
void Shoot_Stop(void)
{
	fric_motor[FRIC_L].pid->speed_out = 0.0f;
	fric_motor[FRIC_R].pid->speed_out = 0.0f;
	dial_motor.pid->speed_out = 0.0f;
	shoot_out[0] = (int16_t)fric_motor[FRIC_L].pid->speed_out;
	shoot_out[1] = (int16_t)fric_motor[FRIC_R].pid->speed_out;
	shoot_out[2] = (int16_t)dial_motor.pid->speed_out;
	
	can2_tx_buf[0] = (shoot_out[0] >> 8) & 0xFF;
	can2_tx_buf[1] = shoot_out[0] & 0xFF;
	can2_tx_buf[2] = (shoot_out[1] >> 8) & 0xFF;
	can2_tx_buf[3] = shoot_out[1] & 0xFF;
	can2_tx_buf[4] = (shoot_out[2] >> 8) & 0xFF;
	can2_tx_buf[5] = shoot_out[2] & 0xFF;
}
