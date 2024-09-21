/**
 ******************************************************************************
 * @file        axis.c
 * @brief       飞镖轴电机朝向控制
 ******************************************************************************
 */
 
/* Includes ------------------------------------------------------------------*/
#include "axis.h"

/* Private typedef -----------------------------------------------------------*/
/**
 * @brief  轴复位信息
		   机械限位最大值：
		   axis_x最大值 -12336003
		   axis_z最大值 -3471399
		   axis_y最大值 -9160618
 */
axis_reset_info_t reset_info = {
    .axisx_speed = RESET_SPEED,
    .axiszl_speed = RESET_SPEED,
    .axiszr_speed = RESET_SPEED,
    .axisx_angle = 0,
    .axiszl_angle = 0,
    .axiszr_angle = 0,
};

axis_now_info_t now_info = {
	.axisx_speed = 0,
	.axisz_speed = 0,
	.axisx_angle = 0,
	.axisz_angle = 0,
	.lock_x_angle = 0,
	.lock_zl_angle = 0,
	.lock_zr_angle = 0,
};

pid_ctrl_t axis_pid = {
    .kp = -40,
    .ki = 0,
    .kd = 0.0,
    .integral_max = 1000,
    .out_max = 9000,
};

/**
 * @brief  飞镖轴控制结构体
 */
axis_t axis = {
	.mode = NORMAL_MODE,
    .state = LOCK_STATE,
    .reset_info = &reset_info,
	.now_info = &now_info,
};

/* Private functions **--------------------------------------------------------*/
/**
 * @brief 2006一圈8192*36 = 294912
 */
void AXISZ_TOSPEED(int16_t speed)
{
	Motor_ToSpeed(&rm_motor[AXIS_ZL], speed);
	Motor_ToSpeed(&rm_motor[AXIS_ZR], speed);
}

void AXISZ_TOANGLE(int32_t angle_zl, int32_t angle_zr)
{
	Motor_ToAxleAngle(&rm_motor[AXIS_ZL], angle_zl);
	Motor_ToAxleAngle(&rm_motor[AXIS_ZR], angle_zr);
}

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  将当前角度储存到FLASH中
 */
void axis_save(axis_t *axis)
{	
	uint32_t flash_data[3] = {0};

	flash_data[0] = axis->reset_info->flash_x_angle;
	flash_data[1] = axis->reset_info->flash_zl_angle;
    flash_data[2] = axis->reset_info->flash_zr_angle;

	Flash_EraseSector11();
	Flash_WriteWordData(SECTOR_11_ADDRESS, flash_data, 3);
}

/**
 * @brief  启动时从FLASH中读取当前角度
 */
void axis_load(axis_t *axis)
{
	uint32_t	flash_data[3] = {0};
	Flash_ReadData(SECTOR_11_ADDRESS, flash_data, 12);

	axis->reset_info->flash_x_angle = flash_data[0];
    axis->reset_info->flash_zl_angle = flash_data[1];
    axis->reset_info->flash_zr_angle = flash_data[2];
}

/**
 * @brief  飞镖轴复位,并将当前角度储存到FLASH中
 * @param  axis: 飞镖轴控制结构体
 * @retval 无
 */
void axis_reset(axis_t *axis)
{
    switch (axis->state)
    {
        case ZERO_STATE:
        {

            axis->reset_info->axisx_angle = rm_motor[AXIS_X].info->angle_sum;
            axis->state = SAVE_STATE;
            break;
        }
        case SAVE_STATE:
        {
            AXISX_TOSPEED(axis->reset_info->axisx_speed);
            AXISY_TOSPEED(0);
			AXISZ_TOSPEED(0);

            if(Motor_DetectStuck(&rm_motor[AXIS_X]))
            {
                axis->reset_info->axisx_speed = 0;
            }
            if(axis->reset_info->axisx_speed == 0 )
            {
                axis->reset_info->flash_x_angle = rm_motor[AXIS_X].info->angle_sum - axis->reset_info->axisx_angle;
                axis->reset_info->flash_zl_angle = rm_motor[AXIS_ZL].info->angle_sum - axis->reset_info->axiszl_angle;
                axis->reset_info->flash_zr_angle = rm_motor[AXIS_ZR].info->angle_sum - axis->reset_info->axiszr_angle;
                axis_save(axis);
                axis->reset_info->axisx_speed = RESET_SPEED;
                axis->reset_info->axiszl_speed = RESET_SPEED;
                axis->reset_info->axiszr_speed = RESET_SPEED;
                axis->reset_info->axisx_angle = rm_motor[AXIS_X].info->angle_sum - axis->reset_info->flash_x_angle;
                axis->state = POS_STATE;
            }
            break;
        }
        case POS_STATE:
        {
            AXISX_TOANGLE(axis->reset_info->axisx_angle);
			AXISZ_TOSPEED(0);
            AXISY_TOSPEED(0);
            if(abs(rm_motor[AXIS_X].info->angle_sum - axis->reset_info->axisx_angle) < 10 )
            {
                axis->mode = NORMAL_MODE;
                axis->state = LOCK_STATE;
            }
            break;
        }
        default: break;
    }
}

