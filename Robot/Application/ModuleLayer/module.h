/**
 * @file        module.c/.h
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        5-January-2023
 * @brief       Module Status Control
 * @update
 */

#ifndef __MODULE_H
#define __MODULE_H

/* Includes ------------------------------------------------------------------*/
#include "device.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
	MODULE_STATE_RCLOST,	// 遥控失联
	MODULE_STATE_RCINIT,	// 遥控初始化
	MODULE_STATE_IMUTMP,	// 陀螺仪校准临界状态
	MODULE_STATE_IMUERR,	// 陀螺仪错误
	MODULE_STATE_KILLED,	// 机构断电
	MODULE_STATE_NORMAL,	// 组件正常
	MODULE_STATE_WRONG,	  // 组件错误
} module_state_t;

typedef enum {
	MODULE_MODE_RESET,		// 模式复位
	MODULE_MODE_MACHINE,	// 机械模式
	MODULE_MODE_GYRO,			// 陀螺仪模式
	MODULE_MODE_CNT,
} module_mode_t;

typedef enum {
	MODULE_DIR_ARMOR,		// 云台朝向装甲板
	MODULE_DIR_WHEEL,		// 云台朝向轮子
	MODULE_DIR_CNT,
} module_dir_t;

/*  发射机构状态  */
typedef struct launcher_state_struct {
	fric_cmd_e		fric_state;
	magz_cmd_e		magz_state;
	dial_cmd_e		shoot_state;
} launcher_state_t;

/*  底盘状态  */
typedef enum
{
	chas_reset, 			// 功能复位
	gyro_on,        	// 小陀螺开启
} chas_cmd_e;

/*  底盘速度状态  */
typedef enum
{
	speed_reset, 			// 功能复位
	rapid_on,        	// 加速开启
	rapid_off,       	// 加速关闭
} speed_cmd_e;

/*  热量限制状态  */
typedef enum
{
	heat_limit_on,     // 开启热量限制
	heat_limit_off,    // 关闭热量限制
} heat_cmd_e;

/*  键盘自动打弹状态  */
typedef enum
{
	auto_shoot_off,    // 关闭自动打弹
	auto_shoot_on,     // 开启自动打弹
} auto_cmd_e;

/*  键盘自动打符状态  */
typedef enum
{
	auto_buff_off,		 // 关闭自动打符
	auto_buff_on,			 // 开启自动打符
} autobuff_cmd_e;

/*  打符标志位  */
typedef enum
{
	buff_reset, 			// 功能复位
	small_buff_on,   	// 小符开启
	big_buff_on,     	// 大符开启
	debuff_on,				// 反符开启
} buff_cmd_e;

typedef struct {
	launcher_state_t	lch_state;
	gim_cmd_e					gim_cmd;
	gim_cmd_e					gim_state;
	chas_cmd_e				chas_cmd;
//	chas_cmd_e				chas_state;
	lch_cmd_t					lch_cmd;
	gim_mode_e				gim_mode;
	heat_cmd_e				heat_mode;
	speed_cmd_e				speed_cmd;
	auto_cmd_e				auto_cmd;
	autobuff_cmd_e		autobuff_cmd;
	buff_cmd_e				buff_cmd;
	uint8_t						tw_last_step[4];
} status_t;

typedef struct __module_t
{
	remote_mode_t		remote_mode;	// 控制方式
	module_state_t	state;	// 组件状态
	module_mode_t		mode;	// 组件模式
	module_dir_t		dir;	// 云台朝向
	symbal_t				*symbal;
	status_t				*status;
	
	void (*heartbeat)(struct __module_t *module);
} module_t;

extern status_t status;
extern module_t module;

/* Exported functions --------------------------------------------------------*/

#endif
