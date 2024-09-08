/* Includes ------------------------------------------------------------------*/
#include "imu_sensor.h"
#include "bmi.h"
#include "drv_gpio.h"
#include "drv_tick.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void imu_init(imu_sensor_t *self);
void imu_heart_beat(work_state_t *heart);
extern void imu_update(imu_sensor_t *self);
extern void transform_init(void);
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern float Kp;

struct bmi2_dev bmi270;
struct bmi2_dev ex_bmi270;

//int8_t rs, rs1;

bmi_t bmi_client = {

	.dev = &bmi270,
	.init = &bmi_init,	

};

bmi_t ex_bmi_client = {

	.dev = &ex_bmi270,
	.init = &bmi_init,	

};

imu_info_t imu_info = 
{
	.offset_info.gx_offset = 0.f,
	.offset_info.gy_offset = 0.f,
	.offset_info.gz_offset = 0.f,
	.kp = &Kp,
	.init_flag = 0,
};

/* Exported variables --------------------------------------------------------*/
imu_sensor_t imu_sensor = {

	.bmi = &bmi_client,
	.ex_bmi = &ex_bmi_client,
	.info = &imu_info,
	.driver.tpye = DR_SPI2,
	.work_state.dev_state = DEV_OFFLINE,
	.id = DEV_ID_IMU,	
	.work_state.cali_end = 0,	
	.work_state.offline_max_cnt = 50,	
	.work_state.err_cnt = 0,
	
	.init = &imu_init,
	.update = &imu_update,
  .heart_beat = &imu_heart_beat,
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
float imu_read[3];
uint8_t init_cnt = 200;
void imu_init(struct imu_struct *self)
{
	uint32_t tickstart = HAL_GetTick();

	self->work_state.dev_state = DEV_OFFLINE;

	self->bmi->read = bmi2_get_regs;
  self->bmi->write = bmi2_set_regs;
	
	if(self->driver.tpye == DR_SPI1 || self->driver.tpye == DR_SPI2 || self->driver.tpye == DR_SPI3){

		self->bmi->drive_type = BMI2_SPI_INTF;
		if(self->driver.tpye == DR_SPI1)
		{
			self->bmi->device_aces = BMI2_INT_ACES;
		}
		else if(self->driver.tpye == DR_SPI1)
		{
			self->bmi->device_aces = BMI2_EXT_ACES;
		}

	}
	else if(self->driver.tpye == DRV_IIC){
	
		self->bmi->drive_type = BMI2_I2C_INTF;
		
	}
	
	self->work_state.init_code = self->bmi->init(self->bmi->dev,self->bmi->drive_type,self->bmi->device_aces);

	while(self->work_state.init_code)
	{
		if (++self->work_state.err_cnt == init_cnt)
		{
			__set_FAULTMASK(1); 
			NVIC_SystemReset();
			break;
		}
        self->work_state.err_code = IMU_INIT_ERR;
        self->work_state.init_code = self->bmi->init(self->bmi->dev,self->bmi->drive_type,self->bmi->device_aces);
	}	

	
	if (self->work_state.init_code == 0)
	{
		self->work_state.dev_state = DEV_ONLINE;
		self->work_state.err_code = IMU_NONE_ERR;
//		self->work_state.err_cnt = 0;
		self->info->init_flag = 1;
		
		/* Ðý×ª¾ØÕó³õÊ¼»¯ */
		transform_init();
		
//		Flash_ReadData(IMU_DATA_ADDR, (uint32_t*)imu_read, 12);
//		if (*(uint32_t*)imu_read != 0xFFFFFFFF)
//		{
//			imu_sensor.info->offset_info.gx_offset = imu_read[0];
//			imu_sensor.info->offset_info.gy_offset = imu_read[1];
//			imu_sensor.info->offset_info.gz_offset = imu_read[2];
//		}
		imu_sensor.info->offset_info.gx_offset = 0.f;
		imu_sensor.info->offset_info.gy_offset = 0.f;
		imu_sensor.info->offset_info.gz_offset = 0.f;
	}
	else
	{
		self->work_state.dev_state = DEV_OFFLINE;
		self->work_state.err_code = IMU_INIT_ERR;
		self->work_state.offline_cnt = self->work_state.offline_max_cnt;
		self->info->init_flag = 0;
	}
	
}


void imu_heart_beat(work_state_t *heart){


	heart->offline_cnt++;
	if(heart->offline_cnt > heart->offline_max_cnt) 
	{
		heart->offline_cnt = heart->offline_max_cnt;
		heart->dev_state = DEV_OFFLINE;
	}
	else 
	{
		if(heart->dev_state == DEV_OFFLINE)
			heart->dev_state = DEV_ONLINE;
	}

}

