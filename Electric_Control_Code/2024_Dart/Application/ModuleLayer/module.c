/**
 ******************************************************************************
 * @file        module.c
 * 
 *                  module
 *                    |
 *      --------------------------------
 *      |                              |
 *     axis                         launcher
 *    ------              -----------------------------
 *      |                 |             |             |
 *   3个模式         MANUAL_MODE   RESET_MODE    AUTO_MODE
 *                                      |             |
 * 								     六个状态       七个状态
 *                    
 */
/* Includes ------------------------------------------------------------------*/
#include "module.h"
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/**
 * @brief  模块总控制结构体
 */
module_t module = {
	.launcher = &launcher,
	.axis = &axis,
};

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  摇控模拟裁判系统数据更新
 * @param  referee_data 裁判系统数据
 * @param  launcher 飞镖发射及供弹控制
 * @retval None
 */
void State_Update(referee_data_t *referee_data, launcher_t *launcher)
{
	if(rc_sensor.info->s1 == RC_SW_MID)
	{
		referee_data->game_progress = 4;
		if(rc_sensor.info->s2 == RC_SW_DOWN)
		{
			referee_data->dart_launch_opening_status = 1;
		}
		else if(rc_sensor.info->s2 == RC_SW_MID)
		{
			referee_data->dart_launch_opening_status = 2;
		}
		else if(rc_sensor.info->s2 == RC_SW_UP)
		{
			referee_data->dart_launch_opening_status = 0;
		}
	}
}

void mode_select(module_t *module)
{
	State_Update(&referee_data, &launcher);

	uint8_t open_status = referee_data.dart_launch_opening_status;

	/*闸门标志*/
	if((open_status == 0 || open_status == 2) 
		&& module->launcher->state == SLEEP_STATE
		&& module->launcher->times <= 4 && referee_data.game_progress == 4)
	{
		module->launcher->state = PULL_STATE;
	}
	if(open_status == 1)
	{
		if(module->launcher->state != FINISHED_STATE)
		{
			module->launcher->state = SLEEP_STATE;
		}
	}

	/*检查复位*/
	if(module->launcher->info->reset_state != 2)
	{
		module->launcher->mode = RESET_MODE;
	}
	else if(rc_sensor.info->s1 == RC_SW_UP)
	{
		module->launcher->mode = MANUAL_MODE;
	}
	else
	{
		module->launcher->mode = AUTO_MODE;
	}

	/*飞镖轴是否解锁*/
	if(rc_sensor.info->s1 == RC_SW_DOWN)
    {
        module->axis->mode = NORMAL_MODE;
        if(rc_sensor.info->s2 == RC_SW_DOWN)
        {
	        if(rc_sensor.info->thumbwheel.step[0] == 1)
            {
                module->axis->state = UNLOCK_STATE;
            }
            else
            {
                module->axis->state = LOCK_STATE;
            }
			vision_sensor.info->tx_info->mode = 0;
        }
    }
//	if(rc_sensor.info->s1 == RC_SW_UP)
//    {
//        module->axis->mode = NORMAL_MODE;

//	    if(rc_sensor.info->thumbwheel.step[0] == 1)
//        {
//            module->axis->state = UNLOCK_STATE;
//        }
//        else
//        {
//            module->axis->state = LOCK_STATE;
//        }
//    }

    /*飞镖轴是否复位*/
	/*手动*/
    if(rc_sensor.info->s1 == RC_SW_UP)
    {
        if(rc_sensor.info->thumbwheel.value > 100 && module->axis->state != SAVE_STATE)
        {
            module->axis->mode = SAVE_MODE;
            module->axis->state = ZERO_STATE;
        }
        else if(rc_sensor.info->thumbwheel.value < -100 && module->axis->state != POS_STATE
				&& module->axis->state != SAVE_STATE)
        {
            module->axis->mode = POSITION_MODE;
            module->axis->state = ZERO_STATE;
        }
//		else if(module->axis->mode != POSITION_MODE/* || module->axis->mode != SAVE_MODE*/)//
//		{
//			module->axis->mode = NORMAL_MODE;
//			module->axis->state = UNLOCK_STATE;
//		}
//		
//		rc_sensor.info->thumbwheel.step[0] = 0;
    }

	/*视觉*/
	/*正式比赛*/
    if(rc_sensor.info->s1 == RC_SW_DOWN)
    {
        if(rc_sensor.info->s2 == RC_SW_MID /*&& module->axis->state == UNLOCK_STATE*/)
        {
			module->axis->mode = NORMAL_MODE;
			module->axis->state = VISION_STATE;
            vision_sensor.info->tx_info->mode = 1;
        }

    }
	/*模拟比赛*/
	if(rc_sensor.info->s1 == RC_SW_MID)
    {
		if(rc_sensor.info->thumbwheel.value > 100 && module->axis->state != VISION_STATE)
		{
			module->axis->mode = NORMAL_MODE;
			module->axis->state = VISION_STATE;
//			vision_sensor.info->tx_info->mode = AIM_ON;
		}
		else if(rc_sensor.info->thumbwheel.value < -100)
		{
			module->axis->mode = NORMAL_MODE;
			module->axis->state = UNLOCK_STATE;
		}
	}
}

/*视觉发送*/
void Tx_vision_info_update(vision_sensor_t *vision_sensor)
{
    vision_tx_info_t *tx_info = vision_sensor->info->tx_info;
	tx_info->number = module.launcher->times;
	tx_info->state = module.axis->state;
	tx_info->done = referee_data.dart_launch_opening_status;
}
