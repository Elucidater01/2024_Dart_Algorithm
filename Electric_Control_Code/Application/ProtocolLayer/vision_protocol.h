#ifndef __VISION_PROTOCOL_H
#define __VISION_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "string.h"

#include "vision_sensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USART1_rxDataHandler(uint8_t *rxBuf);
	
#endif
