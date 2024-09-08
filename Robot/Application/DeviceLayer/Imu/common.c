/**
 * 此处定时器使用TIM2作为延时时钟源
 *
 * 
 */

#include "common.h"
#include "bmi2_defs.h"





/*
	个人配置
	SPI
	IIC片选
	SPI片选
*/


extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

#define IIC_CS_HIG()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET))
#define IIC_CS_LOW()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET))

#define SPI_CS_HIG()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET))
#define SPI_CS_LOW()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET))
#define EX_SPI_CS_HIG()		(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET))
#define EX_SPI_CS_LOW()		(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET))






/******************************************************************************/
/*!                 Macro definitions                                         */
#define BMI2XY_SHUTTLE_ID  UINT16_C(0x1B8)

/*! Macro that defines read write length */
#define READ_WRITE_LEN     UINT8_C(46)

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr = 0x69;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * Delay function map to RP
 */
uint32_t btick = 0;

uint32_t bmi2_micros(void)
{
	register uint32_t bms, bus;

	bms = HAL_GetTick();

	bus = TIM2->CNT;
	
  btick = bms*1000 + bus;
    
	return btick;
} 


void bmi2_delay_us(uint32_t period, void *intf_ptr)
{
	uint32_t now = bmi2_micros();
	
	while((bmi2_micros() - now) < period);
}

void bmi2_delay(uint32_t ms)
{

	while(ms--)
		bmi2_delay_us(1000,NULL);
	
}




/*!
 * I2C read function map to COINES platform
 */
//BMI2_INTF_RETURN_TYPE bmi2_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
//{

//    IIC_Start(); 
//		IIC_Send_Byte( (dev_addr<<1)|0  );	//wirte
//		IIC_Wait_Ack();		
//    IIC_Send_Byte(reg_addr);	
//    IIC_Wait_Ack();		
//    IIC_Start();
//		IIC_Send_Byte( (dev_addr<<1)|1 );	 //read
//    IIC_Wait_Ack();		
//	
//		for(uint8_t i = 0 ; i< len ; i++)
//		{
//			*reg_data=IIC_Read_Byte(1);
//			reg_data++;
//		}
//    IIC_Stop();			
//		
//		return 0;
//}

///*!
// * I2C write function map to COINES platform
// */
//BMI2_INTF_RETURN_TYPE bmi2_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
//{

//    IIC_Start(); 
//		IIC_Send_Byte((dev_addr<<1)|0);
//		if(IIC_Wait_Ack())
//		{
//			IIC_Stop();		 
//			return 1;		
//		}
//    IIC_Send_Byte(reg_addr);	
//    IIC_Wait_Ack();		
//		
//		for(uint8_t i = 0 ; i < len ; i++)
//		{ 
//			IIC_Send_Byte(*reg_data);
//			reg_data++;
//		}

//		if(IIC_Wait_Ack())	
//		{ 
//			IIC_Stop();	 
//			return 1;		 
//		}		 
//    IIC_Stop();	 
//		
//		return 0;
//}

/*!
 * SPI read function map to COINES platform
 */
BMI2_INTF_RETURN_TYPE bmi2_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	uint8_t res;
	
	SPI_CS_LOW();
	bmi2_delay_us(1, NULL);
	HAL_SPI_Transmit(&hspi2, &reg_addr, 1, 1000);
	res = HAL_SPI_Receive(&hspi2, reg_data, len, 1000);
	bmi2_delay_us(1, NULL);
	SPI_CS_HIG();
	return res;
	
}

/*!
 * SPI write function map to COINES platform
 */
BMI2_INTF_RETURN_TYPE bmi2_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	uint8_t res;
	
	SPI_CS_LOW();
	bmi2_delay_us(1, NULL);
	HAL_SPI_Transmit(&hspi2, &reg_addr, 1, 1000);
	res = HAL_SPI_Transmit(&hspi2, (uint8_t *)reg_data, len, 1000);
	bmi2_delay_us(1, NULL);
	SPI_CS_HIG();
	return res;

}


/*!
 *  @brief Function to select the interface between SPI and I2C.
 *  Also to initialize coines platform
 */
int8_t bmi2_interface_init(struct bmi2_dev *bmi, uint8_t intf, uint8_t aces)
{
    int8_t rslt = BMI2_OK;

		if(bmi != NULL)
		{
        /* Bus configuration : I2C */
        if (intf == BMI2_I2C_INTF)
        {
            /* To initialize the user I2C function */
            dev_addr = BMI2_I2C_SEC_ADDR;
            bmi->intf = BMI2_I2C_INTF;
//            bmi->read = bmi2_i2c_read;
//            bmi->write = bmi2_i2c_write;
						rslt = BMI2_E_NULL_PTR;
						return rslt;

						/*IIC_CS selected to stay 100ms*/
//						IIC_CS_HIG();
//            bmi2_delay(100);


        }
        /* Bus configuration : SPI */
        else if (intf == BMI2_SPI_INTF)
        {

            /* To initialize the user SPI function */
            bmi->intf = BMI2_SPI_INTF;
						if (aces == BMI2_INT_ACES)
						{
							bmi->read = bmi2_spi_read;
							bmi->write = bmi2_spi_write;
						}
						else if (aces == BMI2_EXT_ACES)
						{
//							bmi->read = ex_bmi2_spi_read;
//							bmi->write = ex_bmi2_spi_write;
						}
        }

        /* Assign device address to interface pointer */
        bmi->intf_ptr = &dev_addr;

        /* Configure delay in microseconds */
        bmi->delay_us = bmi2_delay_us;

        /* Configure max read/write length (in bytes) ( Supported length depends on target machine) */
        bmi->read_write_len = READ_WRITE_LEN;

        /* Assign to NULL to load the default config file. */
        bmi->config_file_ptr = NULL;

        bmi2_delay(100);

    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;

}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bmi2_error_codes_print_result(int8_t rslt)
{

}

/*!
 *  @brief Deinitializes coines platform
 *
 *  @return void.
 */
void bmi2_coines_deinit(void)
{

}
