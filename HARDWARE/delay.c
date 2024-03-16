#include "delay.h"
//延时微秒  注意：24bit计数器  不能798915us
void delay_us(u32 nus)
{
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = nus*21 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

//延时毫秒  注意：24bit计数器  不能798ms
void delay_ms(u32 nms)
{
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = nms*21000 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}
///**
// * @file delay.c
// * @author shao
// * @brief   使用滴答定时器延时指定的时间
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
//static u8  fac_us=0;							//us延时倍乘数			   
//static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数

// 
//extern void xPortSysTickHandler(void);
// 
////systick中断服务函数,使用OS时用到
//void SysTick_Handler(void)
//{	
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
//    {
//        xPortSysTickHandler();	
//    }
//}
//			   
////初始化延迟函数
////SYSTICK的时钟固定为AHB时钟，基础例程里面SYSTICK时钟频率为AHB/8
////这里为了兼容FreeRTOS，所以将SYSTICK的时钟频率改为AHB的频率！
////SYSCLK:系统时钟频率
//void delay_init(u8 SYSCLK)
//{
//	u32 reload;
// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
//	fac_us=SYSCLK;							//不论是否使用OS,fac_us都需要使用
//	reload=SYSCLK;							//每秒钟的计数次数 单位为M	   
//	reload*=1000000/configTICK_RATE_HZ;		//根据configTICK_RATE_HZ设定溢出时间
//											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
//	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
//	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
//}								    

////延时nus
////nus:要延时的us数.	
////nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	    								   
//void delay_us(u32 nus)
//{		
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
//	ticks=nus*fac_us; 						//需要的节拍数 
//	told=SysTick->VAL;        				//刚进入时的计数器值
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{	    
//			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
//			else tcnt+=reload-tnow+told;	    
//			told=tnow;
//			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
//		}  
//	};										    
//}  
////延时nms
////nms:要延时的ms数
////nms:0~65535
//void delay_ms(u32 nms)
//{	
//    u32 i;
//	for(i=0;i<nms;i++) delay_us(1000);
//}


//#else
//static u8  fac_us=0;							//us延时倍乘数			   
//static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
//	

//			   
////初始化延迟函数
////当使用OS的时候,此函数会初始化OS的时钟节拍
////SYSTICK的时钟固定为AHB时钟的1/8
////SYSCLK:系统时钟频率
//void delay_init(u8 SYSCLK)
//{

// 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
//	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使用
//	fac_ms=(u16)fac_us*1000;				//非OS下,代表每个ms需要的systick时钟数   

//}								    

////延时nus
////nus为要延时的us数.	
////注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
//void delay_us(u32 nus)
//{		
//	u32 temp;	    	 
//	SysTick->LOAD=nus*fac_us; 				//时间加载	  		 
//	SysTick->VAL=0x00;        				//清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数 	 
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
//	SysTick->VAL =0X00;       				//清空计数器 
//}
////延时nms
////注意nms的范围
////SysTick->LOAD为24位寄存器,所以,最大延时为:
////nms<=0xffffff*8*1000/SYSCLK
////SYSCLK单位为Hz,nms单位为ms
////对168M条件下,nms<=798ms 
//void delay_xms(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;			//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;           			//清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
//	SysTick->VAL =0X00;     		  		//清空计数器	  	    
//} 
////延时nms 
////nms:0~65535
//void delay_ms(u16 nms)
//{	 	 
//	u8 repeat=nms/540;						//这里用540,是考虑到某些客户可能超频使用,
//											//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
//	u16 remain=nms%540;
//	while(repeat)
//	{
//		delay_xms(540);
//		repeat--;
//	}
//	if(remain)delay_xms(remain);
//} 


//#endif // DEBUG	 

