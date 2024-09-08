#ifndef __BMI_H_
#define __BMI_H_

#include "stdint.h"

typedef struct bmi_struct {

	struct bmi2_dev *dev;
	
	uint8_t drive_type;
	
	uint8_t device_aces;
	
	int8_t (*init)(struct bmi2_dev *bmi,uint8_t intf, uint8_t aces);
	
	int8_t (*read)(uint8_t reg_addr, uint8_t *data, uint16_t len, struct bmi2_dev *dev);
	int8_t (*write)(uint8_t reg_addr, const uint8_t *data, uint16_t len, struct bmi2_dev *dev);

} bmi_t;


#endif


