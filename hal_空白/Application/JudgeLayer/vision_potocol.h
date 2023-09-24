#ifndef __VISION_POTOCOL_H
#define __VISION_POTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "crc.h"
#include "cmsis_os.h"
/* Exported macro ------------------------------------------------------------*/
#define VISION_TX_BUFFER_LEN	50
#define VISION_FRAME_HEADER		(0xA5)
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USART1_rxDataHandler(uint8_t *rxBuf);
	
#endif

