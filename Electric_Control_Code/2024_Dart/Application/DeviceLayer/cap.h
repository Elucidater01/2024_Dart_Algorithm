#ifndef __CAP_H
#define __CAP_H

#include "type.h"

typedef struct 
{
    uint16_t chassis_power_buffer;  //底盘功率缓冲，0~60J
    uint16_t chassis_power_limit;   //机器人底盘功率限制上限，0~120W
    int16_t  output_power_limit;     //电容放电功率限制，-120~300W
    uint16_t input_power_limit;     //电容充电功率限制，0~150W
    uint16_t chassis_volt;			//底盘输出电压 单位 毫伏 **
	  uint16_t chassis_current;		//底盘输出电流 单位 毫安 **
 
		union{
					uint16_t all;
					struct
					{
							uint16_t cap_switch : 1;    //电容开关
							uint16_t cap_record : 1;    //记录功能开关
              uint16_t gamegoing : 1;     //比赛进行中为1，否则为0 ***						
					}bit;
			}cap_control;
		
}cap_send_data_t;

typedef struct
{
    float cap_Ucr;    //电容两端电压Ucr，0~30V
    float cap_I;    //电容电流I，-20~20A
    union
    {
        uint16_t state;     //电容状态
        struct
        {
            uint16_t warning : 1;   //报警
            uint16_t cap_U_over : 1;    //电容过压
            uint16_t cap_I_over : 1;    //电容过流
            uint16_t cap_U_low : 1;     //电容欠压
            uint16_t bat_I_low : 1;     //裁判系统欠压
            uint16_t can_receive_miss : 1;    //电容未接收到CAN通信数据
        }bit;
    }cap_state;
}cap_receive_data_t;



typedef enum {
	
	CAP_ONLINE,
	CAP_OFFLINE,

} cap_state_e;



typedef struct cap_info_struct {
	
	uint16_t	canId;
	
	cap_send_data_t     tx;
	cap_receive_data_t  rx;

	int16_t 	Buff0x2E[4];
	int16_t 	Buff0x2F[4];

	uint8_t   offline_cnt;
	uint8_t   offline_max_cnt;

} cap_info_t;


typedef struct cap_struct{

	cap_info_t 	info;

	void				(*modifyLimit)(struct cap_struct *self,int16_t out,int16_t in);	
	void				(*setdata)(struct cap_struct *self,uint16_t powerBuff,uint16_t powerLimit,uint16_t volt,uint16_t current);
	void				(*update)(struct cap_struct *self,uint32_t canId, uint8_t *rxBuf);
	void				(*heart_beat)(struct cap_struct *self);
	
	cap_state_e	 state;
	
}cap_t;

extern cap_t cap;

#endif

