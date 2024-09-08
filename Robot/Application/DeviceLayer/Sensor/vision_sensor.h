#ifndef __VISION_SENSOR_H
#define __VISION_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rp_math.h"
#include "string.h"

#include "crc.h"
#include "rc_sensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
	AIM_OFF 		    = 0x00,	// 不启动自瞄
	AIM_ON			    = 0x01,	// 启动自瞄
	AIM_SMALL_BUFF	= 0x02,	// 识别小符
	AIM_BIG_BUFF	  = 0x03,	// 识别大符
	AIM_ANTOP	   	  = 0x04,	// 击打哨兵
	AIM_ANDF		    = 0x05	// 吊射基地
} vision_cmd_e;

typedef __packed struct 
{
	uint8_t  			SOF;
	vision_cmd_e  mode;        			// datau8_1
	uint8_t  			CRC8;
	float    			yaw_angle;	 			// dataf_1
	float    			pitch_angle; 			// dataf_2
	uint8_t  			shoot_speed; 			// datau8_2
 	uint8_t  			my_color;    			// datau8_3	0:红	1:蓝
	uint8_t				blood_0;
	uint8_t				blood_1;
	uint8_t				blood_2;
	uint8_t				blood_3;
	uint8_t				blood_4;
	uint8_t				blood_5;
	uint8_t				blood_6;
	uint8_t				blood_7;
	uint8_t				blood_8;
	uint8_t				size_3;
	uint8_t				size_4;
	uint8_t				size_5;
 	uint8_t  			is_change_target; // datau8_4
	uint16_t 			CRC16;
}vision_tx_info_t;


typedef __packed struct 
{
	uint8_t			  SOF;
	vision_cmd_e  mode;           // datau8_1
	uint8_t 			CRC8;
	float   			pitch_angle;    // dataf_1
	float    			yaw_angle;      // dataf_2
	uint8_t  			is_find_target; // datau8_2
	uint8_t  			is_find_buff; 	// datau8_3
	uint8_t  			is_hit_enable;  // datau8_4
	uint8_t  			is_spin;  			// datau8_5
	uint8_t				shoot_type;			// datau8_6 0连发 1单发
	uint8_t				armor_num;			// datau8_7
	uint8_t				armor_id;				// datau8_8
	uint16_t 			CRC16;
}vision_rx_info_t;


typedef struct
{
	vision_tx_info_t  *tx_info;
	vision_rx_info_t  *rx_info;
	
	vision_cmd_e  mode;
	vision_cmd_e  cmd_mode;
	uint8_t  			color;
	
	float    measure_pitch_angle;
	float    measure_yaw_angle;
	uint8_t  measure_shoot_speed;
	float    target_pitch_angle;
	float    target_yaw_angle;
	uint8_t  is_find_target;
	uint8_t  is_find_buff;
	uint8_t  is_hit_enable;
	uint8_t  is_spin;
	uint8_t	 shoot_type;			// datau8_6 0连发 1单发
	uint8_t	 armor_num;
	uint8_t	 armor_id;
	
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
