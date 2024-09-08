/* Includes ------------------------------------------------------------------*/
#include "imu_sensor.h"

#include "imu_protocol.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ave_filter_t imu_pitch_dif_speed_ave_filter;
ave_filter_t imu_roll_dif_speed_ave_filter;
ave_filter_t imu_yaw_dif_speed_ave_filter;

short ggx, ggy, ggz;
short aax, aay, aaz;

float temp;

//临时变量
float pitch, roll, yaw;

float gyrox, gyroy, gyroz;
float accx, accy, accz;
float gyrox_, gyroy_, gyroz_;
float accx_, accy_, accz_;
float pitch_, roll_, yaw_;
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int16_t imu_cnt = 0;
void imu_update(imu_sensor_t *imu_sen)
{
//	static uint16_t i = 0;
  imu_info_t *imu_info = imu_sen->info;
	
	/* 获取陀螺仪数据 */
	BMI_Get_RawData(&ggx, &ggy, &ggz, &aax, &aay, &aaz);
	BMI_Get_Temperature(&temp);
	
	imu_info->raw_info.acc_x = aax;
	imu_info->raw_info.acc_y = aay;
	imu_info->raw_info.acc_z = aaz;
	imu_info->raw_info.gyro_x = ggx;
	imu_info->raw_info.gyro_y = ggy;
	imu_info->raw_info.gyro_z = ggz;
	
	/* 坐标系变换 */
	Vector_Transform(ggx, ggy, ggz, aax, aay, aaz,\
	                 &gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
	
	/* 陀螺仪校正 */
	if (imu_sen->work_state.err_code == IMU_DATA_CALI)
	{
		if (imu_cnt < 2000)
		{
			imu_info->offset_info.gx_offset -= gyrox * 0.0005f;
			imu_info->offset_info.gy_offset -= gyroy * 0.0005f;
			imu_info->offset_info.gz_offset -= gyroz * 0.0005f;
			imu_cnt++;
		}
		else
		{
			imu_cnt = 0;
			imu_sen->work_state.err_code = IMU_NONE_ERR;
			
			if (abs(imu_info->offset_info.gx_offset) > 5.f)
				imu_info->offset_info.gx_offset = 0;
			if (abs(imu_info->offset_info.gy_offset) > 5.f)
				imu_info->offset_info.gy_offset = 0;
			if (abs(imu_info->offset_info.gz_offset) > 5.f)
				imu_info->offset_info.gz_offset = 0;
			imu_sen->work_state.cali_end = 1;
		}
	}
	else
	{
		gyrox += imu_info->offset_info.gx_offset;
		gyroy += imu_info->offset_info.gy_offset;
		gyroz += imu_info->offset_info.gz_offset;
	}
	
	/* 原始数据低通滤波 */
	gyrox_ = lowpass(gyrox_, gyrox, 1);
	gyroy_ = lowpass(gyroy_, gyroy, 1);
	gyroz_ = lowpass(gyroz_, gyroz, 1);
	accx_ = lowpass(accx_, accx, 0.2);
	accy_ = lowpass(accy_, accy, 0.2);
	accz_ = lowpass(accz_, accz, 0.2);
	
	/* 解算陀螺仪数据 */
  BMI_Get_EulerAngle(&imu_info->base_info.pitch, &imu_info->base_info.roll, &imu_info->base_info.yaw,\
										 &pitch_, &roll_, &yaw_, \
										 &gyrox_, &gyroy_, &gyroz_, \
										 &accx_, &accy_, &accz_);
										 
	pitch = imu_info->base_info.pitch, roll = imu_info->base_info.roll, yaw = imu_info->base_info.yaw;
	
//	accx_ = lowpass(accx_, accx, 0.1);
//	accy_ = lowpass(accy_, accy, 0.1);
//	accz_ = lowpass(accz_, accz, 0.1);
	BMI_Get_Acceleration(pitch, roll, yaw,\
											 accx_, accy_, accz_,\
											 &imu_info->base_info.accx, &imu_info->base_info.accy, &imu_info->base_info.accz);
	
	/* 计算陀螺仪数据 */
	//pitch
	imu_info->base_info.rate_pitch = pitch_;
	imu_info->base_info.ave_rate_pitch = ave_fil_update(&imu_pitch_dif_speed_ave_filter, imu_info->base_info.rate_pitch, 3);
	
	//roll
	imu_info->base_info.rate_roll = roll_;
	imu_info->base_info.ave_rate_roll = ave_fil_update(&imu_roll_dif_speed_ave_filter, imu_info->base_info.rate_roll, 3);
	
	//yaw
	imu_info->base_info.rate_yaw = yaw_;
	imu_info->base_info.ave_rate_yaw = ave_fil_update(&imu_yaw_dif_speed_ave_filter, imu_info->base_info.rate_yaw, 3);
	
	imu_sen->work_state.offline_cnt = 0;
	
	/*  imu读取数据判断  */
	if ((aax == 0) && (aay == 0) && (aaz == 0) \
			 && (ggx == 0) && (ggy == 0) && (ggz == 0))
	{
		if(++imu_sen->work_state.err_cnt >= 100)
		{
			imu_sen->work_state.dev_state = DEV_OFFLINE;
			imu_sen->work_state.err_code = IMU_DATA_ERR;
			imu_sen->work_state.offline_cnt = imu_sen->work_state.offline_max_cnt;
			imu_sen->work_state.err_cnt = 100;
		}
	}
	else
	{
//		imu_sen->work_state.err_cnt = 0;
	}
}
