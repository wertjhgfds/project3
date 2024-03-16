#ifndef __USART_H
#define __USART_H	

#include "stm32f4xx.h"
#include "motor.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "WS2812.h"
//void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

//void USART1_Config(u32 baud);
void USART2_Config(u32 baud);

void USART1_SendString(char *str);
//void USART1_Handle(void);
void USART2_SendString(char *str);
void USART2_Handle(void);

#define  BLE_BUFFERSIZE  256									//接收缓冲区的大小
#define  HOST_BUFFERSIZE  256									//接收缓冲区的大小



//int fputc(int ch, FILE *f) ;
//void _sys_exit(int return_code) ;
#endif