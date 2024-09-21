#ifndef __RC_SENSOR_H
#define __RC_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

/* Exported macro ------------------------------------------------------------*/
/* ----------------------- RC Channel Definition------------------------------*/

#define    RC_CH_VALUE_MIN       ((uint16_t)364 )
#define    RC_CH_VALUE_OFFSET    ((uint16_t)1024)
#define	   RC_CH_VALUE_MAX       ((uint16_t)1684)
#define	   RC_CH_VALUE_SIDE_WIDTH	((RC_CH_VALUE_MAX-RC_CH_VALUE_MIN)/2)

/* ----------------------- RC Switch Definition-------------------------------*/

#define    RC_SW_UP              ((uint16_t)1)
#define    RC_SW_MID             ((uint16_t)3)
#define    RC_SW_DOWN            ((uint16_t)2)

/* ----------------------- RC Thumbwheel Definition-------------------------------*/

#define    RC_TB_UP              ((uint16_t)0)
#define    RC_TB_MU              ((uint16_t)1)
#define    RC_TB_MD              ((uint16_t)3)
#define    RC_TB_DN              ((uint16_t)2)

#define 	 WHEEL_JUMP_VALUE        (550)         //��ť�����ж�ֵ

/* ----------------------- PC Key Definition-------------------------------- */

#define    KEY_PRESSED_OFFSET_W        ((uint16_t)0x01<<0)
#define    KEY_PRESSED_OFFSET_S        ((uint16_t)0x01<<1)
#define    KEY_PRESSED_OFFSET_A        ((uint16_t)0x01<<2)
#define    KEY_PRESSED_OFFSET_D        ((uint16_t)0x01<<3)
#define    KEY_PRESSED_OFFSET_SHIFT    ((uint16_t)0x01<<4)
#define    KEY_PRESSED_OFFSET_CTRL     ((uint16_t)0x01<<5)
#define    KEY_PRESSED_OFFSET_Q        ((uint16_t)0x01<<6)
#define    KEY_PRESSED_OFFSET_E        ((uint16_t)0x01<<7)
#define    KEY_PRESSED_OFFSET_R        ((uint16_t)0x01<<8)
#define    KEY_PRESSED_OFFSET_F        ((uint16_t)0x01<<9)
#define    KEY_PRESSED_OFFSET_G        ((uint16_t)0x01<<10)
#define    KEY_PRESSED_OFFSET_Z        ((uint16_t)0x01<<11)
#define    KEY_PRESSED_OFFSET_X        ((uint16_t)0x01<<12)
#define    KEY_PRESSED_OFFSET_C        ((uint16_t)0x01<<13)
#define    KEY_PRESSED_OFFSET_V        ((uint16_t)0x01<<14)
#define    KEY_PRESSED_OFFSET_B        ((uint16_t)0x01<<15)

/* ��ⰴ������ʱ�� */
#define MOUSE_BTN_L_CNT_MAX     500         //ms ������
#define MOUSE_BTN_R_CNT_MAX     500         //ms ����Ҽ�
#define KEY_Q_CNT_MAX           500         //ms Q��
#define KEY_W_CNT_MAX           300	        //ms W��
#define KEY_E_CNT_MAX           500         //ms E��
#define KEY_R_CNT_MAX           500         //ms R��
#define KEY_A_CNT_MAX           300	        //ms A��
#define KEY_S_CNT_MAX           300	        //ms S��
#define KEY_D_CNT_MAX           300	        //ms D��
#define KEY_F_CNT_MAX           500         //ms F��
#define KEY_G_CNT_MAX           500         //ms G��
#define KEY_Z_CNT_MAX           500         //ms Z��
#define KEY_X_CNT_MAX           500         //ms X��
#define KEY_C_CNT_MAX           500         //ms C��
#define KEY_V_CNT_MAX           500         //ms V��
#define KEY_B_CNT_MAX           500         //ms B��
#define KEY_SHIFT_CNT_MAX       500         //ms SHIFT��
#define KEY_CTRL_CNT_MAX        500         //ms CTRL��

/* ƽ���˲����� */
#define REMOTE_SMOOTH_TIMES     10          //���ƽ���˲�����

/* ----------------------- Function Definition-------------------------------- */
/* ң��ҡ��ͨ��ƫ��ֵ */
#define		RC_SW1_VALUE				(rc_sensor_info.s1)
#define		RC_SW2_VALUE				(rc_sensor_info.s2)
#define		RC_LEFT_CH_LR_VALUE			(rc_sensor_info.ch2)
#define		RC_LEFT_CH_UD_VALUE			(rc_sensor_info.ch3)
#define		RC_RIGH_CH_LR_VALUE			(rc_sensor_info.ch0)
#define		RC_RIGH_CH_UD_VALUE			(rc_sensor_info.ch1)
#define		RC_THUMB_WHEEL_VALUE		(rc_sensor_info.thumbwheel)

/* ���ң��������״̬ */
#define    IF_RC_SW1_UP      (rc_sensor_info.s1 == RC_SW_UP)
#define    IF_RC_SW1_MID     (rc_sensor_info.s1 == RC_SW_MID)
#define    IF_RC_SW1_DOWN    (rc_sensor_info.s1 == RC_SW_DOWN)
#define    IF_RC_SW2_UP      (rc_sensor_info.s2 == RC_SW_UP)
#define    IF_RC_SW2_MID     (rc_sensor_info.s2 == RC_SW_MID)
#define    IF_RC_SW2_DOWN    (rc_sensor_info.s2 == RC_SW_DOWN)

