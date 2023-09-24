/**
 * @file        can_potocol.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-September-2020
 * @brief       CAN Potocol.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "can_potocol.h"

#include "drv_can.h"
#include "chassis_motor.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	从CAN报文中读取电机的位置反馈
 */
static uint16_t CAN_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = ((uint16_t)rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	从CAN报文中读取电机的转子转速反馈
 */
static int16_t CAN_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = ((uint16_t)rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	从CAN报文中读取电机的实际转矩电流反馈
 */
static int16_t CAN_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = ((int16_t)rxData[4] << 8 | rxData[5]);
	return current;
}

///**
// *	@brief	从CAN报文中读取电机的实际输出转矩
// */
//static int16_t CAN_GetMotorTorque(uint8_t *rxData)
//{
//	int16_t torque;
//	torque = ((int16_t)rxData[4] << 8 | rxData[5]);
//	return torque;
//}

/**
 *	@brief	RM3508 CAN标识符
 */
static uint32_t RM3508_GetStdId(drv_can_t *drv)
{
	if((drv->can_id - 0x201U) < 4)
		return 0x200;
	else
		return 0x1FF;
}

/**
 *	@brief	RM3508 CAN数据下标
 */
static uint8_t RM3508_GetDrvId(drv_can_t *drv)
{
	return (drv->can_id - 0x201U)%4;
}

///**
// *	@brief	GM6020 CAN标识符
// */
//static uint32_t GM6020_GetStdId(drv_can_t *drv)
//{
//	if((drv->can_id - 0x205U) < 4)
//		return 0x1FF;
//	else
//		return 0x2FF;
//}

///**
// *	@brief	GM6020 CAN数据下标
// */
//static uint8_t GM6020_GetDrvId(drv_can_t *drv)
//{
//	return (drv->can_id - 0x205U)%4;
//}

///**
// *	@brief	RM2006 CAN标识符
// */
//static uint32_t RM2006_GetStdId(drv_can_t *drv)
//{
//	if((drv->can_id - 0x201U) < 4)
//		return 0x200;
//	else
//		return 0x1FF;
//}

///**
// *	@brief	RM2006 CAN数据下标
// */
//static uint8_t RM2006_GetDrvId(drv_can_t *drv)
//{
//	return (drv->can_id - 0x201U)%4;
//}

/* Exported functions --------------------------------------------------------*/
void chassis_motor_update(chassis_motor_t *chas_motor, uint8_t *rxBuf)
{
	chassis_motor_info_t *motor_info = chas_motor->info;
	
	motor_info->angle = CAN_GetMotorAngle(rxBuf);
	motor_info->speed = CAN_GetMotorSpeed(rxBuf);
	motor_info->current = CAN_GetMotorCurrent(rxBuf);
	
	motor_info->offline_cnt = 0;
}

void chassis_motor_init(chassis_motor_t *motor)
{
	drv_can_t *drv_can = motor->driver;
	
	motor->info->offline_cnt = motor->info->offline_max_cnt+1;
	motor->work_state = DEV_OFFLINE;
	
	motor->errno = NONE_ERR;
	if(motor->id == DEV_ID_CHASSIS_L) {
		drv_can->drv_id = RM3508_GetDrvId(drv_can);
		drv_can->std_id = RM3508_GetStdId(drv_can);
	}
	else if(motor->id == DEV_ID_CHASSIS_R) {
		drv_can->drv_id = RM3508_GetDrvId(drv_can);
		drv_can->std_id = RM3508_GetStdId(drv_can);
	}
	else if(motor->id == DEV_ID_CHASSIS_F) {
		drv_can->drv_id = RM3508_GetDrvId(drv_can);
		drv_can->std_id = RM3508_GetStdId(drv_can);	
	}
	else if(motor->id == DEV_ID_CHASSIS_B) {
		drv_can->drv_id = RM3508_GetDrvId(drv_can);
		drv_can->std_id = RM3508_GetStdId(drv_can);		
	}
	else {
		motor->errno = DEV_ID_ERR;
	}
}

/**
 *	@brief	CAN 发送单独数据
 */
void CAN_SendSingleData(drv_can_t *drv, int16_t txData)
{
	int16_t txArr[4] = {0, 0, 0, 0};
	
	txArr[drv->drv_id] = txData;
	if(drv->type == DRV_CAN1)
		CAN1_SendData(drv->std_id, txArr);
	else if(drv->type == DRV_CAN2)
		CAN2_SendData(drv->std_id, txArr);
}

/**
 *	@brief	CAN 发送数据缓冲
 */
void CAN_SendDataBuff(drv_type_t drv_type, uint32_t std_id, int16_t *txBuff)
{
	if(drv_type == DRV_CAN1)
		CAN1_SendData(std_id, txBuff);
	else if(drv_type == DRV_CAN2)
		CAN2_SendData(std_id, txBuff);
}

/**
 *	@brief	CAN1 接收数据
 */
void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	/* 左前轮 */
	if(canId == CHASSIS_CAN_ID_L)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_L].update(&chassis_motor[CHAS_L], rxBuf);
		chassis_motor[CHAS_L].check(&chassis_motor[CHAS_L]);
	}
	/* 右前轮 */
	else if(canId == CHASSIS_CAN_ID_R)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_R].update(&chassis_motor[CHAS_R], rxBuf);
		chassis_motor[CHAS_R].check(&chassis_motor[CHAS_R]);
	}
	/* 左后轮 */
	else if(canId == CHASSIS_CAN_ID_F)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_F].update(&chassis_motor[CHAS_F], rxBuf);
		chassis_motor[CHAS_F].check(&chassis_motor[CHAS_F]);
	}
	/* 右后轮 */
	else if(canId == CHASSIS_CAN_ID_B)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_B].update(&chassis_motor[CHAS_B], rxBuf);
		chassis_motor[CHAS_B].check(&chassis_motor[CHAS_B]);
	}
}

/**
 *	@brief	CAN2 接收数据
 */
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
}

