#ifndef __BMI_H
#define __BMI_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "bmi_t.h"
#include "bmi2.h"
#include "bmi270.h"
#include "common.h"
#include "arm_math.h"

/* Exported macro ------------------------------------------------------------*/
#define ACCD_X_LSB 0x0c
#define ACCD_X_MSB 0x0d
#define ACCD_Y_LSB 0x0e
#define ACCD_Y_MSB 0x0f
#define ACCD_Z_LSB 0x10
#define ACCD_Z_MSB 0x11
#define GYR_X_LSB 0x12
#define GYR_X_MSB 0x13
#define GYR_Y_LSB 0x14
#define GYR_Y_MSB 0x15
#define GYR_Z_LSB 0x16
#define GYR_Z_MSB 0x17
#define TEMPERATURE_0 0x22
#define TEMPERATURE_1 0x23
#define TEMP_RATIO (0.001953125f)


/* Exported types ------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;


/* Exported functions --------------------------------------------------------*/
int8_t bmi_init(struct bmi2_dev *bmi2_dev, uint8_t intf, uint8_t aces);
void BMI_Get_Temperature(float *temp);
void BMI_Get_RawData(int16_t *ggx, int16_t *ggy, int16_t *ggz, int16_t *aax, int16_t *aay, int16_t *aaz);
void EX_BMI_Get_RawData(int16_t *ggx, int16_t *ggy, int16_t *ggz, int16_t *aax, int16_t *aay, int16_t *aaz);
void Vector_Transform(int16_t gx, int16_t gy, int16_t gz,\
	                    int16_t ax, int16_t ay, int16_t az,\
	                    float *ggx, float *ggy, float *ggz,\
											float *aax, float *aay, float *aaz);
uint8_t BMI_Get_EulerAngle(float *pitch,float *roll,float *yaw,\
                           float *pitch_,float *roll_,float *yaw_,\
													 float *ggx,float *ggy,float *ggz,\
													 float *aax,float *aay,float *aaz);
													 
uint8_t EX_BMI_Get_EulerAngle(float *pitch,float *roll,float *yaw,\
															float *pitch_,float *roll_,float *yaw_,\
															float *ggx,float *ggy,float *ggz,\
															float *aax,float *aay,float *aaz);
													 
void BMI_Get_Acceleration(float pitch, float roll, float yaw,\
													float ax, float ay, float az,\
													float *accx, float *accy, float *accz);


#endif


