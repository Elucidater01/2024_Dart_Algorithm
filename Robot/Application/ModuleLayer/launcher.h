/**
 * @file        launcher.c/.h
 * @author      SSDCFXB
 * @Version     V1.0
 * @date        20-November-2022
 * @brief       Launcher Control Center
 */

#ifndef __LAUNCHER_H
#define __LAUNCHER_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rm_protocol.h"

#include "device.h"
#include "module.h"

/* Exported macro ------------------------------------------------------------*/
#define Fric_15_init 4400.0f
#define Fric_18_init 4870.0f
#define Fric_20_init 4950.0f
#define Fric_22_init 5200.0f
#define Fric_30_init 7020.0f

/* Exported types ------------------------------------------------------------*/
// 发射机构模式枚举
typedef enum
{
	lch_keep,      // 保持
	lch_gyro,      // 陀螺仪
	lch_gyro2,     // 陀螺仪
	lch_machine,   // 机械
} launcher_mode_e;

// 摩擦轮状态枚举
typedef enum
{
	WaitCommond_Fric, // 等待指令
	On_Fric,        // 开启
	Off_Fric,       // 关闭
} fric_status_e;

// 拨盘状态枚举
typedef enum
{
	WaitCommond_Dial,// 等待指令
	Reload_Dial,     // 补弹
	Lock_Dial,    	 // 卡弹
	SpeedKeep_Dial,  // 速度保持
	SwiftKeep_Dial,  // 速射保持
//	Unload_Dial,   // 退弹
//	B_Lock_Dial ,  // 反向卡弹
} dial_status_e;

/*  摩擦轮指令枚举  */
typedef enum {
	Fric_Reset,  	  // 功能复位
	Fric_Open,   		// 开摩擦轮
	Fric_Close,   	// 关摩擦轮
	Fric_Toggle,		// 开关摩擦轮
} Fric_cmd_e;

/*  弹仓指令枚举  */
typedef enum {
	Magz_Reset,  	  // 功能复位
	Magz_Open,      // 开弹仓
	Magz_Close,     // 关弹仓
} Magz_cmd_e;

/*  拨盘指令枚举  */
typedef enum {
	Shoot_Reset,  	// 功能复位
	Single_Shoot,   // 单发
	Keep_Shoot,     // 连发
	Swift_Shoot,		// 速射
} Dial_cmd_e;

// 发射机构指令枚举
typedef struct
{
	Fric_cmd_e  Fric_cmd;
	Magz_cmd_e	Magz_cmd;
	Dial_cmd_e	Dial_cmd;
} launcher_commond_t;

typedef struct 
{
	/*RM_motor_t    *fricl_motor;
	RM_motor_t    *fricr_motor;
	RM_motor_t    *dial_motor;*/
	rc_sensor_t		*rc_sensor;
} launcher_dev_t;

typedef struct
{
	remote_mode_t	 	remote_mode;
	launcher_mode_e launcher_mode;
	
	int16_t  measure_left_speed;
	int16_t  measure_right_speed;
	int16_t  measure_dial_speed;
	float    measure_dial_angle;
	
	float    limit_speed;
	float    measure_launcher_speed;
	uint16_t limit_heat;
	uint16_t measure_launcher_heat;
	uint16_t remain_heat;
	
	float  target_left_speed;
	float  target_right_speed;
	float  target_dial_speed;
	float  target_dial_angle;
	float  target_heat_angle;
	
	uint8_t  vision_shoot_cnt;
	uint8_t  last_vision_shoot_cnt;
	uint8_t  shoot_cnt;
	
	uint8_t  last_s1;
	uint8_t  init_s2;
	uint8_t  last_s2;
	
	dev_work_state_t  rc_work_state;
} launcher_info_t;

typedef struct
{
 	launcher_commond_t  launcher_commond;
	fric_status_e       fric_status;
	dial_status_e       dial_status;
	uint16_t            shoot_cnt;
	uint8_t							swift_enable;
} launcher_work_info_t;

typedef struct
{
	float 	 Fric_15;
	float 	 Fric_18;
	float 	 Fric_20;
	float 	 Fric_22;
	float 	 Fric_30;
	float    fric_speed;
	uint8_t  fric_mode;
	float    dial_speed;
	float    dial_swiftspeed;
	float    dial_torque_limit;
	float    lock_angle_check;
	float    Back_Angle;
	float    Load_Angle;
	uint16_t wait_time;
} launcher_conf_t;

typedef struct 
{
	launcher_dev_t        *dev;
	launcher_info_t       *info;
	launcher_work_info_t  *work_info;
	launcher_conf_t       *conf;
	void			(*init)(void);
	void			(*ctrl)(void);
	void			(*self_protect)(void);
} launcher_t;

extern launcher_t launcher;

/* Exported functions --------------------------------------------------------*/
//void Launcher_GetInfo(void);
//void Launcher_Stop(void);
//void Launcher_RcCtrl(void);
//void Launcher_PidCtrl(void);

#endif
