/**
 * @file        rc_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       DT7&DR16 Rc Protocol.
 * @update
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.ÐÞ¸Ärc_potocol.c/.h->rc_protocol.c/.h 
 */
 
#ifndef __RC_PROTOCOL_H
#define __RC_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rc_sensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USART2_rxDataHandler(uint8_t *rxBuf);
void keyboard_update_interrupt(rc_sensor_info_t	*info);
void keyboard_update(rc_sensor_info_t	*info);
	
#endif
