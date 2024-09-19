/**
 * @file        rm_protocol.c
 * @author      RobotPilots
 * @Version     v1.0.3    
 * @brief       RM Motor Protocol.
 * @update      
 *              v1.0(24-October-2021)
 *              v1.0.1(8-November-2021)
 *                  1.新增CAN电调发送报文协议
 *              v1.0.2(13-November-2021)
 *                  1.修复电机发送数据下标不正确的bug
 *              v1.0.3(13-November-2021)
 *                  1.宏定义中ID_202的值赋值错误成201，已修复
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rm_protocol.h"
#include "rm_motor.h"
#include "drv_can.h"
#include "rp_math.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* RM电机 标识符协议 ---------------------------------------------------------*/
/**
*	@brief	RM3508 CAN发送标识符
 */
uint32_t RM3508_GetTxId(drv_can_t *drv)
{
	if((drv->rx_id - 0x201U) < 4)
		return 0x200;
	else
		return 0x1FF;
}

/**
 *	@brief	RM3508 CAN发送数据下标
 */
uint8_t RM3508_GetDataId(drv_can_t *drv)
{
	return ((drv->rx_id - 0x201U)%4 * 2);
}

/**
 *	@brief	GM6020 CAN发送标识符
 */
uint32_t GM6020_GetTxId(drv_can_t *drv)
{
	if((drv->rx_id - 0x205U) < 4)
		return 0x1FF;
	else
		return 0x2FF;
}

/**
 *	@brief	GM6020 CAN发送数据下标
 */
uint8_t GM6020_GetDataId(drv_can_t *drv)
{
	return ((drv->rx_id - 0x205U)%4 * 2);
}

/**
 *	@brief	RM2006 CAN发送标识符
 */
uint32_t RM2006_GetTxId(drv_can_t *drv)
{
	if((drv->rx_id - 0x201U) < 4)
		return 0x200;
	else
		return 0x1FF;
}

/**
 *	@brief	RM2006 CAN发送数据下标
 */
uint8_t RM2006_GetDataId(drv_can_t *drv)
{
	return ((drv->rx_id - 0x201U)%4 * 2);
}

/**
 *	@brief	RM3510 CAN发送标识符
 */
uint32_t RM3510_GetTxId(drv_can_t *drv)
{
    return 0x200;
}

/**
 *	@brief	RM3510 CAN发送数据下标
 */
uint8_t RM3510_GetDataId(drv_can_t *drv)
{
	return ((drv->rx_id - 0x201U)%4 * 2);
}

/**
 *	@brief	GM3510 CAN发送标识符
 */
uint32_t GM3510_GetTxId(drv_can_t *drv)
{
    return 0x1FF;
}

/**
 *	@brief	GM3510 CAN发送数据下标
 */
uint8_t GM3510_GetDataId(drv_can_t *drv)
{
	return ((drv->rx_id - 0x205U)%4 * 2);
}

/* RM电机 CAN电调反馈报文协议 --------------------------------------------------------*/
/**
 *	@brief	从CAN报文[0][1]中读取电机的位置反馈
 */
uint16_t CAN_01_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = (uint16_t)(rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	从CAN报文[2][3]中读取电机的转子转速反馈
 */
int16_t CAN_23_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = (int16_t)(rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	从CAN报文[4][5]中读取电机的实际转矩电流反馈
 */
int16_t CAN_45_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = (int16_t)(rxData[4] << 8 | rxData[5]);
	return current;
}

/**
 *	@brief	从CAN报文[2][3]中读取电机的实际输出转矩
 */
int16_t CAN_23_GetMotorTorque(uint8_t *rxData)
{
	int16_t torque;
	torque = ((uint16_t)rxData[2] << 8 | rxData[3]);
	return torque;
}

/**
 *	@brief	从CAN报文[4][5]中读取电机的实际输出转矩
 */
int16_t CAN_45_GetMotorTorque(uint8_t *rxData)
{
	int16_t torque;
	torque = ((uint16_t)rxData[4] << 8 | rxData[5]);
	return torque;
}

/**
 *	@brief	从CAN报文[6]中读取电机的实际温度
 */
uint8_t CAN_6_GetMotorTemperature(uint8_t *rxData)
{
	uint8_t temp;
	temp = rxData[6];
	return temp;
}

/* RM电机 CAN电调发送报文协议 --------------------------------------------------------*/
/**
*	@brief	CAN报文发送电机的控制电流/电压
 */
void CAN_AddMotorOutput(drv_can_t *drv, uint16_t txData)
{
//    uint8_t data[2];
//    data[0] = (uint8_t)(txData >> 8);
//    data[1] = (uint8_t)(txData);
//	CAN_AddMsgByDriver(drv, data, 2);
}

/* RM电机 设备层协议 --------------------------------------------------------*/
/**
 *	@brief	解析RM标准电机的角度、速度、转矩电流与温度
 */
void rm_motor_update(rm_motor_t *rm_motor, uint8_t *rxBuf)
{
    rm_motor_info_t *motor_info = rm_motor->info;
    
    motor_info->angle = CAN_GetMotorAngle(rxBuf);
	motor_info->speed = CAN_GetMotorSpeed(rxBuf);
	motor_info->current = CAN_GetMotorCurrent(rxBuf);
    motor_info->temperature = CAN_GetMotorTemperature(rxBuf);
    
    motor_info->offline_cnt = 0;
}

/**
 *	@brief	校验RM标准电机的数据(简化为计算转过的角度)
 */
void rm_motor_check(rm_motor_t *motor)
{
	int16_t err;
	rm_motor_info_t *motor_info = motor->info;
	
	/* 未初始化 */
	if( !motor_info->init_flag )
	{
		motor_info->init_flag = true;
		motor_info->angle_prev = motor_info->angle;
		motor_info->angle_sum = 0;
	}
	
	err = motor_info->angle - motor_info->angle_prev;
	
	/* 过零点 */
	if(abs(err) > 4095)
	{
		/* 0↓ -> 8191 */
		if(err >= 0)
			motor_info->angle_sum += -8191 + err;
		/* 8191↑ -> 0 */
		else
			motor_info->angle_sum += 8191 + err;
	}
	/* 未过零点 */
	else
	{
		motor_info->angle_sum += err;
	}
	
	motor_info->angle_prev = motor_info->angle;    
}
