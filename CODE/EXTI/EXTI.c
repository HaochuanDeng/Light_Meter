#include "EXTI.h"


volatile u8 key_flag[6]={0};//(0~1:“选择”按键点按、双击标志位；2~3:“加”按键点按、双击标志位；4~5:“减”按键点按、双击标志位)
extern u8 T2_BUSY_FLAG;     //T2定时器占用标志位(0：未占用；1：“选择”按键占用；2：“加”按键占用；3：“减”按键占用) 
extern u8 disp_mode;        //OLED显示模式(0~1)


//初始化外部中断0、1、2为下降沿触发，优先级均为最低
//
//参数：无
//
//返回值：无
void EXTI_Init(void)
{
	IT0       = 1;   //下降沿触发
	EX0       = 1;   //允许外部中断0触发中断，P3.2(准双向口)
	
	IT1       = 1;   //下降沿触发
	EX1       = 1;   //允许外部中断1触发中断，P3.3(准双向口)
	
	IP       &= 0xFA;//外部中断0、1优先级设置为最低级
	IPH      &= 0xFA;
	
	INT_CLKO |= 0x10;//允许外部中断2(P3.6准双向口)触发中断，优先级默认且固定为最低级，默认且固定为下降沿触发
	
	P3M1     &= 0xB3;//P3.2、P3.3、P3.6设为准双向口
	P3M0     &= 0xB3;
	
	EA        = 1;   //开总中断
}


//外部中断0(下降沿中断)ISR(“选择”按键)
//
//参数：无
//
//返回值：无
void INT0_Routine(void) interrupt 0
{
	if(T2_BUSY_FLAG == 1)             //T2定时器已启动
	{
		AUXR &= 0xEF;                   //停止T2定时器
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2定时器重装
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED正常显示
		  key_flag[1]  = 1;             //发送双击标志位
		T2_BUSY_FLAG   = 0;             //释放T2定时器占用标志
	}
	else if(T2_BUSY_FLAG == 0)        //T2定时器未启动
	{
		AUXR |= 0x10;                   //启动T2定时器
		T2_BUSY_FLAG = 1;               //“选择”按键占用T2定时器
	}
}


//外部中断1(下降沿中断)ISR(“加”按键)
//
//参数：无
//
//返回值：无
void INT1_Routine(void) interrupt 2
{
	if(T2_BUSY_FLAG == 2)             //T2定时器已启动
	{
		AUXR &= 0xEF;                   //停止T2定时器
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2定时器重装
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED正常显示
		{
			LED_Blink(1,50);              //闪烁LED指示用户
		  key_flag[3]  = 1;             //发送双击标志位
		}
		T2_BUSY_FLAG   = 0;             //释放T2定时器占用标志
	}
	else if(T2_BUSY_FLAG == 0)        //T2定时器未启动
	{
		AUXR |= 0x10;                   //启动T2定时器
		T2_BUSY_FLAG = 2;               //“加”按键占用T2定时器
	}
}


//外部中断2(下降沿中断)ISR(“减”按键)
//
//参数：无
//
//返回值：无
void INT2_Routine(void) interrupt 10
{
	if(T2_BUSY_FLAG == 3)             //T2定时器已启动
	{
		AUXR &= 0xEF;                   //停止T2定时器
		TH2   = DOUBLE_CLICK_DLY_TIME_H;//T2定时器重装
	  TL2   = DOUBLE_CLICK_DLY_TIME_L;
		if(disp_mode == 0)              //OLED正常显示
		{
			LED_Blink(1,50);              //闪烁LED指示用户
		  key_flag[5]  = 1;             //发送双击标志位
		}
		T2_BUSY_FLAG   = 0;             //释放T2定时器占用标志
	}
	else if(T2_BUSY_FLAG == 0)        //T2定时器未启动
	{
		AUXR |= 0x10;                   //启动T2定时器
		T2_BUSY_FLAG = 3;               //“减”按键占用T2定时器
	}
}

