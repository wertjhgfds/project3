////
///**
// * @file delay.h
// * @author shao
// * @brief  ʹ�õδ�ʱ����ʱָ����ʱ��
// * @version 0.1
// * @date 2024-02-05
// * 
// * @copyright Copyright (c) 2024
// * 
// */

#ifndef _DELAY_H
#define _DELAY_H 	
#include "stm32f4xx.h"  //�������
void delay_ms(u32 nms);
void delay_us(u32 nus);

//#define USE_FREERTOS        1
//#include "sys.h"
//	  
//#if USE_FREERTOS

//void delay_init(u8 SYSCLK);
//void delay_us(u32 nus);
//void delay_ms(u32 nms);

//#else
//void delay_init(u8 SYSCLK);
//void delay_us(u32 nus);
//void delay_xms(u16 nms);
//void delay_ms(u16 nms);
//#endif

#endif


