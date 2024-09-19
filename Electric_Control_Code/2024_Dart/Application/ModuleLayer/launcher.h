/**
 ******************************************************************************
 * @file        launcher.h
 * @brief       飞镖发射及供弹控制
 */

#ifndef __LAUNCHER_H
#define __LAUNCHER_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "device.h"

/* Exported macro ------------------------------------------------------------*/
#define BELT_L_SPEED 6500 //8000
#define BELT_R_SPEED 7200 //8000

#define BELT_R_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[BELT_R], speed)
#define BELT_L_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[BELT_L], speed)
#define DIAL_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[DIAL], speed)
#define WIND_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[WIND], speed)
#define WIND_TOANGLE(angle) 	Motor_ToAngle(&rm_motor[WIND], angle)
#define DIAL_TOANGLE(angle) 	Motor_ToAxleAngle(&rm_motor[DIAL], angle)
#define BELT_R_TOANGLE(angle) 	Motor_ToAxleAngle(&rm_motor[BELT_R], angle)
#define BELT_L_TOANGLE(angle) 	Motor_ToAxleAngle(&rm_motor[BELT_L], angle)

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  发射状态枚举
 */
typedef enum
{
	PULL_STATE,			//拉状态
	PUSH_STATE,			//推状态
	READY_STATE,		//准备发射
	SHOOT_STATE,		//发射
	FEED_STATE,			//供弹状态
	WAIT_STATE,			//等待发射
	SLEEP_STATE,		//休眠状态
	FINISHED_STATE,     //打完
} launcher_state_e;

/**
 * @brief  发射模式枚举
 */
typedef enum
{
	MANUAL_MODE,	//手动模式
 	AUTO_MODE,		//自动模式
	RESET_MODE,
} launcher_mode_e;

/**
 * @brief  飞镖当前各电机信息
 */
typedef struct 
{
	int16_t belt_r_speed;
	int32_t belt_r_angle;
	int16_t belt_l_speed;
	int16_t dial_speed;
	int32_t dial_angle;
	int16_t wind_angle;
	int16_t reset_state;
	int32_t reset_angle;
	int16_t belt_r_reset_speed;
	int32_t belt_r_reset_angle;
    int32_t belt_r_slow_angle;
	int32_t belt_l_reset_angle;
    int32_t belt_l_final_angle;
}launcher_info_t;

/**
 * @brief  飞镖发射控制结构体
 */
typedef struct 
{
	launcher_mode_e        mode;
	launcher_state_e       state;
	launcher_info_t		   *info;
	int8_t                 times;//记录飞镖打到第几发
	int16_t                 cnt;//计时/计数器
	
} launcher_t;

/* Exported variables --------------------------------------------------------*/
extern launcher_t launcher;

/* Exported functions --------------------------------------------------------*/
void launcher_manual(launcher_t *launcher);
void launcher_auto(launcher_t *launcher);
void launcher_reset(launcher_t *launcher);
void launcher_ctrl(launcher_t *launcher);

#endif
