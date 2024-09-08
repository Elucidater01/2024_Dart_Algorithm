#include "drv_can.h"

uint8_t CAN1_200_DATA[8] = {0};
uint8_t CAN1_1FF_DATA[8] = {0};
uint8_t CAN2_200_DATA[8] = {0};
uint8_t CAN2_1FF_DATA[8] = {0};

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);


/*
	can1 can2实例
*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

CAN_RxFrameTypeDef hcan1RxFrame;
CAN_RxFrameTypeDef hcan2RxFrame;


/**
  * @brief  can接受中断，在stm32f4xx_hal_can.c内弱定义
  * @param  
  * @retval 
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  
  if(hcan == &hcan1)
  {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		
		CAN1_rxDataHandler(hcan1RxFrame.header.StdId, hcan1RxFrame.data);
  }
  else if(hcan == &hcan2)
  {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan2RxFrame.header, hcan2RxFrame.data);
		
		CAN2_rxDataHandler(hcan2RxFrame.header.StdId, hcan2RxFrame.data);
  }
  else 
  {
    return;
  }
}

void CAN1_CMD_200()
{
	CAN_TxHeaderTypeDef tx_message;
	
	uint32_t send_mail_box;
	tx_message.StdId = 0x200;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	HAL_CAN_AddTxMessage(&hcan1, &tx_message, CAN1_200_DATA, &send_mail_box);
}

void CAN2_CMD_200()
{
	CAN_TxHeaderTypeDef tx_message;
	
	uint32_t send_mail_box;
	tx_message.StdId = 0x200;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	HAL_CAN_AddTxMessage(&hcan2, &tx_message, CAN2_200_DATA, &send_mail_box);
}

void CAN1_CMD_1FF()
{
	CAN_TxHeaderTypeDef tx_message;
	
	uint32_t send_mail_box;
	tx_message.StdId = 0x1FF;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	HAL_CAN_AddTxMessage(&hcan1, &tx_message, CAN1_1FF_DATA, &send_mail_box);
}

void CAN2_CMD_1FF()
{
	CAN_TxHeaderTypeDef tx_message;
	
	uint32_t send_mail_box;
	tx_message.StdId = 0x1FF;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;

	HAL_CAN_AddTxMessage(&hcan2, &tx_message, CAN2_1FF_DATA, &send_mail_box);
}

void CAN_BUS(CAN_HandleTypeDef hcan, uint8_t *CAN_DATA, uint32_t StdId)
{
	uint32_t send_mail_box;
	
	CAN_TxHeaderTypeDef tx_message;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;
	
	tx_message.StdId = StdId;
	
	HAL_CAN_AddTxMessage(&hcan, &tx_message, CAN_DATA, &send_mail_box);
}
/* rxData Handler [Weak] functions -------------------------------------------*/
/**
 *  @brief  [__WEAK] 需要在Protocol Layer中实现具体的 CAN1 处理协议
 */
__WEAK void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
}

/**
 *  @brief  [__WEAK] 需要在Protocol Layer中实现具体的 CAN2 处理协议
 */
__WEAK void CAN2_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
}


void CAN_Filter_Init(void)
{
	HAL_CAN_Init(&hcan1);
	HAL_CAN_Init(&hcan2);
	
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;//使用FIFO0
	can_filter_st.FilterActivation = ENABLE;//使能滤波器
	can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;//设置滤波器模式
	can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;//设置比特数
    can_filter_st.FilterIdHigh = 0x0000;//高位ID
	can_filter_st.FilterIdLow = 0x0000;//低位ID
	can_filter_st.FilterMaskIdHigh = 0x0000;//高位掩码
	can_filter_st.FilterMaskIdLow = 0x0000;//低位掩码 注：此配置下没有过滤功能
	
	can_filter_st.FilterBank = 0;//CAN1 过滤器组设置
	can_filter_st.SlaveStartFilterBank = 14;//CAN2 起始过滤器组设置，CAN2是CAN1的Slaver
	HAL_CAN_ConfigFilter(&hcan1,&can_filter_st);//应用配置到CAN1
	HAL_CAN_Start(&hcan1);//启动CAN1
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN1中断
	
	can_filter_st.FilterBank = 14;//CAN2 过滤器组设置
	HAL_CAN_ConfigFilter(&hcan2,&can_filter_st);
	HAL_CAN_Start(&hcan2);//启动CAN2
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN2中断
}

