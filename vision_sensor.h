/**
 ******************************************************************************
 * @file    vision_sensor.h
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
#include "judge_sensor.h"
#include "rp_config.h"
#include "rp_math.h"
#include "string.h"
#include "crc.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum {
	AIM_OFF 		    = 0x00,
	AIM_ON			    = 0x01,
	RECORD				= 0x02,
} vision_cmd_e;

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  Vision information to be sent
 */
typedef __packed struct 
{
	uint8_t  			SOF;            /* Start of frame */
	uint8_t		  		mode;           /* 0:AIM_OFF 1:AIM_ON 2:RECORD */
	uint8_t  			CRC8;           /* CRC8 */
	uint8_t		  		state;          /* The operating status of the dart */
	uint8_t				number;	        /* Record the number of darts fired */
	uint8_t				gate;           /* The status of the dart gate */
	uint16_t 			CRC16;          /* CRC16 */
}vision_tx_info_t;

/**
 * @brief  Vision information to be received
 */
typedef __packed struct 
{
	uint8_t			    SOF;            /* Start of frame */
	uint8_t 			CRC8;           /* CRC8 */
	float    			yaw_error;      /* The angle between the target and the center of the camera */
	uint8_t  			target_status; 	/* 0:lost 1:find */
	uint16_t 			CRC16;          /* CRC16 */
}vision_rx_info_t;

/**
 * @brief  Vision information structure
 */
typedef struct
{
	vision_tx_info_t  *tx_info;
	vision_rx_info_t  *rx_info;
	
	uint8_t  rx_flag;
	int16_t	 offline_cnt;
	int16_t	 offline_max_cnt;
}vision_info_t;

/**
 * @brief  Vision sensor structure
 */
typedef struct vision_sensor_struct {
	vision_info_t	    *info;
	drv_uart_t		    *driver;
	void				(*init)(struct vision_sensor_struct *self);
	void				(*update)(struct vision_sensor_struct *self, uint8_t *rxBuf);
	void				(*send)(struct vision_sensor_struct *self);	
	void				(*heart_beat)(struct vision_sensor_struct *self);
	dev_work_state_t	work_state;
	dev_err_t			error;
	dev_id_t			id;
} vision_sensor_t;

/* Exported variables --------------------------------------------------------*/
extern vision_tx_info_t vision_tx_info;
extern vision_rx_info_t vision_rx_info;
extern vision_sensor_t vision_sensor;

#endif
