#include "can_protocol.h"
#include <string.h>

void CAN_SendAll()
{
	CAN1_CMD_200();
	CAN2_CMD_200();
	CAN1_CMD_1FF();
	CAN2_CMD_1FF();
}

void CAN_SendAllZero()
{
	memset(CAN1_200_DATA, 0, sizeof(CAN1_200_DATA));
	memset(CAN1_1FF_DATA, 0, sizeof(CAN1_1FF_DATA));
	memset(CAN2_200_DATA, 0, sizeof(CAN2_200_DATA));
	memset(CAN2_1FF_DATA, 0, sizeof(CAN2_1FF_DATA));
	
	CAN_SendAll();
}
/**
 *  @brief  CAN1 接收数据
 */
void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch (rxId)
	{
		case 0x201:
		{
			rm_motor_update(&rm_motor[BELT_R], rxBuf);
			rm_motor_check(&rm_motor[BELT_R]);
			break;
		}
		case 0x206:
		{
			rm_motor_update(&rm_motor[WIND], rxBuf);
			rm_motor_check(&rm_motor[WIND]);
			break;
		}
		case 0x203:
		{
			rm_motor_update(&rm_motor[BELT_L], rxBuf);
			rm_motor_check(&rm_motor[BELT_L]);
			break;
		}
		case 0x204:
		{
			rm_motor_update(&rm_motor[DIAL], rxBuf);
			rm_motor_check(&rm_motor[DIAL]);
			break;
		}
		
	}
}
/**
 *  @brief  CAN2 接收数据
 */
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	
	switch (canId)
	{
		case 0x205:
		{
			rm_motor_update(&rm_motor[AXIS_X], rxBuf);
			rm_motor_check(&rm_motor[AXIS_X]);
			break;
		}
		case 0x206:
		{
			rm_motor_update(&rm_motor[AXIS_ZL], rxBuf);
			rm_motor_check(&rm_motor[AXIS_ZL]);
			break;
		}
		case 0x207:
		{
			rm_motor_update(&rm_motor[AXIS_ZR], rxBuf);
			rm_motor_check(&rm_motor[AXIS_ZR]);
			break;
		}
		case 0x208:
		{
			rm_motor_update(&rm_motor[AXIS_Y], rxBuf);
			rm_motor_check(&rm_motor[AXIS_Y]);
			break;
		}
	}
}
