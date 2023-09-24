#ifndef __SOFT_IIC_H
#define __SOFT_IIC_H

#include "drv_io.h"

//IO�ڷ�������
#define SDA_IN() 	{GPIOB->MODER&=~((uint32_t)3<<(15*2));GPIOB->MODER|=(uint32_t)0<<(15*2);}	//PB15����ģʽ
#define SDA_OUT() 	{GPIOB->MODER&=~((uint32_t)3<<(15*2));GPIOB->MODER|=(uint32_t)1<<(15*2);}	//PB15����ģʽ
//IO�ڲ�������
#define IIC_SCL 	PBout(13)
#define IIC_SDA 	PBout(15)
#define READ_SDA 	PBin(15)			

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(uint8_t ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  

#endif

