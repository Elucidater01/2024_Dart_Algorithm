#include "can_protocol.h"


/**
 *  @brief  CAN1 接收数据
 */
void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch (rxId)
	{
		case CHASSIS_CAN_ID_LF:
		{
			
			
			
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
		case RM2006_CAN_ID_203:
		{
			rm_motor_update(&rm_motor[DIAL], rxBuf);
			rm_motor_check(&rm_motor[DIAL]);
			break;
		}
	}
}
