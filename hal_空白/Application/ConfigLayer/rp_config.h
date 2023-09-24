#ifndef __RP_CONFIG_H
#define __RP_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* Exported macro ------------------------------------------------------------*/
/* 时间戳 */
#define		TIME_STAMP_250MS	250
#define 	TIME_STAMP_500MS	500
#define    	TIME_STAMP_400MS    400
#define		TIME_STAMP_5000MS	5000

//底盘与云台的机械中值
#define 	CO_MECH_ANGLE_POS_MID	(6300)
#define 	CO_MECH_ANGLE_NEG_MID	(2229)

/* Exported types ------------------------------------------------------------*/
/* 驱动层 --------------------------------------------------------------------*/
/**
 *	@brief	驱动类型
 *	@class	driver
 */
typedef enum drv_type {
    DRV_CAN1,
    DRV_CAN2,
    DRV_PWM_FRIC_L,
    DRV_PWM_FRIC_R,
    DRV_PWM_SERVO,
    DRV_IIC,
    DRV_UART1,
    DRV_UART2,
    DRV_UART3,
    DRV_UART4,
    DRV_UART5,
} drv_type_t;

/**
 *	@brief	iic驱动
 *	@class	driver
 */
typedef struct drv_iic {
    enum drv_type 	type;
} drv_iic_t;

/**
 *	@brief	can驱动
 *	@class	driver
 */
typedef struct drv_can {
    enum drv_type 	type;
    uint32_t		can_id;
    uint32_t		std_id;
    uint8_t			drv_id;
    void			(*tx_data)(struct drv_can *self, int16_t txData);
} drv_can_t;

/**
 *	@brief	pwm驱动
 *	@class	driver
 */
typedef struct drv_pwm {
    enum drv_type	type;
    void			(*output)(struct drv_pwm *self, int16_t pwm);
} drv_pwm_t;

/**
 *	@brief	uart驱动
 *	@class	driver
 */
typedef struct drv_uart {
    enum drv_type	type;
    void			(*tx_byte)(struct drv_uart *self, uint8_t byte);
} drv_uart_t;

/* 设备层 --------------------------------------------------------------------*/
/**
 *	@brief	设备id列表
 *	@class	device
 */
typedef enum {
    DEV_ID_RC = 0,
    DEV_ID_IMU = 1,
    DEV_ID_CHASSIS_L = 2,
    DEV_ID_CHASSIS_R = 3,
    DEV_ID_CHASSIS_F = 4,
    DEV_ID_CHASSIS_B = 5,
    DEV_ID_CNT = 6,
		DEV_ID_JUDGE = 7,
	  DEV_ID_VISION = 8,
} dev_id_t;

/**
 *	@brief	底盘电机设备索引
 *	@class	device
 */
typedef enum {
    CHAS_L,//左右
    CHAS_R,
    CHAS_F,//前后
    CHAS_B,
    CHAS_MOTOR_CNT,
} chassis_motor_cnt_t;
/**
 *	@brief	陀螺仪设备索引
 *	@class	device
 */
typedef enum{
	pitch_longtime,
	yaw_longtime,
	IMU_CNT,
}imu_cnt_t;

/**
 *	@brief	设备工作状态
 *	@class	device
 */
typedef enum {
    DEV_ONLINE,
    DEV_OFFLINE,
} dev_work_state_t;

/**
 *	@brief	错误代码
 *	@class	device
 */
typedef enum {
    NONE_ERR,		// 正常(无错误)
    DEV_ID_ERR,		// 设备ID错误
    DEV_INIT_ERR,	// 设备初始化错误
    DEV_DATA_ERR,	// 设备数据错误
} dev_errno_t;

/**
 *	@brief	设备结构体定义模板
 *	@class	device
 */
typedef struct device {
    void				*info;		// 自定义具体设备信息结构体
    void				*driver;	// 自定义具体设备驱动结构体
    void				(*init)(struct device *self);	// 设备初始化函数
    void				(*update)(struct device *self, uint8_t *rxBuf);	// 设备数据更新函数
    void				(*check)(struct device *self);	// 设备数据检查函数
    void				(*heart_beat)(struct device *self);	// 设备心跳包
    dev_work_state_t	work_state;	// 设备工作状态
    dev_errno_t			errno;		// 可自定义具体设备错误代码
    dev_id_t			id;			// 设备id
} device_t;

/* 应用层 --------------------------------------------------------------------*/
/**
 *	@brief	pid控制器
 *	@class	controller
 */
typedef struct pid_ctrl {
    float		target;
    float		measure;
    float 	err;
    float 	last_err;
    float		kp;
    float 	ki;
    float 	kd;
    float 	pout;
    float 	iout;
    float 	dout;
    float 	out;
    float		integral;
    float 	integral_max;
    float 	out_max;
	  float   deta_err;
	  float   blind_err;
} pid_ctrl_t;

/* Remote Mode Enum */
typedef enum {
    RC = 0,
    KEY = 1,
    REMOTE_MODE_CNT = 2,
} remote_mode_t;

typedef enum {
    SYS_STATE_NORMAL,	// 系统正常
    SYS_STATE_RCLOST,	// 遥控失联
    SYS_STATE_RCERR,	// 遥控出错
    SYS_STATE_WRONG,	// 其它系统错误
} sys_state_t;

typedef enum {
  SYS_MODE_NORMAL,		//常规行走模式
	SYS_MODE_AUTO,			//自瞄模式
	SYS_MODE_LONGSHOOT,	//吊射模式
	SYS_MODE_PARK,			//对位模式
	SYS_MODE_CNT,				//凑数
} sys_mode_t;					//系统各种模式

typedef enum {
	CO_MECH,
	CO_GYRO
}co_mode_t;
//前后轮控制顺序标志位
typedef enum {
	level_0 = 0,//
	level_1 = 1,
	level_2 = 2,
	level_3 = 3,
	level_4 = 4,
}co_sequent_t;
typedef struct{
	co_sequent_t  FBco_sequent;//前后轮初始化；
}co_sequent_list;
typedef enum{
	None = 0,
	Set  = 1,
	ReSet= 2,
}init_mode_t;
//控制环的开关
typedef struct{
	init_mode_t Stand_circle;
	init_mode_t Locat_circle;
	init_mode_t Turnaround_circle;
	init_mode_t FB_wheel;
	init_mode_t F_wheel;
	init_mode_t B_wheel;
}circle_mode_t;

typedef enum {
	LOGIC_FRONT,
	LOGIC_BACK
	
}co_angle_logic_t;
//typedef struct {
//	struct {
//		uint8_t reset_start;
//		uint8_t reset_ok;
//	}gimbal;
//} flag_t;
/**
 *	@brief	kalman控制器
 *	@class	controller
 */
typedef enum{
	kal_standI,//直立环内环
	kal_locat_pout,
	KAL_CNT
}kalman_mode_cnt_t;
typedef struct {
    remote_mode_t		remote_mode;	// 控制方式
		co_mode_t 			co_mode;			// 控制模式	云台/机械
    sys_state_t			state;				// 系统状态
    sys_mode_t			mode;					// 系统模式
		circle_mode_t   circle_mode;  // 控制环
	uint8_t           init_stand_flag;//只做一次
} system_t;

//extern flag_t	flag;
extern system_t sys;
extern co_sequent_list co_sequent;
/* Exported functions --------------------------------------------------------*/
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif
