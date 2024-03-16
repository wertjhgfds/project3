/**
 * @brief RGB LED驱动程序
 * @name: ws2812.c
 * @date: 2024-02-29
 * @author: shao
 * 
 */

#include "ws2812.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
/**********************************************
 * 
 *          定义RGB颜色值
 * 
 * ********************************************/
const u32 RGBColors[] =
{
    WS2812_WARM_COLOR,
    WS2812_WHITE_WITH_YELLOW,
    WS2812_PeachPuff,
    WS2812_PaleTurquoise
};
__IO u8 RGBColors_index = 0;    /* 颜色数组的下标 */
__IO float RGBColors_Brightness = 100;   /* RGB灯的亮度，0% ~ 100% */
bool WS2812_State = false;
//-------------------------------------------------------------------------------
//子程序名称:WS2812_Init(void)
//功能：端口初始化
//-------------------------------------------------------------------------------
void WS2812_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_8);
}

//-------------------------------------------------------------------------------
//子程序名称:ResetDateFlow(void)
//功能：复位，为下一次发送做准备，
//说明：将DI置位为0后，延时约65us
//-------------------------------------------------------------------------------
void ResetDataFlow(void)
{
	WS2812_DI = 0;			//DI置为0后，延时50us以上，实现帧复位
    delay_us(60);
}
//-------------------------------------------------------------------------------
//子程序名称:WS2812_SendOnePix(unsigned char *ptr)
//功能：发送一个像素点的24bit数据
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的连续的三个Byte的数据发送
//说明：在主函数中直接调用此函数时，在整个帧发送开始前需要先执行 ResetDataFlow()
//		数据是按归零码的方式发送，速率为800KBPS
//-------------------------------------------------------------------------------
void WS2812_SendOnePix(unsigned char *ptr)
{
	unsigned char i,j;
	unsigned char temp;

	for(j=0;j<3;j++)
	{
		temp=ptr[j];
		for(i=0;i<8;i++)
		{
			if(temp&0x80)		 //从高位开始发送
			{
				WS2812_DI = 1;			 //发送“1”码
                /* 延时0.8us */
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();

				WS2812_DI = 0;
                /* 延时0.4us */
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();

			}
			else				//发送“0”码
			{
				WS2812_DI = 1;
                /* 延时0.4us */
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	

				WS2812_DI = 0;
                /* 延时0.8us */
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
                __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
			}
			temp=(temp<<1);		 //左移位
		}
	}
}
//-------------------------------------------------------------------------------
//子程序名称:WS2812_SendOneFrame(unsigned char *ptr)
//功能：发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整个数组的数据发送
//-------------------------------------------------------------------------------
void WS2812_SendOneFrame(unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

	for(k=0;k<SNUM;k++)				 //发送一帧数据，SNUM是板子LED的个数
	{
		WS2812_SendOnePix(&ptr[(3*k)]);
	}

	ResetDataFlow();				 //发送帧复位信号
}

//-------------------------------------------------------------------------------
//子程序名称:WS2812_SendSameColor(unsigned char *ptr,unsigned char cnt)
//功能：相同颜色的点发送cnt次
//参数：接收的参数是一个指针，指向像素点颜色数组，cnt传递发送个数
//-------------------------------------------------------------------------------
void WS2812_SendSameColor(unsigned char *ptr,unsigned char cnt)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

	for(k=0;k<cnt;k++)				 //发送一帧数据，SNUM是板子LED的个数
	{
		WS2812_SendOnePix(&ptr[0]);
	}

	ResetDataFlow();				 //发送帧复位信号
}
//-------------------------------------------------------------------------------
//子程序名称:WS2812_SendOneFrameFrom(unsigned char i,unsigned char *ptr)
//功能：从指定的像素点开始发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
//		i:把数组的第0个像素数据发送到第i个像素点（第0个像素是板上标号为01的像素）
//说明：即原本对应第一个像素的数据会发送到第i个像素点（LED）上
//-------------------------------------------------------------------------------
void WS2812_SendOneFrameFrom(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

   	for(k=(SNUM-i);k<SNUM;k++)		 //发送一帧数据
	{
		WS2812_SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<(SNUM-i);k++)
	{
		WS2812_SendOnePix(&ptr[(3*k)]);
	}

	ResetDataFlow();				 //发送帧复位信号
}