/* ��ȡ���������ƶ��ٶ� */
#define    MOUSE_X_MOVE_SPEED    (rc_sensor_info.mouse_vx)
#define    MOUSE_Y_MOVE_SPEED    (rc_sensor_info.mouse_vy)
#define    MOUSE_Z_MOVE_SPEED    (rc_sensor_info.mouse_vz)


/* �����갴��״̬ 
   ����Ϊ1��û����Ϊ0*/
#define    MOUSE_PRESSED_LEFT    (rc_sensor_info.mouse_btn_l == 1)
#define    MOUSE_PRESSED_RIGH    (rc_sensor_info.mouse_btn_r == 1)


/* �����̰���״̬ 
   ����Ӧ���������£����߼����ʽ��ֵΪ1������Ϊ0 */
#define    KEY_PRESSED         (  rc_sensor_info.key_v  )
#define    KEY_PRESSED_W       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_W)    != 0 )
#define    KEY_PRESSED_S       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_S)    != 0 )
#define    KEY_PRESSED_A       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_A)    != 0 )
#define    KEY_PRESSED_D       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_D)    != 0 )
#define    KEY_PRESSED_Q       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_Q)    != 0 )
#define    KEY_PRESSED_E       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_E)    != 0 )
#define    KEY_PRESSED_G       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_G)    != 0 )
#define    KEY_PRESSED_X       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_X)    != 0 )
#define    KEY_PRESSED_Z       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_Z)    != 0 )
#define    KEY_PRESSED_C       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_C)    != 0 )
#define    KEY_PRESSED_B       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_B)    != 0 )
#define    KEY_PRESSED_V       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_V)    != 0 )
#define    KEY_PRESSED_F       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_F)    != 0 )
#define    KEY_PRESSED_R       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_R)    != 0 )
#define    KEY_PRESSED_CTRL    ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_CTRL) != 0 )
#define    KEY_PRESSED_SHIFT   ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_SHIFT) != 0 )

/* Exported types ------------------------------------------------------------*/
/* ����״̬ö�� */
typedef enum
{
  relax_K,        //����
  down_K,         //����
  up_K,           //̧��
  short_press_K,  //�̰�
  long_press_K,   //����
}key_board_status_e;

/* ������Ϣ */
typedef struct key_board_info_struct {
  uint8_t						 value; //ֵ
  key_board_status_e status;            //״̬
  key_board_status_e last_status;       //��һ��״̬
	
  int16_t cnt;                          //��ǰ����
  int16_t cnt_max;                      //��������
}key_board_info_t;

/* ������Ϣ */
typedef struct
{
  int16_t value_last;  //��һ��ֵ
  int16_t value;       //��ֵ
  uint8_t step[4];     //״̬
}thumbwheel_info_t;

typedef struct rc_sensor_info_struct {
	/* ��������ֵ */
	int16_t    tw_step_value[4];
	
	/* ң���� */
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	uint8_t  	s1;
	uint8_t  	s2;
	thumbwheel_info_t 			thumbwheel;						//����
	/* ���� */
  int16_t                 mouse_vx;             //���x���ٶ�
  int16_t                 mouse_vy;             //���y���ٶ�
  int16_t                 mouse_vz;             //���z���ٶ�
  float                   mouse_x;         	    //���x���˲����ٶ�
  float                   mouse_y;          	  //���y���˲����ٶ�
  float                   mouse_z;          	  //���z���˲����ٶ�
  key_board_info_t        mouse_btn_l;          //������
  key_board_info_t        mouse_btn_r;          //����Ҽ�
  key_board_info_t        Q;                    //����Q
  key_board_info_t        W;                    //����W
  key_board_info_t        E;                    //����E
  key_board_info_t        R;                    //����R
  key_board_info_t        A;                    //����A
  key_board_info_t        S;                    //����S
  key_board_info_t        D;                    //����D
  key_board_info_t        F;                    //����F
  key_board_info_t        G;                    //����G
  key_board_info_t        Z;                    //����Z
  key_board_info_t        X;                    //����X
  key_board_info_t        C;                    //����C
  key_board_info_t        V;                    //����V
  key_board_info_t        B;                    //����B
  key_board_info_t        Shift;                //����Shift
  key_board_info_t        Ctrl;                 //����Ctrl
	uint16_t								key_v;
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;
} rc_sensor_info_t;

typedef struct rc_sensor_struct {
	rc_sensor_info_t	*info;
	drv_uart_t		  	*driver;
	void				(*init)(struct rc_sensor_struct *self);
	void				(*update)(struct rc_sensor_struct *self, uint8_t *rxBuf);
	void				(*check)(struct rc_sensor_struct *self);	
	void				(*heart_beat)(struct rc_sensor_struct *self);
	dev_work_state_t	work_state;
	dev_errno_t			errno;
	dev_id_t			id;
} rc_sensor_t;

extern rc_sensor_info_t rc_sensor_info;
extern rc_sensor_t 		rc_sensor;
/* Exported functions --------------------------------------------------------*/
bool RC_IsChannelReset(void);
void RC_ResetData(rc_sensor_t *rc);
	
#endif
