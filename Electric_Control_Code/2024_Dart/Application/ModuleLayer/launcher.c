/**
 ******************************************************************************
 * @file        launcher.c
 * @brief       飞镖发射及供弹控制
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "launcher.h"
#include "rp_math.h"

/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define WIND_ANGLE_1 400
#define WIND_ANGLE_2 1331
#define WIND_ANGLE_3 4090
#define WIND_ANGLE_4 6809

/* Private typedef -----------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
launcher_info_t launcher_info = {
	.belt_l_speed = -BELT_L_SPEED,
	.belt_r_speed = BELT_R_SPEED,
    .belt_r_slow_angle = 0,
	.belt_r_reset_angle = 0,
	.belt_r_reset_speed = 800,
	.dial_speed = 300,
	.dial_angle = 0,
	.wind_angle = 0,
	.reset_angle = 1150,
	.reset_state = 0,
};

launcher_t launcher = {
	.cnt = 0,
	.times = 0,
	.state = SLEEP_STATE,
	.mode = RESET_MODE,
	.info = &launcher_info,
};
/* Export function -----------------------------------------------*/
/**
 * @brief  手动调节
 */
void launcher_manual(launcher_t *launcher)
{
	
	BELT_R_TOSPEED(rc_sensor.info->ch3*30);
	Motor_SendData(rm_motor[BELT_L].driver, 0);
	DIAL_TOANGLE(launcher->info->dial_angle);
	WIND_TOANGLE(launcher->info->wind_angle);
	if(rc_sensor_info.s2 == RC_SW_MID)
	{
		Trigger_Close();
	}
	else if(rc_sensor_info.s2 == RC_SW_UP)
	{
		Trigger_Open();
	}
	else if(rc_sensor_info.s2 == RC_SW_DOWN)
	{
		Trigger_Sleep();
	}
	launcher->times = 0;
	launcher->cnt = 0;
}

/**
 * @brief  供弹复位
 *	state = 0:电机归位
 *	state = 1:检测到位
 *	state = 2:复位完成
 * @param  launcher: 飞镖发射控制结构体
 */
void launcher_reset(launcher_t *launcher)
{
	launcher->cnt = 0;
	if(launcher->info->reset_state != 2)
	{
		BELT_R_TOSPEED(launcher->info->belt_r_reset_speed);
		DIAL_TOSPEED(launcher->info->dial_speed);
		if(Motor_DetectStuck(&rm_motor[DIAL]))
		{
			launcher->info->dial_speed = 0;
		}
		if(Motor_DetectStuck(&rm_motor[BELT_R]))
		{
			launcher->info->belt_r_reset_speed = 0;
		}
	}
	
	if(launcher->info->reset_state == 0)//左皮带先推到顶
	{
		WIND_TOSPEED(0);
		BELT_L_TOSPEED(700);
		if(Motor_DetectStuck(&rm_motor[BELT_L]))
		{
			launcher->info->reset_state++;
		}
	}
	else if(launcher->info->reset_state == 1)
	{
		BELT_L_TOSPEED(0);
        WIND_TOANGLE(WIND_ANGLE_1);
		if(launcher->info->dial_speed == 0 && launcher->info->belt_r_reset_speed == 0)
        {
            launcher->info->reset_state++;
            launcher->info->dial_angle = rm_motor[DIAL].info->angle_sum - 75000;
            launcher->info->belt_l_reset_angle = rm_motor[BELT_L].info->angle_sum;
			launcher->info->belt_r_reset_angle = rm_motor[BELT_R].info->angle_sum - 12000;
            launcher->info->belt_l_final_angle = launcher->info->belt_l_reset_angle - 970000;
            launcher->info->belt_r_slow_angle = launcher->info->belt_r_reset_angle - 400000;
			launcher->info->wind_angle = WIND_ANGLE_1;
        }
	}
    else if(launcher->info->reset_state == 2)
    {
		BELT_L_TOSPEED(0);
        WIND_TOANGLE(launcher->info->wind_angle);
		DIAL_TOANGLE(launcher->info->dial_angle);
		BELT_R_TOANGLE(launcher->info->belt_r_reset_angle);
    }
	else
    {
		CAN_SendAllZero();
	}	
}

