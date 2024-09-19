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

#define 	 WHEEL_JUMP_VALUE        (550)         //旋钮跳变判断值

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

/* 检测按键长按时间 */
#define MOUSE_BTN_L_CNT_MAX     500         //ms 鼠标左键
#define MOUSE_BTN_R_CNT_MAX     500         //ms 鼠标右键
#define KEY_Q_CNT_MAX           500         //ms Q键
#define KEY_W_CNT_MAX           300	        //ms W键
#define KEY_E_CNT_MAX           500         //ms E键
#define KEY_R_CNT_MAX           500         //ms R键
#define KEY_A_CNT_MAX           300	        //ms A键
#define KEY_S_CNT_MAX           300	        //ms S键
#define KEY_D_CNT_MAX           300	        //ms D键
#define KEY_F_CNT_MAX           500         //ms F键
#define KEY_G_CNT_MAX           500         //ms G键
#define KEY_Z_CNT_MAX           500         //ms Z键
#define KEY_X_CNT_MAX           500         //ms X键
#define KEY_C_CNT_MAX           500         //ms C键
#define KEY_V_CNT_MAX           500         //ms V键
#define KEY_B_CNT_MAX           500         //ms B键
#define KEY_SHIFT_CNT_MAX       500         //ms SHIFT键
#define KEY_CTRL_CNT_MAX        500         //ms CTRL键

/* 平滑滤波次数 */
#define REMOTE_SMOOTH_TIMES     10          //鼠标平滑滤波次数

/* ----------------------- Function Definition-------------------------------- */
/* 遥控摇杆通道偏移值 */
#define		RC_SW1_VALUE				(rc_sensor_info.s1)
#define		RC_SW2_VALUE				(rc_sensor_info.s2)
#define		RC_LEFT_CH_LR_VALUE			(rc_sensor_info.ch2)
#define		RC_LEFT_CH_UD_VALUE			(rc_sensor_info.ch3)
#define		RC_RIGH_CH_LR_VALUE			(rc_sensor_info.ch0)
#define		RC_RIGH_CH_UD_VALUE			(rc_sensor_info.ch1)
#define		RC_THUMB_WHEEL_VALUE		(rc_sensor_info.thumbwheel)

/* 检测遥控器开关状态 */
#define    IF_RC_SW1_UP      (rc_sensor_info.s1 == RC_SW_UP)
#define    IF_RC_SW1_MID     (rc_sensor_info.s1 == RC_SW_MID)
#define    IF_RC_SW1_DOWN    (rc_sensor_info.s1 == RC_SW_DOWN)
#define    IF_RC_SW2_UP      (rc_sensor_info.s2 == RC_SW_UP)
#define    IF_RC_SW2_MID     (rc_sensor_info.s2 == RC_SW_MID)
#define    IF_RC_SW2_DOWN    (rc_sensor_info.s2 == RC_SW_DOWN)

/* 获取鼠标三轴的移动速度 */
#define    MOUSE_X_MOVE_SPEED    (rc_sensor_info.mouse_vx)
#define    MOUSE_Y_MOVE_SPEED    (rc_sensor_info.mouse_vy)
#define    MOUSE_Z_MOVE_SPEED    (rc_sensor_info.mouse_vz)


/* 检测鼠标按键状态 
   按下为1，没按下为0*/
#define    MOUSE_PRESSED_LEFT    (rc_sensor_info.mouse_btn_l == 1)
#define    MOUSE_PRESSED_RIGH    (rc_sensor_info.mouse_btn_r == 1)


/* 检测键盘按键状态 
   若对应按键被按下，则逻辑表达式的值为1，否则为0 */
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
/* 按键状态枚举 */
typedef enum
{
  relax_K,        //放松
  down_K,         //按下
  up_K,           //抬起
  short_press_K,  //短按
  long_press_K,   //长按
}key_board_status_e;

/* 按键信息 */
typedef struct key_board_info_struct {
  uint8_t						 value; //值
  key_board_status_e status;            //状态
  key_board_status_e last_status;       //上一次状态
	
  int16_t cnt;                          //当前计数
  int16_t cnt_max;                      //计数上限
}key_board_info_t;

/* 拨轮信息 */
typedef struct
{
  int16_t value_last;  //上一次值
  int16_t value;       //新值
  uint8_t step[4];     //状态
}thumbwheel_info_t;

typedef struct rc_sensor_info_struct {
	/* 拨轮跳变值 */
	int16_t    tw_step_value[4];
	
	/* 遥控器 */
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	uint8_t  	s1;
	uint8_t  	s2;
	thumbwheel_info_t 			thumbwheel;						//拨轮
	/* 键鼠 */
  int16_t                 mouse_vx;             //鼠标x轴速度
  int16_t                 mouse_vy;             //鼠标y轴速度
  int16_t                 mouse_vz;             //鼠标z轴速度
  float                   mouse_x;         	    //鼠标x轴滤波后速度
  float                   mouse_y;          	  //鼠标y轴滤波后速度
  float                   mouse_z;          	  //鼠标z轴滤波后速度
  key_board_info_t        mouse_btn_l;          //鼠标左键
  key_board_info_t        mouse_btn_r;          //鼠标右键
  key_board_info_t        Q;                    //按键Q
  key_board_info_t        W;                    //按键W
  key_board_info_t        E;                    //按键E
  key_board_info_t        R;                    //按键R
  key_board_info_t        A;                    //按键A
  key_board_info_t        S;                    //按键S
  key_board_info_t        D;                    //按键D
  key_board_info_t        F;                    //按键F
  key_board_info_t        G;                    //按键G
  key_board_info_t        Z;                    //按键Z
  key_board_info_t        X;                    //按键X
  key_board_info_t        C;                    //按键C
  key_board_info_t        V;                    //按键V
  key_board_info_t        B;                    //按键B
  key_board_info_t        Shift;                //按键Shift
  key_board_info_t        Ctrl;                 //按键Ctrl
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
