
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "rc_sensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/*  键盘状态  */
typedef struct keyboard_state_struct {
  key_board_status_e        mouse_btn_l;          //鼠标左键
  key_board_status_e        mouse_btn_r;          //鼠标右键
  key_board_status_e        Q;                    //按键Q
  key_board_status_e        W;                    //按键W
  key_board_status_e        E;                    //按键E
  key_board_status_e        R;                    //按键R
  key_board_status_e        A;                    //按键A
  key_board_status_e        S;                    //按键S
  key_board_status_e        D;                    //按键D
  key_board_status_e        F;                    //按键F
  key_board_status_e        G;                    //按键G
  key_board_status_e        Z;                    //按键Z
  key_board_status_e        X;                    //按键X
  key_board_status_e        C;                    //按键C
  key_board_status_e        V;                    //按键V
  key_board_status_e        B;                    //按键B
  key_board_status_e        Shift;                //按键Shift
  key_board_status_e        Ctrl;                 //按键Ctrl
}keyboard_state_t;

/*  摩擦轮指令枚举  */
typedef enum {
	fric_reset,  	  // 功能复位
	fric_on,   		  // 开摩擦轮
	fric_off,   		// 关摩擦轮
} fric_cmd_e;

/*  弹仓指令枚举  */
typedef enum {
	magz_reset,  	  // 功能复位
	magz_open,      // 开弹仓
	magz_close,     // 关弹仓
} magz_cmd_e;

/*  拨盘指令枚举  */
typedef enum {
	shoot_reset,  	// 功能复位
	single_shoot,   // 单发
	keep_shoot,     // 连发
	swift_shoot,		// 速射
} dial_cmd_e;

/*  发射机构指令  */
typedef struct launcher_command_struct {
	fric_cmd_e		fric_cmd;
	magz_cmd_e		magz_cmd;
	dial_cmd_e		shoot_cmd;
} lch_cmd_t;

/*  云台指令  */
typedef enum {
	gim_reset,   		// 功能复位
	gim_turn,	 	    // 掉头
} gim_cmd_e;

/*  云台模式  */
typedef enum {
	gyro,	 	    // 陀螺仪模式
	vision,   	// 自瞄模式
	keep,				// 保持
} gim_mode_e;

typedef struct keyboard_struct {
	keyboard_state_t	state;
	keyboard_state_t	last_state;
} keyboard_t;

extern keyboard_t keyboard;

/* Exported functions --------------------------------------------------------*/
void Key_StateUpdate(void);


#endif