/**
 * @brief  自动模式控制。闸门开启耗时约 7 秒，发射站闸门完全开启
 *		   时长为 15 秒，需要在22秒内发射完两发飞镖。
 * @param  launcher: 飞镖发射控制结构体
 */
void launcher_auto(launcher_t *launcher)
{
    static launcher_state_e last_state = SHOOT_STATE;/**/
	WIND_TOANGLE(launcher->info->wind_angle);
	switch(launcher->state)
	{
		case SLEEP_STATE:
		{
			BELT_L_TOSPEED(0);
			BELT_R_TOANGLE(launcher->info->belt_r_reset_angle);
			DIAL_TOANGLE(launcher->info->dial_angle);
			if(launcher->times >= 4 || last_state != SHOOT_STATE)
			{
				launcher->state = FINISHED_STATE;
			}
			break;
		}
		case PULL_STATE:	//拉状态，把皮筋拉下上膛
		{
            last_state = PULL_STATE;/**/
			DIAL_TOANGLE(launcher->info->dial_angle);
			BELT_L_TOSPEED(0);
			BELT_R_TOANGLE(launcher->info->belt_r_reset_angle - 1887000);//
			motor_pid_t *Motor_Pid = &rm_motor[BELT_R].motor_pid;
			
            /*第一发推镖*/
			if(rm_motor[BELT_R].info->angle_sum < (launcher->info->belt_r_reset_angle - 1883000))
			{
				Trigger_Close();
				launcher->cnt++;
				if(launcher->cnt > 1000)//给扳机一点时间关上
				{
					launcher->times++; // 记录第几发飞镖上膛
					if(launcher->times == 2)
					{
						launcher->info->wind_angle = WIND_ANGLE_2;
					}
					else if(launcher->times == 3)
					{
						launcher->info->wind_angle = WIND_ANGLE_3;
					}
					else if(launcher->times == 4)
					{
						launcher->info->wind_angle = WIND_ANGLE_4;
					}
					
					launcher->state = PUSH_STATE;
					launcher->cnt = 0;
				}
			}
			else
			{
				Trigger_Open();
				if(launcher->times == 0)
				{
					if(abs(rm_motor[BELT_R].info->angle_sum - launcher->info->belt_r_slow_angle) < 50000)
					{
						/*推镖过快镖容易从槽中滑出，推滑块前速度较慢*/
						Motor_Pid->angle.out_max = 1000;
					}
					else
					{
						/*之后推到底*/					
                        if(Motor_Pid->angle.out_max >= BELT_R_SPEED)
                        {
                            Motor_Pid->angle.out_max = BELT_R_SPEED;
                        }
                        else
                        {
                            Motor_Pid->angle.out_max += 6;
                        }
					}
				}
				else
				{
					Motor_Pid->angle.out_max = BELT_R_SPEED;;
				}
			}
			break;
		}
		case PUSH_STATE:  //把滑块推回去
		{
			BELT_L_TOSPEED(0);

			BELT_R_TOANGLE(launcher->info->belt_r_reset_angle);
			//推滑块的同时风车和拨弹开始进行
			DIAL_TOANGLE(launcher->info->dial_angle + 74800);
			if(abs(rm_motor[WIND].info->angle - launcher->info->wind_angle)<10 && 
				abs(rm_motor[DIAL].info->angle_sum - launcher->info->dial_angle) > 73800)
			{
				launcher->cnt++;
				if(launcher->cnt > 300)
				{
					launcher->state = FEED_STATE;
					launcher->cnt = 0;
				}
			}
			break;
		}
		case FEED_STATE:  //把飞镖推进槽内
		{

            BELT_R_TOANGLE(launcher->info->belt_r_reset_angle);
            DIAL_TOANGLE(launcher->info->dial_angle + 74750);
            if(launcher->cnt <= 200)
            {
                BELT_L_TOANGLE(launcher->info->belt_l_final_angle);
				Motor_HandleStuck(&rm_motor[BELT_L]);
                if(abs(rm_motor[BELT_L].info->angle_sum - launcher->info->belt_l_final_angle) < 1800)
                {
                    launcher->cnt++;
                }
            }
            else if (launcher->cnt > 200 && launcher->cnt <= 400)
            {
                BELT_L_TOANGLE(launcher->info->belt_l_reset_angle);
                if(abs(rm_motor[BELT_L].info->angle_sum - launcher->info->belt_l_reset_angle) < 1800)
                {
                    launcher->cnt++;
                }
            }
            else if (launcher->cnt > 400)
            {
                if(launcher->times > 1)
			 	{
					launcher->info->wind_angle += 1365; 
				}
                launcher->state = READY_STATE;
            }
			break;
		}
		case READY_STATE:
		{
			BELT_L_TOSPEED(0);
			BELT_R_TOANGLE(launcher->info->belt_r_reset_angle);
			DIAL_TOANGLE(launcher->info->dial_angle);
	
			if(abs(rm_motor[WIND].info->angle - launcher->info->wind_angle)<5 && 
				abs(rm_motor[DIAL].info->angle_sum - launcher->info->dial_angle)<400 && 
				abs(rm_motor[BELT_R].info->speed) < 50)
			{
				launcher->state = WAIT_STATE;
				/*为下一次循环赋初值*/
				launcher->info->belt_l_speed = -BELT_L_SPEED;
				launcher->info->belt_r_speed = BELT_R_SPEED;
				launcher->info->belt_r_reset_angle = rm_motor[BELT_R].info->angle_sum ;
				launcher->cnt = 0;
			}
			break;
		}
		case WAIT_STATE://延时1秒发射
		{
			BELT_L_TOSPEED(0);
			BELT_R_TOSPEED(0);
			DIAL_TOANGLE(launcher->info->dial_angle);
			launcher->cnt++;
			if(launcher->cnt > 1000)
			{
				launcher->cnt = 1001;
				if(referee_data.dart_launch_opening_status == 0)
				{
					launcher->state = SHOOT_STATE;
					launcher->cnt = 0;
				}
			}
			break;
		}
		case SHOOT_STATE:
		{
			Trigger_Open();
			BELT_L_TOSPEED(0);
			BELT_R_TOSPEED(0);
			DIAL_TOANGLE(launcher->info->dial_angle);
			launcher->cnt++;
			if(launcher->times == 1 || launcher->times == 3)
			{
				if(launcher->cnt > 1000)
				{
					launcher->state = PULL_STATE;
					launcher->cnt = 0;
				}
			}
			else
			{
				launcher->cnt = 0;
			}

            last_state = SHOOT_STATE;/**/
			break;
		}
		case FINISHED_STATE:
		{
			Motor_SendData(rm_motor[DIAL].driver, 0);
			Motor_SendData(rm_motor[WIND].driver, 0);
			Motor_SendData(rm_motor[BELT_L].driver, 0);
			Motor_SendData(rm_motor[BELT_R].driver, 0);
		}
		default: break;
	}
}

/**
 * @brief  飞镖发射总控制
 * @param  launcher: 飞镖发射控制结构体
 */
void launcher_ctrl(launcher_t *launcher)
{
	if(launcher->mode == MANUAL_MODE)
	{
		launcher_manual(launcher);
	}
	else if(launcher->mode == AUTO_MODE)
	{
		launcher_auto(launcher);
	}
	else if(launcher->mode == RESET_MODE)
	{
		launcher_reset(launcher);
	}
}
