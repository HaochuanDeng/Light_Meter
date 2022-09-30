#include "LED.h"


//设置LED状态
//
//参数：u8 LED_STATUS：1  点亮LED
//                     0  熄灭LED
//
//返回值：无
static void LED_SET(u8 LED_STATUS)
{
	if(LED_STATUS == 0)     //熄灭LED
		P11 = 1;
	else if(LED_STATUS == 1)//点亮LED
    P11 = 0; 
}


//初始化LED用到的GPIO口
//
//参数：无
//
//返回值：无
void LED_Config(void)
{
	P1DR &= 0xFD;//增强驱动能力
	P1M1 &= 0xFD;//推挽输出模式，最大电流20mA
	P1M0 |= 0x02;
	LED_SET(0);  //默认熄灭LED
}


//闪烁LED
//
//注意：1次LED闪烁包括：亮(维持dely毫秒)，灭(维持dely毫秒)
//
//参数：u8 num：  闪烁次数(1~255)
//      u16 dely：LED状态翻转间隔时间(单位：ms)(0~65535)
//
//返回值：无
void LED_Blink(u8 num, u16 dely)
{
	u8 x;
	
	for(x=0;x<num;x++)
	{
		LED_SET(1);
		DelayXms(dely);
		LED_SET(0);
		DelayXms(dely);
	}
}

