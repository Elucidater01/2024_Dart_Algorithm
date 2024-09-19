/**
 * @file        rp_device_config.h
 * @author      RobotPilots
 * @Version     v1.0
 * @brief       RobotPilots Robots' Device Configuration.
 * @update
 *              v1.0(7-November-2021)  
 */
#ifndef __RP_DEVICE_CONFIG_H
#define __RP_DEVICE_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "rp_driver_config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* 设备层 --------------------------------------------------------------------*/
/**
 *	@brief	设备id列表
 *	@class	device
 */
typedef enum {
	DEV_ID_IMU = 0,
	DEV_ID_IMU_EX,
	DEV_ID_RC,
	DEV_ID_JUDGE,
	DEV_ID_VISION,
	DEV_ID_CNT,
} dev_id_t;

/**
 *	@brief	设备工作状态(通用)
 *	@class	device
 */
typedef enum {
	DEV_ONLINE,
	DEV_OFFLINE,
} dev_work_state_t;

/**
 *	@brief	错误代码(通用)
 *  @note   可自定义设备错误代码类型并替换变量errno的变量类型，如
 *          typedef enum {
 *              IMU_NONE_ERR,
 *              IMU_ID_ERR,
 *              IMU_COM_FAILED,
 *              IMU_DEV_NOT_FOUND,
 *              ...
 *          } imu_errno_t;
 *          
 *          typedef struct imu_sensor_struct {
 *              ...
 *	            imu_errno_t errno;
 *              ...	
 *          } imu_sensor_t;
 *	@class	device
 */
typedef enum {
	NONE_ERR,		// 正常(无错误)
	DEV_ID_ERR,		// 设备ID错误
	DEV_INIT_ERR,	// 设备初始化错误
	DEV_DATA_ERR,	// 设备数据错误
} dev_errno_t;

typedef enum {
	BELT_R,		//右皮带电机（因为它固定在皮带右边），负责拉飞镖储存弹性势能
	WIND,		//挂着飞镖的风车
	BELT_L,		//左皮带电机，负责把风车上的飞镖拨进发射槽
	DIAL,		//控制拨飞镖的挡板
	AXIS_X,		//X轴朝向
	AXIS_ZL,	//左Z轴朝向（以飞镖打出去的方向为前）
	AXIS_ZR,	//右Z轴朝向
	AXIS_Y,		//控制绑着皮筋位置的电机，控制拉力
	MOTOR_LIST,
} dev_motor_list_e;

/**
 *	@brief	设备结构体定义模板
 *	@class	device
 */
typedef struct device {
	void				        *info;		                                    // 自定义具体设备信息结构体
	void				        *driver;	                                    // 自定义具体设备驱动结构体
	void				        (*init)(struct device *self);	                // 设备初始化函数
	void				        (*update)(struct device *self, uint8_t *rxBuf);	// 设备数据更新函数
	void				        (*check)(struct device *self);	                // 设备数据检查函数
	void				        (*heart_beat)(struct device *self);	            // 设备心跳包
	volatile dev_work_state_t	work_state;	                                    // 设备工作状态
	volatile dev_errno_t	  	errno;		                                    // 可自定义具体设备错误代码
	const    dev_id_t		    	id;			                                // 设备id
} device_t;

#endif