/**
 * @brief  飞镖轴复位后对位
 * @param  axis: 飞镖轴控制结构体
 * @retval 无
 */
void axis_toposition(axis_t *axis)
{
    axis->now_info->lock_x_angle = rm_motor[AXIS_X].info->angle_sum;
    axis->now_info->lock_zl_angle = rm_motor[AXIS_ZL].info->angle_sum;
    axis->now_info->lock_zr_angle = rm_motor[AXIS_ZR].info->angle_sum;
	axis->now_info->lock_y_angle = rm_motor[AXIS_Y].info->angle_sum;
    switch (axis->state)
    {
        case ZERO_STATE:
        {
            AXISX_TOSPEED(axis->reset_info->axisx_speed);
            AXISY_TOSPEED(0);
			AXISZ_TOSPEED(0);
            if(Motor_DetectStuck(&rm_motor[AXIS_X]))
            {
                axis->reset_info->axisx_speed = 0;
            }
            if(axis->reset_info->axisx_speed == 0)
            {
                axis_load(axis);
                axis->reset_info->axisx_angle = rm_motor[AXIS_X].info->angle_sum - axis->reset_info->flash_x_angle;
                axis->reset_info->axiszl_angle = rm_motor[AXIS_ZL].info->angle_sum - axis->reset_info->flash_zl_angle;
                axis->reset_info->axiszr_angle = rm_motor[AXIS_ZR].info->angle_sum - axis->reset_info->flash_zr_angle;
                axis->reset_info->axisx_speed = RESET_SPEED;
                axis->reset_info->axiszl_speed = RESET_SPEED;
                axis->reset_info->axiszr_speed = RESET_SPEED;
                axis->state = POS_STATE;
            }
            break;
        }
        case POS_STATE:
        {
            AXISX_TOANGLE(axis->reset_info->axisx_angle);
			AXISZ_TOSPEED(0);
            AXISY_TOSPEED(0);
            if(abs(rm_motor[AXIS_X].info->angle_sum - axis->reset_info->axisx_angle) < 10 )
            {
                axis->mode = NORMAL_MODE;
                axis->state = LOCK_STATE;
            }
            break;
        }
        default: break;
    }
}

/**
 * @brief  飞镖轴普通模式控制
 * @param  axis: 飞镖轴控制结构体
 * @retval 无
 */
