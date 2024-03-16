#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oled.h"
#include "usart.h"



void RTC_Config(u8 hour,u8 min,u8 sec,u8 year,u8 month,u8 date );

void RTC_WKUP_IRQHandler(void);

#endif