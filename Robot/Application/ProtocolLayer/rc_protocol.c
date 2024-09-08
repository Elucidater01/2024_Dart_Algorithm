/**
 * @file        rc_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       DT7&DR16 Rc Protocol.
 * @update
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.修改rc_potocol.c/.h->rc_protocol.c/.h 
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"
#include "rp_math.h"

#include "rc_sensor.h"
#include "keyboard.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void keyboard_cnt_max_set(rc_sensor_t *rc_sen);
void keyboard_status_update_interrupt(key_board_info_t *key);
void keyboard_status_update(key_board_info_t *key);

extern uint32_t micros(void);
uint32_t tt1, tt2, ttp1;
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void rc_sensor_init(rc_sensor_t *rc_sen)
{
	// 初始化为离线状态
	rc_sen->info->offline_cnt = rc_sen->info->offline_max_cnt + 1;
	rc_sen->work_state = DEV_OFFLINE;
	
	RC_ResetData(rc_sen);
	keyboard_cnt_max_set(rc_sen);
	
	if(rc_sen->id == DEV_ID_RC)
		rc_sen->errno = NONE_ERR;
	else
		rc_sen->errno = DEV_ID_ERR;
}

/**
  * @brief  按键长按时间设置
  */
void keyboard_cnt_max_set(rc_sensor_t *rc_sen)
{
	rc_sensor_info_t *info = rc_sen->info;
	
  info->mouse_btn_l.cnt_max = MOUSE_BTN_L_CNT_MAX;
  info->mouse_btn_r.cnt_max = MOUSE_BTN_R_CNT_MAX;
  info->Q.cnt_max = KEY_Q_CNT_MAX;
  info->W.cnt_max = KEY_W_CNT_MAX;
  info->E.cnt_max = KEY_E_CNT_MAX;
  info->R.cnt_max = KEY_R_CNT_MAX;
  info->A.cnt_max = KEY_A_CNT_MAX;
  info->S.cnt_max = KEY_S_CNT_MAX;
  info->D.cnt_max = KEY_D_CNT_MAX;
  info->F.cnt_max = KEY_F_CNT_MAX;
  info->G.cnt_max = KEY_G_CNT_MAX;
  info->Z.cnt_max = KEY_Z_CNT_MAX;
  info->X.cnt_max = KEY_X_CNT_MAX;
  info->C.cnt_max = KEY_C_CNT_MAX;
  info->V.cnt_max = KEY_V_CNT_MAX;
  info->B.cnt_max = KEY_B_CNT_MAX;
  info->Shift.cnt_max = KEY_SHIFT_CNT_MAX;
  info->Ctrl.cnt_max = KEY_CTRL_CNT_MAX;
}

/**
  * @brief  鼠标数据更新
  */
void rc_interrupt_update(rc_sensor_t *rc_sen)
{
	/* 鼠标速度均值滤波 */
	static int16_t mouse_x[REMOTE_SMOOTH_TIMES], mouse_y[REMOTE_SMOOTH_TIMES];
	static int16_t index = 0;
	if(index == REMOTE_SMOOTH_TIMES)
	{
		index = 0;
	}
	rc_sen->info->mouse_x -= (float)mouse_x[index] / (float)REMOTE_SMOOTH_TIMES;
	rc_sen->info->mouse_y -= (float)mouse_y[index] / (float)REMOTE_SMOOTH_TIMES;
	mouse_x[index] = rc_sen->info->mouse_vx;
	mouse_y[index] = rc_sen->info->mouse_vy;
	rc_sen->info->mouse_x += (float)mouse_x[index] / (float)REMOTE_SMOOTH_TIMES;
	rc_sen->info->mouse_y += (float)mouse_y[index] / (float)REMOTE_SMOOTH_TIMES;
	
	index++;
	//symbal.rc_update = 1;
	
}
/**
 *	@brief	遥控器数据解析协议
 */
