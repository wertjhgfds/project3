#ifndef __I2C_H
#define __I2C_H	

#include "stm32f4xx.h"
#include <stdbool.h>
#include "delay.h"

//使用IO口来模拟IIC时序，从而控制从器件
#define  IIC_SDA(n)  (n) ? GPIO_SetBits(GPIOB,GPIO_Pin_9) : GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define  IIC_SCL(n)  (n) ? GPIO_SetBits(GPIOB,GPIO_Pin_8) : GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define  IIC_READ		 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

void IIC_Config(void);
static void IIC_SDASetOutputMode(void);
static void IIC_SDASetInputMode(void);
void IIC_Start(void);
void IIC_SendByte(uint8_t data);
bool IIC_WaitSlaveAck(void);
uint8_t IIC_ReadByte(void);
void IIC_MasterSendAck(uint8_t ack);
void IIC_Stop(void);

#endif