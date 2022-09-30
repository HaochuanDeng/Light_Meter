#include "RCC.h"


//初始化外部32MHz晶体振荡器
//
//注意：在函数main()中必须要第1个被调用
//主时钟(晶振产生)：32MHz
//系统时钟：        32MHz
//XFR区域访问：     使能
//
//参数：无
//
//返回值：无
void RCC_Init(void)
{
	P_SW2    |= 0x80;              //使能访问XFR区域：0x0FA00~0x0FFFF
	CLKDIV    = 0x00;              //系统时钟为主时钟的1分频
	XOSCCR   |= 0xC0;              //开启外部晶体振荡器
	while((XOSCCR & 0x01) == 0x00);//等待外部晶体振荡器稳定
	CLKSEL    = 0x01;              //主时钟源切换为外部晶体振荡器
	IRC24MCR &= 0x7F;              //关闭内部时钟振荡器IRC
}

