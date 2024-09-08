#ifndef __DRV_CAN_H
#define __DRV_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "rp_driver_config.h"

extern uint8_t CAN1_200_DATA[8];
extern uint8_t CAN1_1FF_DATA[8];
extern uint8_t CAN2_200_DATA[8];
extern uint8_t CAN2_1FF_DATA[8];
/*
	CAN的接收结构体
*/
typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_RxFrameTypeDef;

void CAN_Filter_Init(void);
void CAN1_CMD_200(void);
void CAN1_CMD_1FF(void);
void CAN2_CMD_200(void);
void CAN2_CMD_1FF(void);

void CAN_BUS(CAN_HandleTypeDef hcan, uint8_t *CAN_DATA, uint32_t StdId);
#endif
