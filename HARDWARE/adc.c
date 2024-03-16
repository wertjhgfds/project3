#include "adc.h"
uint8_t dhtbuf[5] = {0};
//DHT11的初始化
void DHT11_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	DHT11_SET(1);

}


void DHT11_SetPinOutputMode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void DHT11_SetPinInputMode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


//开始信号
void DHT11_StartSignal(void)
{
	//1.设置引脚为输出模式
	DHT11_SetPinOutputMode();
	
	//2.确保引脚为空闲状态
	DHT11_SET(1);

	//3.把引脚拉低并持续大于18ms
	DHT11_SET(0);
  delay_ms(20);

	//4.把引脚拉高并持续20~40us
	DHT11_SET(1);
	delay_us(30);
}

//响应信号
bool  DHT11_GetACK(void)
{
	int cnt = 0; //用于计数
	
	//1.设置引脚为输入模式
	DHT11_SetPinInputMode();
	
	//2.等待DHT11模块把引脚拉低  为了提高程序可靠性，应该人为使用超时处理  100us
	while( DHT11_READ ==SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//说明未响应，原因是超时了
	if(cnt > 100)
	{
		return false;
	}
	
	cnt=0;
	
	//3.如果DHT11把引脚电平拉低，则需要分析是否拉低时间为80us左右 
	while( DHT11_READ ==RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt > 100)
	{
		return false; //说明未响应，原因是超时了
	}
	else
		return true;  //说明响应了
	
}

//读取1bit  0000 000?
uint8_t DHT11_ReadBit(void)
{
	int cnt = 0; //用于计数
	
	//1.等待DHT11把引脚电平拉低
	while( DHT11_READ ==SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	cnt=0;
	
	//2.等待DHT11把引脚电平拉高
	while( DHT11_READ==RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//3.延时28us < us < 70us
	delay_us(40);
	
	return DHT11_READ;
}


bool DHT11_GetValue(uint8_t buf[5])
{
	int i,j;
	
  uint8_t data=0;
	
	//1.发送开始信号
	DHT11_StartSignal();
	
	//2.等待DHT11响应
	if ( DHT11_GetACK() )
	{
		//3.循环读取40bit
		for(i=0;i<5;i++)
		{
			for(j=0;j<8;j++)
			{
				data <<= 1; 
				data |= DHT11_ReadBit();  				 
			}
			
			buf[i] = data;
			data = 0;
		}
		
		//4.进行校验
		if( buf[0]+buf[1]+buf[2]+buf[3] == buf[4] )
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false; //获取数据失败，原因是未响应
	}
}

void Smog_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.打开ADC外设的接口时钟  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	//2.打开GPIO端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.配置GPIO引脚参数+初始化 注意：引脚模式为模拟模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//4.配置ADC外设的参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//预分频值  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//不使用DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样的间隔时间
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.配置ADC外设的转换精度、数据对齐
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//转换精度  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//不扫描
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//不连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不使用外部触发源
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//数据对齐  右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//转换序号
	ADC_Init(ADC2, &ADC_InitStructure);
	
	//6.配置ADC的规则通道  总转换时间 = 3 + 12 = 15周期
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);  

	//7.使能ADC
	ADC_Cmd(ADC2, ENABLE);

}
void Fire_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.打开ADC外设的接口时钟  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//2.打开GPIO端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.配置GPIO引脚参数+初始化 注意：引脚模式为模拟模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//4.配置ADC外设的参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//预分频值  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//不使用DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样的间隔时间
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.配置ADC外设的转换精度、数据对齐
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//转换精度  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//不扫描
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//不连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不使用外部触发源
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//数据对齐  右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//转换序号
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//6.配置ADC的规则通道  总转换时间 = 3 + 12 = 15周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);  

	//7.使能ADC
	ADC_Cmd(ADC1, ENABLE);

}
void LightSensor_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.打开ADC外设的接口时钟  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//2.打开GPIO端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//3.配置GPIO引脚参数+初始化 注意：引脚模式为模拟模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//4.配置ADC外设的参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//预分频值  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//不使用DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样的间隔时间
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.配置ADC外设的转换精度、数据对齐
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//转换精度  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//不扫描
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//不连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不使用外部触发源
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//数据对齐  右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//转换序号
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//6.配置ADC的规则通道  总转换时间 = 3 + 12 = 15周期
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);  

	//7.使能ADC
	ADC_Cmd(ADC3, ENABLE);

}