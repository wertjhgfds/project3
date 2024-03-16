#include "rtc.h"

RTC_DateTypeDef  RTC_DateStructure;
RTC_TimeTypeDef  RTC_TimeStructure;

char aShowTime[50] = {0};      //���ڴ洢RTC��ϵͳʱ��

__IO uint32_t uwTimeDisplay = 0;  //��ʾRTC�����жϵı�־

void RTC_Config(u8 hour,u8 min,u8 sec,u8 year,u8  month,u8  date )
{
	RTC_InitTypeDef  RTC_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//1.��PWR����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	//2.ʹ��RTC�Ĵ�����д����
	PWR_BackupAccessCmd(ENABLE);

	//3.ʹ��LSE�����ⲿʱ�� 32.768KHZ  ���ȸ�  ���ĵ�
	RCC_LSEConfig(RCC_LSE_ON);
	
	//4.�ȴ�LSE�����ⲿʱ�Ӿ���
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	//5.ѡ��LSE��ΪRTC�����ʱ��Դ  ���Է�Ƶ�õ�׼ȷ��1HZ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//6.��RTC�����ʱ��
	RCC_RTCCLKCmd(ENABLE);
	
	//7.�ȴ�RTC�Ĵ���ͬ�����
	RTC_WaitForSynchro();
	
	//8.��RTC����ʱ�ӷ�Ƶ����ʼ��  32768HZ / (127+1) * (255+1) = 1HZ
	RTC_InitStructure.RTC_AsynchPrediv = 128-1;
	RTC_InitStructure.RTC_SynchPrediv  = 256-1;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; //24Сʱ��
	RTC_Init(&RTC_InitStructure);
	
	//9.����RTC������  2024/2/26
	RTC_DateStructure.RTC_Year 		= (year/10)*16+(year%10);										//���
	RTC_DateStructure.RTC_Month 	= (month/10)*16+(month%10);										//�·�
	RTC_DateStructure.RTC_Date 		= (date/10)*16+(date%10);										//����
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	//10.����RTC��ʱ�� 15:51:30
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;							//����
	RTC_TimeStructure.RTC_Hours   = (hour/10)*16+(hour%10);										//ʱ��
	RTC_TimeStructure.RTC_Minutes = (min/10)*16+(min%10);										//����
	RTC_TimeStructure.RTC_Seconds = (sec/10)*16+(sec%10); 									//����
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	//11.���û����жϲ���  NVIC�Ĳ���
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//12.����EXTI22�ⲿ�ж� ���ӵ�RTC�����ж�
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//13.���RTC�Ļ����жϵı�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//14.�ر�RTC���Ѷ�ʱ��
	RTC_WakeUpCmd(DISABLE);
	
	//15.ѡ��RTC����ʱ��Դ  ѡ���ڲ�ʱ��(1HZ)
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//16.���û��Ѽ�������ֵ
	RTC_SetWakeUpCounter(0);
	
	//17.ѡ�����ж�  ѡ���ж�Դ
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	//18.�򿪻��Ѷ�ʱ��
	RTC_WakeUpCmd(ENABLE); 
}
  

//ÿ��1s����һ��
void RTC_WKUP_IRQHandler(void)
{
	 //�ж�RTC�����ж��Ƿ���
   if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
   {
     uwTimeDisplay = 1;
     RTC_ClearITPendingBit(RTC_IT_WUT);			//��������жϱ�־
     EXTI_ClearITPendingBit(EXTI_Line22);   //���EXTI22�ı�־
   }
}