//-------------------------------------------------------------------------------
//子程序名称:WS2812_SendOneFrameSince(unsigned char i,unsigned char *ptr)
//功能：从第i个像素点的数据开始发送一帧数据（即发送整个数组的数据）
//参数：接收的参数是一个指针，此函数会将此指针指向的地址的整帧数据发送
//		i:把数组的第i个像素数据发送到第1个像素点
//说明：即原本对应第i像素的数据会发送到第1个像素点（LED）上，第i+1个像素点的数据
//		发送到第2个像素上
//-------------------------------------------------------------------------------
void WS2812_SendOneFrameSince(unsigned char i,unsigned char *ptr)
{
	unsigned char k;

	ResetDataFlow();				 //发送帧复位信号

	for(k=i;k<SNUM;k++)				 //发送一帧数据
	{
		WS2812_SendOnePix(&ptr[(3*k)]);
	}
	for(k=0;k<i;k++)
	{
		WS2812_SendOnePix(&ptr[(3*k)]);
	}
	ResetDataFlow();				 //发送帧复位信号
}
/**********************************THE END**********************************/ 
/* R G B */


/* G R B */
const unsigned char WS2812_Clear[3]=
{
    0x00, 0x00, 0x00
};

const unsigned char WS2812_Red[3]=
{
    0x00, 0xFF, 0x00
};
const unsigned char WS2812_Green[3]=
{
    0xFF, 0x00, 0x00
};
const unsigned char WS2812_Blue[3]=
{
    0x00, 0x00, 0xFF
};
const unsigned char WS2812_Yellow[3]=
{
    0x00, 0xFF, 0x00
};

const u8 WS2812_Frame[3 * SNUM]=
{
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,

    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,

    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,

    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x00,
};


/* 
    RGB转GRB
*/
u8* WS2812_RGB2GRB(u8 *ptr, u32 len)
{
    u8 temp;
    u8 *temp_ptr = ptr;
    u8 i;
    for(i=0; i<len; i++)
    {
        temp=temp_ptr[ i + 0 ];
        temp_ptr[ i + 0 ] = temp_ptr[ i + 1 ];
        temp_ptr[ i + 1 ] = temp_ptr[ i + 2 ];
        temp_ptr[ i + 2 ] = temp;
    }
    return ptr;
}

#if 0 
uint32_t changeL(uint32_t rgb, float k) {
	uint8_t r, g, b;
	float h, s, v;
	uint8_t cmax, cmin, cdes;
 
	r = (uint8_t) (rgb >> 16);
	g = (uint8_t) (rgb >> 8);
	b = (uint8_t) (rgb);
 
	cmax = r > g ? r : g;
	if (b > cmax)
		cmax = b;
	cmin = r < g ? r : g;
	if (b < cmin)
		cmin = b;
	cdes = cmax - cmin;
 
	v = cmax / 255.0f;
	s = cmax == 0 ? 0 : cdes / (float) cmax;
	h = 0;
 
	if (cmax == r && g >= b)
		h = ((g - b) * 60.0f / cdes) + 0;
	else if (cmax == r && g < b)
		h = ((g - b) * 60.0f / cdes) + 360;
	else if (cmax == g)
		h = ((b - r) * 60.0f / cdes) + 120;
	else
		h = ((r - g) * 60.0f / cdes) + 240;
 
	//
 
	v *= k;
 
	float f, p, q, t;
	float rf, gf, bf;
	int i = ((int) (h / 60) % 6);
	f = (h / 60) - i;
	p = v * (1 - s);
	q = v * (1 - f * s);
	t = v * (1 - (1 - f) * s);
	switch (i) {
	case 0:
		rf = v;
		gf = t;
		bf = p;
		break;
	case 1:
		rf = q;
		gf = v;
		bf = p;
		break;
	case 2:
		rf = p;
		gf = v;
		bf = t;
		break;
	case 3:
		rf = p;
		gf = q;
		bf = v;
		break;
	case 4:
		rf = t;
		gf = p;
		bf = v;
		break;
	case 5:
		rf = v;
		gf = p;
		bf = q;
		break;
	default:
		break;
	}
 
	r = (uint8_t) (rf * 255.0);
	g = (uint8_t) (gf * 255.0);
	b = (uint8_t) (bf * 255.0);
 
	uint32_t returnColor = ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
	return returnColor;
}
#endif

static float min(float a, float b, float c)
{
  float m;
  
  m = a < b ? a : b;
  return (m < c ? m : c); 
}

static float max(float a, float b, float c)
{
  float m;
  
  m = a > b ? a : b;
  return (m > c ? m : c); 
}
  
