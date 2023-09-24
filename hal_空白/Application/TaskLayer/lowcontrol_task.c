#include "lowcontrol_task.h"


#include "device.h"
#include "chassis.h"
#include "cmsis_os.h"

#include "rc_sensor.h"
#include "drv_can.h"
#include "can.h"
extern short int send_outWheel_F,send_outWheel_B;//extern
extern float stand_wheel_F,stand_wheel_B;
static void FB_control(void)
{
	if(sys.remote_mode == RC)
	{
		if(co_sequent.FBco_sequent == level_3)
		{
			FBwheel_PidangleCtrl(-4000,-5000);
			if(rc_sensor.info->s2 == RC_SW_MID)
				co_sequent.FBco_sequent = level_4;//真正可以控制前后轮放下与收起
		}
		else if(co_sequent.FBco_sequent == level_4)
		{
			if(rc_sensor.info->s2 == RC_SW_MID)
			{
				FBwheel_PidangleCtrl(-4000,-5000);//轮子缩回
			}
			else if(rc_sensor.info->s2 == RC_SW_DOWN)
			{
				FBwheel_PidangleCtrl(-10000,-10000);//!!!!!!!!!!!!!!要改
			}
		}
		send_outWheel_F = (short int)stand_wheel_F;//这句话始终在执行，接收chassis的计算
		send_outWheel_B = (short int)stand_wheel_B;
	}
	else if(sys.remote_mode == KEY)
	{
		send_outWheel_F = 0;
		send_outWheel_B = 0;
	}
}
static void FB_RClost_output(void)
{
	send_outWheel_F = 0;
	send_outWheel_B = 0;
}
static int16_t Send_FB_Array[4]; 
static void FBwheel_output(void)
{
	Send_FB_Array[2] = send_outWheel_B;//0x207
	Send_FB_Array[3] = send_outWheel_F;//0x208
	CAN_SendData(&hcan1,0x1ff,Send_FB_Array);
}
void StartlowControlTask(void const * argument)
{
	for(;;)
	{
		if(sys.state == SYS_STATE_NORMAL) 
		{
			FB_control();
		}
		else if(sys.state ==SYS_STATE_RCLOST)
		{
			FB_RClost_output();
		}
		FBwheel_output();
		osDelay(13);
	}

}






