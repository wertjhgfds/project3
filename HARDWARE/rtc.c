#include "rtc.h"

RTC_DateTypeDef  RTC_DateStructure;
RTC_TimeTypeDef  RTC_TimeStructure;

char aShowTime[50] = {0};      //用于存储RTC的系统时间

__IO uint32_t uwTimeDisplay = 0;  //表示RTC唤醒中断的标志

void RTC_Config(u8 hour,u8 min,u8 sec,u8 year,u8  month,u8  date )
{
	RTC_InitTypeDef  RTC_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//1.打开PWR外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	//2.使能RTC寄存器的写访问
	PWR_BackupAccessCmd(ENABLE);

	//3.使能LSE低速外部时钟 32.768KHZ  精度高  功耗低
	RCC_LSEConfig(RCC_LSE_ON);
	
	//4.等待LSE低速外部时钟就绪
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	//5.选择LSE作为RTC外设的时钟源  可以分频得到准确的1HZ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//6.打开RTC外设的时钟
	RCC_RTCCLKCmd(ENABLE);
	
	//7.等待RTC寄存器同步完成
	RTC_WaitForSynchro();
	
	//8.对RTC进行时钟分频并初始化  32768HZ / (127+1) * (255+1) = 1HZ
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;
	RTC_InitStructure.RTC_SynchPrediv  = 256-1;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; //24小时制
	RTC_Init(&RTC_InitStructure);
	
	//9.设置RTC的日期  2024/2/26
	RTC_DateStructure.RTC_Year 		= (year/10)*16+(year%10);										//年份
	RTC_DateStructure.RTC_Month 	= (month/10)*16+(month%10);										//月份
	RTC_DateStructure.RTC_Date 		= (date/10)*16+(date%10);										//日期
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	//10.设置RTC的时间 15:51:30
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;							//下午
	RTC_TimeStructure.RTC_Hours   = (hour/10)*16+(hour%10);										//时钟
	RTC_TimeStructure.RTC_Minutes = (min/10)*16+(min%10);										//分钟
	RTC_TimeStructure.RTC_Seconds = (sec/10)*16+(sec%10); 									//秒钟
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	//11.设置唤醒中断参数  NVIC的参数
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//12.配置EXTI22外部中断 连接到RTC唤醒中断
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//13.清除RTC的唤醒中断的标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//14.关闭RTC唤醒定时器
	RTC_WakeUpCmd(DISABLE);
	
	//15.选择RTC唤醒时钟源  选择内部时钟(1HZ)
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//16.设置唤醒计数器的值
	RTC_SetWakeUpCounter(0);
	
	//17.选择唤醒中断  选择中断源
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	//18.打开唤醒定时器
	RTC_WakeUpCmd(ENABLE); 
}
  

//每隔1s触发一次
void RTC_WKUP_IRQHandler(void)
{
	 //判断RTC唤醒中断是否发生
   if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
   {
     uwTimeDisplay = 1;
     RTC_ClearITPendingBit(RTC_IT_WUT);			//清除唤醒中断标志
     EXTI_ClearITPendingBit(EXTI_Line22);   //清除EXTI22的标志
   }
}