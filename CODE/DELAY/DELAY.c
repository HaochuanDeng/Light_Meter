#include "DELAY.h"


//软件延时1ms
//
//注意：@32MHz(系统时钟)
//
//参数：无
//
//返回值：无
static void Delay1ms(void)
{
	u8 i,j;

	_nop_();
	i = 42;
	j = 140;
	do
	{
		while(--j);
	}while(--i);
}


//软件延时 X ms
//
//注意：@32MHz(系统时钟)
//
//参数：u16 x：延时 X ms(0~65535)
//
//返回值：无
void DelayXms(u16 x)
{
	for(x=x;x>0;x--)
	  Delay1ms();
}


//软件延时2us
//
//注意：@32MHz(系统时钟)，用于控制软件模拟的IIC总线SCL时钟频率：Tscl = 2 * 2us = 4us，Fscl = 1s / Tscl = 250KHz
//
//参数：无
//
//返回值：无
void Delay2us(void)
{
	u8 i;

	_nop_();
	i = 19;
	while(--i);
}

