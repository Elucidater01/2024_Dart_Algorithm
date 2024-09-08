
#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "rc_sensor.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/*  ����״̬  */
typedef struct keyboard_state_struct {
  key_board_status_e        mouse_btn_l;          //������
  key_board_status_e        mouse_btn_r;          //����Ҽ�
  key_board_status_e        Q;                    //����Q
  key_board_status_e        W;                    //����W
  key_board_status_e        E;                    //����E
  key_board_status_e        R;                    //����R
  key_board_status_e        A;                    //����A
  key_board_status_e        S;                    //����S
  key_board_status_e        D;                    //����D
  key_board_status_e        F;                    //����F
  key_board_status_e        G;                    //����G
  key_board_status_e        Z;                    //����Z
  key_board_status_e        X;                    //����X
  key_board_status_e        C;                    //����C
  key_board_status_e        V;                    //����V
  key_board_status_e        B;                    //����B
  key_board_status_e        Shift;                //����Shift
  key_board_status_e        Ctrl;                 //����Ctrl
}keyboard_state_t;

/*  Ħ����ָ��ö��  */
typedef enum {
	fric_reset,  	  // ���ܸ�λ
	fric_on,   		  // ��Ħ����
	fric_off,   		// ��Ħ����
} fric_cmd_e;

/*  ����ָ��ö��  */
typedef enum {
	magz_reset,  	  // ���ܸ�λ
	magz_open,      // ������
	magz_close,     // �ص���
} magz_cmd_e;

/*  ����ָ��ö��  */
typedef enum {
	shoot_reset,  	// ���ܸ�λ
	single_shoot,   // ����
	keep_shoot,     // ����
	swift_shoot,		// ����
} dial_cmd_e;

/*  �������ָ��  */
typedef struct launcher_command_struct {
	fric_cmd_e		fric_cmd;
	magz_cmd_e		magz_cmd;
	dial_cmd_e		shoot_cmd;
} lch_cmd_t;

/*  ��ָ̨��  */
typedef enum {
	gim_reset,   		// ���ܸ�λ
	gim_turn,	 	    // ��ͷ
} gim_cmd_e;

/*  ��̨ģʽ  */
typedef enum {
	gyro,	 	    // ������ģʽ
	vision,   	// ����ģʽ
	keep,				// ����
} gim_mode_e;

typedef struct keyboard_struct {
	keyboard_state_t	state;
	keyboard_state_t	last_state;
} keyboard_t;

extern keyboard_t keyboard;

/* Exported functions --------------------------------------------------------*/
void Key_StateUpdate(void);


#endif
