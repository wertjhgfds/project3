#ifndef __MOTOR_H
#define __MOTOR_H	

#include "stm32f4xx.h"
#include "delay.h"
#define DC_A_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_8)
#define DC_B_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_9)
#define DC_C_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_10)
#define DC_D_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_15)

#define DC_A_ON  GPIO_SetBits(GPIOD,GPIO_Pin_8)
#define DC_B_ON  GPIO_SetBits(GPIOD,GPIO_Pin_9)
#define DC_C_ON  GPIO_SetBits(GPIOD,GPIO_Pin_10)
#define DC_D_ON  GPIO_SetBits(GPIOD,GPIO_Pin_15)

  void FanInit(void);
void FanConfig(void);
void Motor_Config(void);
void Motorcw1(void);
void Motorcw2(void);
void MotorStop(void);
#endif