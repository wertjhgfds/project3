#include "adc.h"
uint8_t dhtbuf[5] = {0};
//DHT11�ĳ�ʼ��
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


//��ʼ�ź�
void DHT11_StartSignal(void)
{
	//1.��������Ϊ���ģʽ
	DHT11_SetPinOutputMode();
	
	//2.ȷ������Ϊ����״̬
	DHT11_SET(1);

	//3.���������Ͳ���������18ms
	DHT11_SET(0);
  delay_ms(20);

	//4.���������߲�����20~40us
	DHT11_SET(1);
	delay_us(30);
}

//��Ӧ�ź�
bool  DHT11_GetACK(void)
{
	int cnt = 0; //���ڼ���
	
	//1.��������Ϊ����ģʽ
	DHT11_SetPinInputMode();
	
	//2.�ȴ�DHT11ģ�����������  Ϊ����߳���ɿ��ԣ�Ӧ����Ϊʹ�ó�ʱ����  100us
	while( DHT11_READ ==SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//˵��δ��Ӧ��ԭ���ǳ�ʱ��
	if(cnt > 100)
	{
		return false;
	}
	
	cnt=0;
	
	//3.���DHT11�����ŵ�ƽ���ͣ�����Ҫ�����Ƿ�����ʱ��Ϊ80us���� 
	while( DHT11_READ ==RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	if(cnt > 100)
	{
		return false; //˵��δ��Ӧ��ԭ���ǳ�ʱ��
	}
	else
		return true;  //˵����Ӧ��
	
}

//��ȡ1bit  0000 000?
uint8_t DHT11_ReadBit(void)
{
	int cnt = 0; //���ڼ���
	
	//1.�ȴ�DHT11�����ŵ�ƽ����
	while( DHT11_READ ==SET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	cnt=0;
	
	//2.�ȴ�DHT11�����ŵ�ƽ����
	while( DHT11_READ==RESET && cnt < 100)
	{
		delay_us(1);
		cnt++;
	}
	
	//3.��ʱ28us < us < 70us
	delay_us(40);
	
	return DHT11_READ;
}


bool DHT11_GetValue(uint8_t buf[5])
{
	int i,j;
	
  uint8_t data=0;
	
	//1.���Ϳ�ʼ�ź�
	DHT11_StartSignal();
	
	//2.�ȴ�DHT11��Ӧ
	if ( DHT11_GetACK() )
	{
		//3.ѭ����ȡ40bit
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
		
		//4.����У��
		if( buf[0]+buf[1]+buf[2]+buf[3] == buf[4] )
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		return false; //��ȡ����ʧ�ܣ�ԭ����δ��Ӧ
	}
}

void Smog_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.��ADC����Ľӿ�ʱ��  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	//2.��GPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//4.����ADC����Ĳ���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//Ԥ��Ƶֵ  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//��ʹ��DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���β����ļ��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.����ADC�����ת�����ȡ����ݶ���
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//ת������  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ʹ���ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//���ݶ���  �Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//ת�����
	ADC_Init(ADC2, &ADC_InitStructure);
	
	//6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);  

	//7.ʹ��ADC
	ADC_Cmd(ADC2, ENABLE);

}
void Fire_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.��ADC����Ľӿ�ʱ��  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//2.��GPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//4.����ADC����Ĳ���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//Ԥ��Ƶֵ  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//��ʹ��DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���β����ļ��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.����ADC�����ת�����ȡ����ݶ���
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//ת������  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ʹ���ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//���ݶ���  �Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//ת�����
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);  

	//7.ʹ��ADC
	ADC_Cmd(ADC1, ENABLE);

}
void LightSensor_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.��ADC����Ľӿ�ʱ��  PF7 -- ADC3_IN5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//2.��GPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//3.����GPIO���Ų���+��ʼ�� ע�⣺����ģʽΪģ��ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//4.����ADC����Ĳ���
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;										//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;									//Ԥ��Ƶֵ  84MHZ /2 = 42MHZ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;			//��ʹ��DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���β����ļ��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//5.����ADC�����ת�����ȡ����ݶ���
	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;										//ת������  12bit
	ADC_InitStructure.ADC_ScanConvMode 	= DISABLE;															//��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;													//������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ʹ���ⲿ����Դ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;											//���ݶ���  �Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;																	//ת�����
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//6.����ADC�Ĺ���ͨ��  ��ת��ʱ�� = 3 + 12 = 15����
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);  

	//7.ʹ��ADC
	ADC_Cmd(ADC3, ENABLE);

}