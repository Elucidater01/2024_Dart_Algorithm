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

extern void rc_sensor_init(rc_sensor_t *rc_sen);
extern void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf);

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void rc_sensor_check(rc_sensor_t *rc_sen);
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// 遥控器驱动
drv_uart_t	rc_sensor_driver = {
	.id = DRV_UART2,
	.tx_byte = NULL,
};

// 遥控器信息
rc_sensor_info_t 	rc_sensor_info = {
	//顺序:相同符号之间按绝对值从大到小
	.tw_step_value[RC_TB_UP] = -650,
	.tw_step_value[RC_TB_MU] = -400,
	.tw_step_value[RC_TB_DN] = +650,
	.tw_step_value[RC_TB_MD] = +400,	
	.offline_max_cnt = 60,
};

// 遥控器传感器
rc_sensor_t	rc_sensor = {
	.info = &rc_sensor_info,
	.init = rc_sensor_init,
	.update = rc_sensor_update,
	.check = rc_sensor_check,
	.heart_beat = rc_sensor_heart_beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_RC,
};

/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	遥控器数据检查
 */
static void rc_sensor_check(rc_sensor_t *rc_sen)
{
	static int16_t thumbwheel_record = 0;
	rc_sensor_info_t *rc_info = rc_sen->info;

	/*拨轮产生阶跃信号*/
	if((abs(rc_info->thumbwheel.value_last) < abs(rc_info->thumbwheel.value)) && \
		 (abs(thumbwheel_record) < abs(rc_info->thumbwheel.value)))
	{
		thumbwheel_record = rc_info->thumbwheel.value;
	}
	if((rc_info->thumbwheel.value == 0) && (thumbwheel_record != 0))
	{
			for(char i = 0;i < 4;i++)
			{
				if(rc_info->tw_step_value[i] > 0 && thumbwheel_record > 0)
				{
					if(thumbwheel_record >= rc_info->tw_step_value[i])
					{
						rc_info->thumbwheel.step[i] = !rc_info->thumbwheel.step[i];
						thumbwheel_record = 0;
					}
				}
				if(rc_info->tw_step_value[i] < 0 && thumbwheel_record < 0)
				{
					if(thumbwheel_record <= rc_info->tw_step_value[i])
					{
						rc_info->thumbwheel.step[i] = !rc_info->thumbwheel.step[i];
						thumbwheel_record = 0;
					}
				}
				
			}

		thumbwheel_record = 0;
	}
	rc_info->thumbwheel.value_last = rc_info->thumbwheel.value;
	
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
		rc_info->thumbwheel.value = 0;
		rc_info->thumbwheel.value_last = 0;
		rc_info->thumbwheel.step[RC_TB_UP] = 0;
		rc_info->thumbwheel.step[RC_TB_MU] = 0;
		rc_info->thumbwheel.step[RC_TB_MD] = 0;
		rc_info->thumbwheel.step[RC_TB_DN] = 0;
	}
	else
	{
		rc_sen->errno = NONE_ERR;
	}
}

/**
 *	@brief	遥控器心跳包
 */
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen)
{
	rc_sensor_info_t *rc_info = rc_sen->info;

	rc_info->offline_cnt++;
	if(rc_info->offline_cnt > rc_info->offline_max_cnt) {
		rc_info->offline_cnt = rc_info->offline_max_cnt;
		rc_sen->work_state = DEV_OFFLINE;
	} 
	else {
		/* 离线->在线 */
		if(rc_sen->work_state == DEV_OFFLINE)
		{
			rc_sen->work_state = DEV_ONLINE;
		}
	}
}

/* Exported functions --------------------------------------------------------*/
bool RC_IsChannelReset(void)
{
	if(  (DeathZoom(rc_sensor_info.ch0, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch1, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch2, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch3, 0, 50) == 0))	
	{
		return true;
	}
	return false;		
}

void RC_ResetData(rc_sensor_t *rc)
{
	// 通道值强行设置成中间值(不拨动摇杆的状态)
	rc->info->ch0 = 0;
	rc->info->ch1 = 0;
	rc->info->ch2 = 0;
	rc->info->ch3 = 0;
	// 左右开关选择强行设置成中间值状态
	rc->info->s1 = RC_SW_MID;
	rc->info->s2 = RC_SW_MID;
	// 鼠标
	rc->info->mouse_vx = 0;
	rc->info->mouse_vy = 0;
	rc->info->mouse_vz = 0;
	rc->info->mouse_x = 0.f;
	rc->info->mouse_y = 0.f;
	rc->info->mouse_z = 0.f;
	rc->info->mouse_btn_l.value = 0;
	rc->info->mouse_btn_r.value = 0;
	// 键盘
	rc->info->key_v = 0;
  rc->info->W.value = 0;
  rc->info->S.value = 0;
  rc->info->A.value = 0;
  rc->info->D.value = 0;
  rc->info->Shift.value = 0;
  rc->info->Ctrl.value = 0;
  rc->info->Q.value = 0;
  rc->info->E.value = 0;
  rc->info->R.value = 0;
  rc->info->F.value = 0;
  rc->info->G.value = 0;
  rc->info->Z.value = 0;
  rc->info->X.value = 0;
  rc->info->C.value = 0;
  rc->info->V.value = 0;
  rc->info->B.value = 0;
	// 左拨轮
	rc->info->thumbwheel.value = 0;
	rc->info->thumbwheel.value_last = 0;
	rc->info->thumbwheel.step[RC_TB_UP] = 0;
	rc->info->thumbwheel.step[RC_TB_MU] = 0;
	rc->info->thumbwheel.step[RC_TB_MD] = 0;
	rc->info->thumbwheel.step[RC_TB_DN] = 0;
}
