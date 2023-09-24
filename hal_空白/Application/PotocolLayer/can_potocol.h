#ifndef __CAN_POTOCOL_H
#define __CAN_POTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_CAN_ID_L	0x202U
#define CHASSIS_CAN_ID_R	0x201U
#define CHASSIS_CAN_ID_F	0x208U//Ç°ÂÖ
#define CHASSIS_CAN_ID_B	0x207U//ºóÂÖ

/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void	CAN_SendSingleData(drv_can_t *drv, int16_t txData);
void 	CAN_SendDataBuff(drv_type_t drv_type, uint32_t std_id, int16_t *txBuff);

#endif