void axis_normal(axis_t *axis)
{
    pid_ctrl_t *pid = &axis_pid;
	
	static int32_t y_angle;
    switch(axis->state)
    {
	    case LOCK_STATE:
	    {
		    AXISX_TOANGLE(axis->now_info->lock_x_angle);
		    AXISZ_TOANGLE(axis->now_info->lock_zl_angle, axis->now_info->lock_zr_angle);
		    AXISY_TOANGLE(axis->now_info->lock_y_angle);
            break;
        }
	    case UNLOCK_STATE:
        {
            AXISX_TOSPEED(-rc_sensor.info->ch0*20);
            AXISZ_TOSPEED(-rc_sensor.info->ch1*20);
            AXISY_TOSPEED(rc_sensor.info->ch2*20);
            axis->now_info->lock_x_angle = rm_motor[AXIS_X].info->angle_sum;
            axis->now_info->lock_zl_angle = rm_motor[AXIS_ZL].info->angle_sum;
            axis->now_info->lock_zr_angle = rm_motor[AXIS_ZR].info->angle_sum;
            axis->now_info->lock_y_angle = rm_motor[AXIS_Y].info->angle_sum;
            break;
        }
	    case VISION_STATE:
	    {

			if(judge_sensor.info->game_status.stage_remain_time > 5 && judge_sensor.info->game_status.game_progress == 1)
			{
				axis->now_info->lock_x_angle = rm_motor[AXIS_X].info->angle_sum;
				axis->now_info->lock_zl_angle = rm_motor[AXIS_ZL].info->angle_sum;
				axis->now_info->lock_zr_angle = rm_motor[AXIS_ZR].info->angle_sum;
				axis->now_info->lock_y_angle = rm_motor[AXIS_Y].info->angle_sum;
				AXISX_TOSPEED(-rc_sensor.info->ch0*20);
				AXISZ_TOSPEED(-rc_sensor.info->ch1*20);
				AXISY_TOSPEED(rc_sensor.info->ch2*20);
			}
			else if(judge_sensor.info->game_status.stage_remain_time <= 5 && judge_sensor.info->game_status.game_progress == 1)
			{
				axis->now_info->lock_x_angle = rm_motor[AXIS_X].info->angle_sum;
				axis->now_info->lock_zl_angle = rm_motor[AXIS_ZL].info->angle_sum;
				axis->now_info->lock_zr_angle = rm_motor[AXIS_ZR].info->angle_sum;
				axis->now_info->lock_y_angle = rm_motor[AXIS_Y].info->angle_sum;
				AXISX_TOSPEED(-rc_sensor.info->ch0*20);
				AXISZ_TOSPEED(-rc_sensor.info->ch1*20);
				AXISY_TOSPEED(rc_sensor.info->ch2*20);
			}
			else if(judge_sensor.info->game_status.game_progress == 4)
			{
				/*视觉控制x轴*/
				vision_sensor.info->tx_info->mode = 1;
				 /* When the dart gate is open */
				if(referee_data.dart_launch_opening_status != 1)
				{
					/* When the vision sensor is online and the target is found */
					if(vision_sensor.work_state == DEV_ONLINE && vision_sensor.info->target_status == 1)
					{
						/* Record the error and double it */
						pid->err = abs(vision_sensor.info->rx_info->yaw_error) * vision_sensor.info->rx_info->yaw_error / 2;
						single_pid_ctrl(pid);
						/* Calculate the PID output */
						AXISX_TOSPEED(pid->out);
						/* Record the now angle of axis_x */
						y_angle = rm_motor[AXIS_X].info->angle_sum;
					}
					else
					{
						/* When the vision sensor is offline or the target is lost, axis_x need to be protected */
						AXISX_TOANGLE(y_angle);
					}
				}
				else
				{
					AXISX_TOANGLE(axis->now_info->lock_x_angle);
				}
				AXISZ_TOANGLE(axis->now_info->lock_zl_angle, axis->now_info->lock_zr_angle);
				
			}
			else
			{
				AXISX_TOSPEED(-rc_sensor.info->ch0*20);
				AXISZ_TOSPEED(-rc_sensor.info->ch1*20);
				AXISY_TOSPEED(rc_sensor.info->ch2*20);
				axis->now_info->lock_x_angle = rm_motor[AXIS_X].info->angle_sum;
				axis->now_info->lock_zl_angle = rm_motor[AXIS_ZL].info->angle_sum;
				axis->now_info->lock_zr_angle = rm_motor[AXIS_ZR].info->angle_sum;
				axis->now_info->lock_y_angle = rm_motor[AXIS_Y].info->angle_sum;
			}
            break;
	    }
        default: break;
    }
}

/**
 * @brief  飞镖轴控制
 * @param  axis: 飞镖轴控制结构体
 * @retval 无
 */
void axis_ctrl(axis_t *axis)
{
    switch(axis->mode)
    {
        case NORMAL_MODE:
        {
            axis_normal(axis);
            break;
        }
        case SAVE_MODE:
        {
            axis_reset(axis);
            break;
        }
        case POSITION_MODE:
        {
            axis_toposition(axis);
            break;
        }
        default: break;
    }
}
