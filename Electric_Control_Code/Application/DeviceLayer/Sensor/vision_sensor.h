/**
 ******************************************************************************
 * @file    vision_sensor.c
 * @author  RobotPilots
 * @brief   Vision sensor driver
 ******************************************************************************
 * @attention
 * 
 * Copyright 2024 RobotPilots
 * 
 * @Version     V1.0
 * @date        August-2024
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VISION_SENSOR_H
#define __VISION_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rp_math.h"
#include "string.h"
#include "crc.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
	AIM_OFF 		    = 0x00,	// 不启动自瞄
	AIM_ON			    = 0x01,	// 启动自瞄
	RECORD				= 0x02,
} vision_cmd_e;

typedef __packed struct 
{
	uint8_t  			SOF;
	uint8_t		  		mode;        		// 0:不开自瞄，1：开自瞄 2；仅录像
	uint8_t  			CRC8;
	uint8_t		  		state;
	uint8_t				number;				// 第几发飞镖，取值0--4
	uint8_t				done;
	uint16_t 			CRC16;
}vision_tx_info_t;

typedef __packed struct 
{
	uint8_t			    SOF;
	//vision_cmd_e  		mode;           // 
	uint8_t 			CRC8;
	float    			yaw_error;      // 
	uint8_t  			target_status; 	// 0:lost 1;find
	uint16_t 			CRC16;
}vision_rx_info_t;


typedef struct
{
	vision_tx_info_t  *tx_info;
	vision_rx_info_t  *rx_info;

	float    yaw_error;
	uint8_t  target_status;
	
	uint8_t  rx_flag;
	int16_t	 offline_cnt;
	int16_t	 offline_max_cnt;
}vision_info_t;

typedef struct vision_sensor_struct {
	vision_info_t	    *info;
	drv_uart_t		    *driver;
	void				     (*init)(struct vision_sensor_struct *self);
	void				     (*update)(struct vision_sensor_struct *self, uint8_t *rxBuf);
	void				     (*check)(struct vision_sensor_struct *self);	
	void				     (*heart_beat)(struct vision_sensor_struct *self);
	dev_work_state_t	work_state;
	dev_errno_t			  errno;
	dev_id_t			    id;
} vision_sensor_t;


extern vision_tx_info_t vision_tx_info;
extern vision_rx_info_t vision_rx_info;
extern vision_sensor_t vision_sensor;

/* Exported functions --------------------------------------------------------*/
extern bool vision_send_data(void);

#endif
