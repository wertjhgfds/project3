#include "delay.h"
//��ʱ΢��  ע�⣺24bit������  ����798915us
void delay_us(u32 nus)
{
	SysTick->CTRL = 0; 					// �رն�ʱ��
	SysTick->LOAD = nus*21 - 1; // ��������ֵ
	SysTick->VAL 	= 0; 					// �������ֵ
	SysTick->CTRL = 1; 					// ������ʱ��  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
	SysTick->CTRL = 0; 					// �رն�ʱ��
}

//��ʱ����  ע�⣺24bit������  ����798ms
void delay_ms(u32 nms)
{
	SysTick->CTRL = 0; 					// �رն�ʱ��
	SysTick->LOAD = nms*21000 - 1; // ��������ֵ
	SysTick->VAL 	= 0; 					// �������ֵ
	SysTick->CTRL = 1; 					// ������ʱ��  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
	SysTick->CTRL = 0; 					// �رն�ʱ��
}
///**
// * @file delay.c
// * @author shao
// * @brief   ʹ�õδ�ʱ����ʱָ����ʱ��
// * @version 0.1
// * @date 2024-02-05
// * 
// * @copyright Copyright (c) 2024
// * 
// */
//#include "delay.h"

//#if USE_FREERTOS

//#include "FreeRTOS.h"
//#include "task.h"
//static u8  fac_us=0;							//us��ʱ������			   
//static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

// 
//extern void xPortSysTickHandler(void);
// 
////systick�жϷ�����,ʹ��OSʱ�õ�
//void SysTick_Handler(void)
//{	
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
//    {
//        xPortSysTickHandler();	
//    }
//}
//			   
////��ʼ���ӳٺ���
////SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
////����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
////SYSCLK:ϵͳʱ��Ƶ��
//void delay_init(u8 SYSCLK)
//{
//	u32 reload;
// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
//	fac_us=SYSCLK;							//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
//	reload=SYSCLK;							//ÿ���ӵļ������� ��λΪM	   
//	reload*=1000000/configTICK_RATE_HZ;		//����configTICK_RATE_HZ�趨���ʱ��
//											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.0998s����	
//	fac_ms=1000/configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ	   
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
//	SysTick->LOAD=reload; 					//ÿ1/configTICK_RATE_HZ��һ��	
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK     
//}								    

////��ʱnus
////nus:Ҫ��ʱ��us��.	
////nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    								   
//void delay_us(u32 nus)
//{		
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
//	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
//	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
//		}  
//	};										    
//}  
////��ʱnms
////nms:Ҫ��ʱ��ms��
////nms:0~65535
//void delay_ms(u32 nms)
//{	
//    u32 i;
//	for(i=0;i<nms;i++) delay_us(1000);
//}


//#else
//static u8  fac_us=0;							//us��ʱ������			   
//static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
//	

//			   
////��ʼ���ӳٺ���
////��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
////SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
////SYSCLK:ϵͳʱ��Ƶ��
//void delay_init(u8 SYSCLK)
//{

// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
//	fac_us=SYSCLK/8;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
//	fac_ms=(u16)fac_us*1000;				//��OS��,����ÿ��ms��Ҫ��systickʱ����   

//}								    

////��ʱnus
////nusΪҪ��ʱ��us��.	
////ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
//void delay_us(u32 nus)
//{		
//	u32 temp;	    	 
//	SysTick->LOAD=nus*fac_us; 				//ʱ�����	  		 
//	SysTick->VAL=0x00;        				//��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ���� 	 
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
//	SysTick->VAL =0X00;       				//��ռ����� 
//}
////��ʱnms
////ע��nms�ķ�Χ
////SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
////nms<=0xffffff*8*1000/SYSCLK
////SYSCLK��λΪHz,nms��λΪms
////��168M������,nms<=798ms 
//void delay_xms(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;           			//��ռ�����
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
//	SysTick->VAL =0X00;     		  		//��ռ�����	  	    
//} 
////��ʱnms 
////nms:0~65535
//void delay_ms(u16 nms)
//{	 	 
//	u8 repeat=nms/540;						//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
//											//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
//	u16 remain=nms%540;
//	while(repeat)
//	{
//		delay_xms(540);
//		repeat--;
//	}
//	if(remain)delay_xms(remain);
//} 


//#endif // DEBUG	 