void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf)
{
	rc_sensor_info_t *rc_info = rc_sen->info;
	/* 遥控器 */
	rc_info->ch0 = (rxBuf[0] | rxBuf[1] << 8) & 0x07FF;
	rc_info->ch0 -= 1024;
	rc_info->ch1 = (rxBuf[1] >> 3 | rxBuf[2] << 5) & 0x07FF;
	rc_info->ch1 -= 1024;
	rc_info->ch2 = (rxBuf[2] >> 6 | rxBuf[3] << 2 | rxBuf[4] << 10) & 0x07FF;
	rc_info->ch2 -= 1024;
	rc_info->ch3 = (rxBuf[4] >> 1 | rxBuf[5] << 7) & 0x07FF;
	rc_info->ch3 -= 1024;

	rc_info->thumbwheel.value = ((int16_t)rxBuf[16] | ((int16_t)rxBuf[17] << 8)) & 0x07ff;
	rc_info->thumbwheel.value -= 1024;

	rc_info->s1 = ((rxBuf[5] >> 4) & 0x000C) >> 2;
	rc_info->s2 = (rxBuf[5] >> 4) & 0x0003;	
	
	/* 键鼠 */
	rc_info->mouse_vx = rxBuf[6]  | (rxBuf[7 ] << 8);
	rc_info->mouse_vy = rxBuf[8]  | (rxBuf[9 ] << 8);
	rc_info->mouse_vz = rxBuf[10] | (rxBuf[11] << 8);
  rc_info->mouse_btn_l.value = rxBuf[12] & 0x01;
  rc_info->mouse_btn_r.value = rxBuf[13] & 0x01;
	rc_info->key_v   =  rxBuf[14] | (rxBuf[15] << 8);
	
  rc_info->W.value = 	KEY_PRESSED_W;
  rc_info->S.value =  KEY_PRESSED_S;
  rc_info->A.value = 	KEY_PRESSED_A;
  rc_info->D.value = 	KEY_PRESSED_D;
  rc_info->Shift.value = 	KEY_PRESSED_SHIFT;
  rc_info->Ctrl.value  = 	KEY_PRESSED_CTRL;
  rc_info->Q.value = 	KEY_PRESSED_Q;
  rc_info->E.value = 	KEY_PRESSED_E;
  rc_info->R.value = 	KEY_PRESSED_R;
  rc_info->F.value = 	KEY_PRESSED_F;
  rc_info->G.value = 	KEY_PRESSED_G;
  rc_info->Z.value = 	KEY_PRESSED_Z;
  rc_info->X.value = 	KEY_PRESSED_X;
  rc_info->C.value = 	KEY_PRESSED_C;
  rc_info->V.value = 	KEY_PRESSED_V;
  rc_info->B.value = 	KEY_PRESSED_B;
	
	rc_info->offline_cnt = 0;
	tt1 = tt2;
	tt2 = micros();
	ttp1 = tt2 - tt1;
	
}

/**
 *	@brief	在遥控器中断中更新键盘状态
 */
void keyboard_update_interrupt(rc_sensor_info_t	*info)
{
  keyboard_status_update_interrupt(&info->mouse_btn_l);
  keyboard_status_update_interrupt(&info->mouse_btn_r);
  keyboard_status_update_interrupt(&info->Q);
  keyboard_status_update_interrupt(&info->W);
  keyboard_status_update_interrupt(&info->E);
  keyboard_status_update_interrupt(&info->R);
  keyboard_status_update_interrupt(&info->A);
  keyboard_status_update_interrupt(&info->S);
  keyboard_status_update_interrupt(&info->D);
  keyboard_status_update_interrupt(&info->F);
  keyboard_status_update_interrupt(&info->G);
  keyboard_status_update_interrupt(&info->Z);
  keyboard_status_update_interrupt(&info->X);
  keyboard_status_update_interrupt(&info->C);
  keyboard_status_update_interrupt(&info->V);
  keyboard_status_update_interrupt(&info->B);
  keyboard_status_update_interrupt(&info->Shift);
  keyboard_status_update_interrupt(&info->Ctrl);
}

/**
 *	@brief	在遥控器中断中更新键盘按键状态
 */
void keyboard_status_update_interrupt(key_board_info_t *key)
{
	key->last_status = key->status;
  switch(key->status)
  {
    case relax_K:
      if(key->value == 1)
      {
        key->status = down_K;
        key->cnt = 0;
      }
      break;
    case short_press_K:
      if(key->value == 0)
      {
        key->status = up_K;
				key->cnt = 0;
      }
      else if(key->value == 1)
      {
        key->cnt++;
        if(key->cnt >= key->cnt_max)
        {
          key->status = long_press_K;
					key->cnt = key->cnt_max;
        }
      }
      break;
    case long_press_K:
      if(key->value == 0)
      {
        key->status = up_K;
				key->cnt = 0;
      }
      break;
    default:
      break;
  }
}

/**
 *	@brief	更新键盘状态
 */
void keyboard_update(rc_sensor_info_t	*info)
{
  keyboard_status_update(&info->mouse_btn_l);
  keyboard_status_update(&info->mouse_btn_r);
  keyboard_status_update(&info->Q);
  keyboard_status_update(&info->W);
  keyboard_status_update(&info->E);
  keyboard_status_update(&info->R);
  keyboard_status_update(&info->A);
  keyboard_status_update(&info->S);
  keyboard_status_update(&info->D);
  keyboard_status_update(&info->F);
  keyboard_status_update(&info->G);
  keyboard_status_update(&info->Z);
  keyboard_status_update(&info->X);
  keyboard_status_update(&info->C);
  keyboard_status_update(&info->V);
  keyboard_status_update(&info->B);
  keyboard_status_update(&info->Shift);
  keyboard_status_update(&info->Ctrl);
}

/**
 *	@brief	更新键盘按键状态
 */
void keyboard_status_update(key_board_info_t *key)
{
	key->last_status = key->status;
  switch(key->status)
  {
    case down_K:
      key->status = short_press_K;
      key->cnt++;
      break;
    case up_K:
      key->status = relax_K;
			key->cnt = 0;
      break;
    case short_press_K:
      key->cnt++;
      if(key->cnt >= key->cnt_max)
      {
        key->value = long_press_K;
				key->cnt = key->cnt_max;
      }
      break;
    default:
      break;
  }
}

/**
 *	@brief	在串口2中解析遥控数据协议
 */
void USART2_rxDataHandler(uint8_t *rxBuf)
{
	// 更新遥控数据
	rc_sensor.update(&rc_sensor, rxBuf);
	rc_sensor.check(&rc_sensor);
	
	rc_interrupt_update(&rc_sensor);
	
}
