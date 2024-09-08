
/* Includes ------------------------------------------------------------------*/
#include "keyboard.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Key_StateUpdate(void);
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
keyboard_t keyboard;
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Key_StateUpdate(void)
{
	keyboard.state.mouse_btn_l = rc_sensor.info->mouse_btn_l.status;
	keyboard.state.mouse_btn_r = rc_sensor.info->mouse_btn_r.status;
	keyboard.state.Q = rc_sensor.info->Q.status;
	keyboard.state.W = rc_sensor.info->W.status;
	keyboard.state.E = rc_sensor.info->E.status;
	keyboard.state.R = rc_sensor.info->R.status;
	keyboard.state.A = rc_sensor.info->A.status;
	keyboard.state.S = rc_sensor.info->S.status;
	keyboard.state.D = rc_sensor.info->D.status;
	keyboard.state.F = rc_sensor.info->F.status;
	keyboard.state.G = rc_sensor.info->G.status;
	keyboard.state.Z = rc_sensor.info->Z.status;
	keyboard.state.X = rc_sensor.info->X.status;
	keyboard.state.C = rc_sensor.info->C.status;
	keyboard.state.V = rc_sensor.info->V.status;
	keyboard.state.B = rc_sensor.info->B.status;
	keyboard.state.Shift = rc_sensor.info->Shift.status;
	keyboard.state.Ctrl = rc_sensor.info->Ctrl.status;	
	
	keyboard.last_state.mouse_btn_l = rc_sensor.info->mouse_btn_l.last_status;
	keyboard.last_state.mouse_btn_r = rc_sensor.info->mouse_btn_r.last_status;
	keyboard.last_state.Q = rc_sensor.info->Q.last_status;
	keyboard.last_state.W = rc_sensor.info->W.last_status;
	keyboard.last_state.E = rc_sensor.info->E.last_status;
	keyboard.last_state.R = rc_sensor.info->R.last_status;
	keyboard.last_state.A = rc_sensor.info->A.last_status;
	keyboard.last_state.S = rc_sensor.info->S.last_status;
	keyboard.last_state.D = rc_sensor.info->D.last_status;
	keyboard.last_state.F = rc_sensor.info->F.last_status;
	keyboard.last_state.G = rc_sensor.info->G.last_status;
	keyboard.last_state.Z = rc_sensor.info->Z.last_status;
	keyboard.last_state.X = rc_sensor.info->X.last_status;
	keyboard.last_state.C = rc_sensor.info->C.last_status;
	keyboard.last_state.V = rc_sensor.info->V.last_status;
	keyboard.last_state.B = rc_sensor.info->B.last_status;
	keyboard.last_state.Shift = rc_sensor.info->Shift.last_status;
	keyboard.last_state.Ctrl = rc_sensor.info->Ctrl.last_status;	
}
