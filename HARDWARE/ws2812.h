
#ifndef _WS2812_H_
#define _WS2812_H_

#include "sys.h"
/*******************************通信接口配置***********************************/
#define WS2812_DI PEout(8)


/*----------------------------灯珠串联个数设置--------------------------------*/  
#define SNUM 8

/**-----------------------------灯珠颜色设置--------------------------------**/
#define WS2812_RED 0x00FF0000       /*红色*/
#define WS2812_GREEN 0x0000FF00     /*绿色*/
#define WS2812_BLUE 0x000000FF      /*蓝色*/
#define WS2812_YELLOW 0x00FFFF00    /* 黄色 */
#define WS2812_WHITE 0x00FFFFFF     /* 白色 */
#define WS2812_PURPLE 0x00FF00FF    /* 紫色 */
#define WS2812_CYAN 0x0000FFFF      /* 青绿色 */
#define WS2812_PINK 0x00FF0080      /* 粉红 */
#define WS2812_ORANGE 0x00FF8000    /* 橙色 */
#define WS2812_BROWN 0x00804000     /* 棕色 */
#define WS2812_YELLOW_GREEN 0x7FFF00    /* 绿色 */

#define WS2812_OFF               0x000000    /* 关灯 */
#define WS2812_WARM_COLOR        0x8B4513    /* 暖白色 */
#define WS2812_WHITE_WITH_YELLOW 0xEEB422    /* 白中带点黄 */
#define WS2812_PeachPuff         0xFFDAB9    /* 浅粉红 */ 
#define WS2812_PaleTurquoise     0xAFEEEE    /* 浅天蓝 */


//-------------------------THE END------------------------*/



/*--------------------------------函数声明------------------------------------*/   
void WS2812_Init(void);
void ResetDataFlow(void);
void WS2812_SendOnePix(unsigned char *ptr);
void WS2812_SendOneFrame(unsigned char *ptr);
void WS2812_SendSameColor(unsigned char *ptr,unsigned char cnt);
void WS2812_SendOneFrameFrom(unsigned char i,unsigned char *ptr);
void WS2812_SendOneFrameSince(unsigned char i,unsigned char *ptr);
void WS2812_Close(void);
void WS2812_Open(void);
void WS2812_Show_RGB(u32 RGB, u32 len, float brightness);
void WS2812_Change_Brightness(u32 RGB, u8 len, float brightness);
#endif

//-------------------------THE END------------------------*/   