void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
  float red, green ,blue;
  float cmax, cmin, delta;
  
  red = (float)r / 255;
  green = (float)g / 255;
  blue = (float)b / 255;
  
  cmax = max(red, green, blue);
  cmin = min(red, green, blue);
  delta = cmax - cmin;

  /* H */
  if(delta == 0)
  {
    *h = 0;
  }
  else
  {
    if(cmax == red)
    {
      if(green >= blue)
      {
        *h = 60 * ((green - blue) / delta);
      }
      else
      {
        *h = 60 * ((green - blue) / delta) + 360;
      }
    }
    else if(cmax == green)
    {
      *h = 60 * ((blue - red) / delta + 2);
    }
    else if(cmax == blue) 
    {
      *h = 60 * ((red - green) / delta + 4);
    }
  }
  
  /* S */
  if(cmax == 0)
  {
    *s = 0;
  }
  else
  {
    *s = delta / cmax;
  }
  
  /* V */
  *v = cmax;
}
  
void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int hi = ((int)h / 60) % 6;
    float f = h * 1.0 / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    switch (hi){
        case 0:
            *r = 255 * v;
            *g = 255 * t;
            *b = 255 * p;
            break;
        case 1:
            *r = 255 * q;
            *g = 255 * v;
            *b = 255 * p;
            break;
        case 2:
            *r = 255 * p;
            *g = 255 * v;
            *b = 255 * t;
            break;
        case 3:
            *r = 255 * p;
            *g = 255 * q;
            *b = 255 * v;
            break;
        case 4:
            *r = 255 * t;
            *g = 255 * p;
            *b = 255 * v;
            break;
        case 5:
            *r = 255 * v;
            *g = 255 * p;
            *b = 255 * q;
            break;
    }
}
/**
 * @brief WS2812显示颜色
 * @param RGB: 颜色值
 *        len: 指定显示灯的个数
 *        brightness：灯的亮度，范围 0.01 ~ 1.00
 * @return void
 */
void WS2812_Show_RGB(u32 RGB, u32 len, float brightness)
{
    float h = 0,s = 0,v = 0;
    uint8_t r = 0,g = 0,b = 0;
    rgb2hsv(RGB >> 16 , RGB >> 8 , RGB , &h , &s , &v);
    hsv2rgb(h, s, brightness, &r, &g, &b);
    uint32_t temp_rgb = ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
    WS2812_SendSameColor((u8*)WS2812_Clear, 16);
    WS2812_SendSameColor((u8 *)WS2812_RGB2GRB((u8 *)&temp_rgb, 1), len);

}

void WS2812_Change_Brightness(u32 RGB, u8 len, float brightness)
{
    float h = 0,s = 0,v = 0;
    uint8_t r = 0,g = 0,b = 0;
    rgb2hsv(RGB >> 16 , RGB >> 8 , RGB , &h , &s , &v);
    hsv2rgb(h, s, brightness, &r, &g, &b);
    uint32_t temp_rgb = ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
    WS2812_SendSameColor((u8 *)WS2812_RGB2GRB((u8 *)&temp_rgb, 1), len);
}


/* 测试 */
//void WS2812_Test(u8 *buf)
void WS2812_Close()
{

   WS2812_Show_RGB((u32 *)WS2812_OFF,8,0.1);
//  WS2812_SendOneFrameFrom(1,(u8 *)WS2812_Red);
//     while(1)
//     {
//         WS2812_SendSameColor((u8 *)WS2812_Red, 16);
//         delay_ms(500);
//         WS2812_SendSameColor((u8 *)WS2812_Green, 16);
//         delay_ms(500);
//         WS2812_SendSameColor((u8 *)WS2812_Blue, 16);
//         delay_ms(500);
//     }
    //WS2812_SendOneFrame((u8 *)WS2812_Frame);

//    /* 字符串分割 */
//    u8 * r = strtok(buf, " ");
//    u8 * g = strtok(NULL, " ");
//    u8 * b = strtok(NULL, " ");
//    
//    u32 rgb = ((uint32_t) atoi((const char *)r) << 16) | 
//                ((uint32_t) atoi((const char *)g) << 8) | 
//                atoi((const char *)b);
//    WS2812_Show_RGB(rgb, 2, 0.1);
}
void WS2812_Open()
{

   WS2812_Show_RGB((u32 *)WS2812_WHITE,8,0.1);
}

