/**
 * @file        axis.h
 * @brief       飞镖轴电机朝向控制
 */
#ifndef __AXIS_H
#define __AXIS_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rp_math.h"
#include "launcher.h"
#include "device.h"

/* Exported macro ------------------------------------------------------------*/
#define AXISX_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[AXIS_X], speed)
#define AXISX_TOANGLE(angle) 	Motor_ToAxleAngle(&rm_motor[AXIS_X], angle)
#define AXISZL_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[AXIS_ZL], speed)
#define AXISZR_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[AXIS_ZR], speed)
#define AXISY_TOSPEED(speed) 	Motor_ToSpeed(&rm_motor[AXIS_Y], speed)
#define AXISY_TOANGLE(angle) 	Motor_ToAxleAngle(&rm_motor[AXIS_Y], angle)
#define RESET_SPEED 2000

/* Private typedef -----------------------------------------------------------*/
/**
 * @brief  飞镖轴电机朝向控制
 */
typedef enum
{
	LOCK_STATE,
	UNLOCK_STATE,
	SAVE_STATE,
    SAVED_STATE,
	POS_STATE,  /*上一次对位*/
	ZERO_STATE,
	VISION_STATE,
} axis_state_e;

typedef enum
{
    NORMAL_MODE,
    SAVE_MODE,
    POSITION_MODE,
    // TEST_MODE,
} axis_mode_e;

/**
 * @brief  飞镖轴信息
 */
typedef struct 
{
	int16_t axisx_speed;
	int16_t axiszl_speed;
    int16_t axiszr_speed;
	int32_t axisx_angle;
	int32_t axiszl_angle;
    int32_t axiszr_angle;
	int32_t flash_x_angle;
	int32_t flash_zl_angle;
    int32_t flash_zr_angle;
}axis_reset_info_t;

typedef struct 
{
	int16_t axisx_speed;
	int16_t axisz_speed;
	int32_t axisx_angle;
	int32_t axisz_angle;
	int32_t lock_x_angle;
	int32_t lock_zl_angle;
	int32_t lock_zr_angle;
	int32_t lock_y_angle;
}axis_now_info_t;

/**
 * @brief  飞镖轴控制结构体
 */
typedef struct 
{
	axis_state_e        state;
    axis_mode_e         mode;
	axis_reset_info_t   *reset_info;
	axis_now_info_t	 	*now_info;
}axis_t;

/* Exported variables --------------------------------------------------------*/
extern axis_t axis;

/* Exported functions --------------------------------------------------------*/
void AXISZ_TOSPEED(int16_t speed);
void AXISZ_TOANGLE(int32_t angle_zl, int32_t angle_zr);
void axis_ctrl(axis_t *axis);

#endif
