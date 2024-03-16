#ifndef __ADC_H
#define __ADC_H	

#define  DHT11_SET(n)  ( (n) ? GPIO_SetBits(GPIOG,GPIO_Pin_9) : GPIO_ResetBits(GPIOG,GPIO_Pin_9) )

#define  DHT11_READ	   GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)

#include "stm32f4xx.h"
#include <stdbool.h>
#include "delay.h"

void DHT11_Config(void);
void DHT11_SetPinOutputMode(void);
void DHT11_SetPinInputMode(void);
void DHT11_StartSignal(void);
bool  DHT11_GetACK(void);
uint8_t DHT11_ReadBit(void);
bool DHT11_GetValue(uint8_t buf[5]);
void Smog_Config(void);
void Fire_Config(void);
void LightSensor_Config(void);

#endif