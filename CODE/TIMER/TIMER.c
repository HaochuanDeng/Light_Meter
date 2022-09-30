#include "TIMER.h"


u8 T2_BUSY_FLAG = 0;  //T2定时器占用标志位(0：未占用；1：“选择”按键占用；2：“加”按键占用；3：“减”按键占用) 
extern u8 key_flag[6];//(0~1:“选择”按键点按、双击标志位；2~3:“加”按键点按、双击标志位；4~5:“减”按键点按、双击标志位)


//初始化T2为16位自动重装模式
//
//参数：无
//
//返回值：无
void T2_Init(void)
{	
	//T2定时器(固定为8位预分频+16位自动重装)(优先级固定为最低)
	TM2PS = 31;                     //对32MHz系统时钟进行32倍预分频
	AUXR &= 0xF3;                   //再进行12分频，定时器模式
  TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2定时器重装值
	TL2   = DOUBLE_CLICK_DLY_TIME_L;//最长单次溢出时间为：786.432ms
	IE2   |= 0x04;                  //允许T2溢出中断
	
	//开放中断
	EA     = 1;                     //开总中断
}


//T2 ISR
//
//参数：无
//
//返回值：无
void T2_ISR(void) interrupt 12
{
	AUXR &= 0xEF;       //停止T2定时器
	switch(T2_BUSY_FLAG)
	{
		case 1:
			key_flag[0] = 1;//发送“选择”按键单机标志位
		break;
		
		case 2:
			key_flag[2] = 1;//发送“加”按键单机标志位
		break;
		
		case 3:
			key_flag[4] = 1;//发送“减”按键单机标志位
		break;
	}
	T2_BUSY_FLAG = 0;   //释放T2定时器占用标志
}

