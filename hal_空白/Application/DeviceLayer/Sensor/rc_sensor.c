/**
 * @file        rc_sensor.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-September-2020
 * @brief       Device Rc.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rc_sensor.h"

#include "rp_math.h"
#include "device.h"
#include "rc_potocol.h"

extern void rc_sensor_init(rc_sensor_t *rc_sen);
extern void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf);

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void rc_sensor_check(rc_sensor_t *rc_sen);
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// ң��������
drv_uart_t	rc_sensor_driver = {
	.type = DRV_UART2,
	.tx_byte = NULL,
};

// ң������Ϣ
rc_sensor_info_t 	rc_sensor_info = {
	.offline_max_cnt = 60,
};

// ң����������
rc_sensor_t	rc_sensor = {
	.info = &rc_sensor_info,								//���ݽṹ��
	.init = rc_sensor_init,									//��������ʼ��
	.update = rc_sensor_update,							//���ݸ���
	.check = rc_sensor_check,								//���ݺ������ж�
	.heart_beat = rc_sensor_heart_beat,			//״̬����
	.work_state = DEV_OFFLINE,							//״̬��ѯ
	.id = DEV_ID_RC,
};

/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	ң�������ݼ��
 */
static void rc_sensor_check(rc_sensor_t *rc_sen)
{
	rc_sensor_info_t *rc_info = rc_sen->info;
	
	if(abs(rc_info->ch0) > 660 ||
	   abs(rc_info->ch1) > 660 ||
	   abs(rc_info->ch2) > 660 ||
	   abs(rc_info->ch3) > 660)
	{
		rc_sen->errno = DEV_DATA_ERR;
		rc_info->ch0 = 0;
		rc_info->ch1 = 0;
		rc_info->ch2 = 0;
		rc_info->ch3 = 0;		
		rc_info->s1 = RC_SW_MID;
		rc_info->s2 = RC_SW_MID;
		rc_info->thumbwheel = 0;
	}
	else
	{
		rc_sen->errno = NONE_ERR;
	}
}

/**
 *	@brief	ң����������
 */
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen)//��������
{
	rc_sensor_info_t *rc_info = rc_sen->info;

	rc_info->offline_cnt++;
	if(rc_info->offline_cnt > rc_info->offline_max_cnt)//ÿ�εȴ�һ��ʱ����Զ�����
	{
		rc_info->offline_cnt = rc_info->offline_max_cnt;
		rc_sen->work_state = DEV_OFFLINE;
	} 
	else //ÿ�ν��ճɹ�����ռ���
	{
		/* ����->���� */
		if(rc_sen->work_state == DEV_OFFLINE)
			rc_sen->work_state = DEV_ONLINE;
	}
}

/* Exported functions --------------------------------------------------------*/
bool RC_IsChannelReset(void)//ң�ع��в�ѯ������һ�㸴λʱʹ��
{
	if(  (DeathZoom(rc_sensor_info.ch0, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch1, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch2, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch3, 0, 50) == 0)   )	
	{
		return true;
	}
	return false;		
}

void RC_ResetData(rc_sensor_t *rc)//ң��������գ�һ��ʧ��ʱʹ��
{
	// ͨ��ֵǿ�����ó��м�ֵ(������ҡ�˵�״̬)
	rc->info->ch0 = 0;
	rc->info->ch1 = 0;
	rc->info->ch2 = 0;
	rc->info->ch3 = 0;
	// ���ҿ���ѡ��ǿ�����ó��м�ֵ״̬
	rc->info->s1 = RC_SW_MID;
	rc->info->s2 = RC_SW_MID;
	// ���
	rc->info->mouse_vx = 0;
	rc->info->mouse_vy = 0;
	rc->info->mouse_vz = 0;
	rc->info->mouse_btn_l = 0;
	rc->info->mouse_btn_r = 0;
	// ����
	rc->info->key_v = 0;
	// ����
	rc->info->thumbwheel = 0;
}
