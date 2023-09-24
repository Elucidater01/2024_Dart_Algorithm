#ifndef __VISION_SENSOR_H
#define __VISION_SENSOR_H
/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "drv_uart.h"
/* Exported macro ------------------------------------------------------------*/
//��ʼ�ֽڣ�Э��̶�β0xA5
#define 	VISION_SOF					(0xA5)
//���ȸ���Э�鶨��
#define		VISION_LEN_HEADER		3			//֡ͷ��
#define 	VISION_LEN_DATA 		18    //���ݶγ��ȣ����Զ���
#define     VISION_LEN_TAIL			2			//ƵβCRC16
#define		VISION_LEN_PACKED		23		//���ݰ����ȣ����Զ���

/* Exported types ------------------------------------------------------------*/
/**
 *	���->�Ӿ�
 */
typedef enum {
	CMD_AIM_OFF 		= 0x00,	// ����������
	CMD_AIM_AUTO		= 0x01,	// ��������
	CMD_AIM_SMALL_BUFF	= 0x02,	// ʶ��С��
	CMD_AIM_BIG_BUFF	= 0x03,	// ʶ����
	CMD_AIM_SENTRY		= 0x04,	// �����ڱ�
	CMD_AIM_BASE		= 0x05	// �������
} Vision_Cmd_Id_t;
/* ֡ͷ�ֽ�ƫ�� */
typedef enum {
	sof			= 0,
	Cmd_ID		= 1,
	Crc8		= 2,
	Data		= 3,
	TX_CRC16	= 20,
} Vision_Frame_Header_Offset_t;

/* ���ݳ��� */
typedef enum {
	/* Std */
	LEN_FRAME_HEADER 		= 3,	// ֡ͷ����
	LEN_RX_DATA 			= 18,	// �������ݶγ���
	LEN_TX_DATA 			= 17,	// �������ݶγ���
	LEN_FRAME_TAILER 		= 2,	// ֡βCRC16
	LEN_VISION_RX_PACKET	= 325,	// ���հ���������23
	LEN_VISION_TX_PACKET	= 22,	// ���Ͱ���������22
} Vision_Data_Length_t;

/* ֡ͷ��ʽ */
typedef __packed struct
{
	uint8_t  			sof;		// ͬ��ͷ
	Vision_Cmd_Id_t  	cmd_id;		// ������
	uint8_t  			crc8;		// CRC8У����
} Vision_Frame_Header_t;

/* ֡β��ʽ */
typedef __packed struct 
{
	uint16_t crc16;					// CRC16У����
} Vision_Frame_Tailer_t;
/* �������ݶθ�ʽ */
typedef __packed struct 
{
//	float 	pitch_angle;	// pitchƫ��Ƕ�/���ص�	��λ���Ƕ�/���ص�
//	float 	yaw_angle;		// yawƫ��Ƕ�/���ص�	��λ���Ƕ�/���ص�
//	float 	distance;			// ����				��λ��mm
//	char change_for;// �Ƿ�ʶ��Ŀ��	��λ��0/1
//	char is_find_Target;	// �Ƿ�ʶ��С����	��λ��0/1	
//	char is_find_Dafu;	// �Ƿ�ʶ��Buff	��λ��0/1
//	char near_face;         //����״̬���Ƿ���
//	char anti_top;	// Ӣ��ʶ���־λ��1ΪӢ�ۣ�0ΪСװ�װ�
//	char anti_top_change_armor;
	char data[LEN_VISION_RX_PACKET-5];
	float jiesuan[(LEN_VISION_RX_PACKET-5)/4];
} Vision_Rx_Data_t;//3float,6char
/* ���հ���ʽ */
typedef __packed struct 
{
	Vision_Frame_Header_t FrameHeader;	// ֡ͷ
	Vision_Rx_Data_t	  RxData;		// ����
	Vision_Frame_Tailer_t FrameTailer;	// ֡β	
} Vision_Rx_Packet_t;

/* �������ݶθ�ʽ */
typedef __packed struct
{
	uint8_t buff_shoot_four;// �����ʱ��������Ŀ��ӵ�
	uint8_t fric_speed;		// ���ٵ�λ(���ݵȼ�����)
	uint8_t my_color;		// ���Լ�����ɫ
} Vision_Tx_Data_t;
/* ���Ͱ���ʽ */
typedef __packed struct
{
	Vision_Frame_Header_t FrameHeader;	// ֡ͷ
	Vision_Tx_Data_t	  TxData;		// ����
	Vision_Frame_Tailer_t FrameTailer;	// ֡β		
} Vision_Tx_Packet_t;
/**
 *	@brief	�Ӿ�ģʽ
 */
typedef enum
{
	VISION_MODE_MANUAL		= 0,	// �ֶ�ģʽ
	VISION_MODE_AUTO		= 1,	// ����ģʽ
	VISION_MODE_BIG_BUFF	= 2,	// ����ģʽ
	VISION_MODE_SMALL_BUFF	= 3,	// ��С��ģʽ
} Vision_Mode_t;
/* ������ʶ���� */
typedef struct
{
	uint8_t 		my_color;			// ��0/1��ʾ��ɫ
	Vision_Mode_t	mode;				// �Ӿ�ģʽ
	uint8_t  		rx_data_valid;		// �������ݵ���ȷ��
	uint16_t 		rx_err_cnt;			// �������ݵĴ���ͳ��
	uint32_t		rx_cnt;				// �������ݰ���ͳ��
	bool		    rx_data_update;		// ���������Ƿ����
	uint32_t 		rx_time_prev;		// �������ݵ�ǰһʱ��
	uint32_t 		rx_time_now;		// �������ݵĵ�ǰʱ��
	uint16_t 		rx_time_fps;		// ֡��
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;	
} Vision_State_t;

/* �Ӿ�ͨ�����ݰ���ʽ */
typedef struct {
	Vision_Rx_Packet_t RxPacket;
	Vision_Tx_Packet_t TxPacket;
	Vision_State_t     State;
} vision_info_t;

typedef struct vision_sensor_struct {
	vision_info_t	*info;
	drv_uart_t			*driver;
	void				(*init)(struct vision_sensor_struct *self);
	void				(*update)(struct vision_sensor_struct *self, uint8_t *rxBuf);
	void				(*check)(struct vision_sensor_struct *self);	
	void				(*heart_beat)(struct vision_sensor_struct *self);
	dev_work_state_t	work_state;
	dev_errno_t			errno;
	dev_id_t			id;
} vision_sensor_t;

extern vision_info_t    vision_sensor_info;
extern vision_sensor_t	vision_sensor;
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



























#endif
