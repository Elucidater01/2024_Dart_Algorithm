#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "driver.h"
#include "device.h"

#define SLAVE_TX_ID 0x100
#define SLAVE_RX_ID 0x101

/*底盘电机3508时*/
#define CHASSIS_CAN_ID_LF			 RM3508_CAN_ID_201
#define CHASSIS_CAN_ID_RF			 RM3508_CAN_ID_202
#define CHASSIS_CAN_ID_LB			 RM3508_CAN_ID_203
#define CHASSIS_CAN_ID_RB			 RM3508_CAN_ID_204


/*底盘电机9025*/
#define CHASSIS_CAN_ID_LEG_L   KT9025_CAN_ID_141
#define CHASSIS_CAN_ID_LEG_R   KT9025_CAN_ID_142


/*动量电机*/
#define MOMENTUM_CAN_ID_L      GM6020_CAN_ID_205 
#define MOMENTUM_CAN_ID_R 		 GM6020_CAN_ID_206

void CAN_SendAll(void);
void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif
