#include "LED.h"
#include "RCC.h"
#include "EXTI.h"
#include "EEPROM.h"
#include "ADC.h"
#include "DELAY.h"
#include "TIMER.h"
#include "VEML7700.h"
#include "OLED.h"
#include "AE.h"
#include "STATE_MACHINE.h"


u16   temp_u;           //16位无符号临时变量
float temp_f;           //32位浮点临时变量


//主函数，程序入口
void main(void)
{
	//硬件初始化
	RCC_Init();           //初始化外部晶体振荡器(32MHz)
	ADC_Init();           //初始化片上ADC
	EEPROM_Init();        //初始化片上EEPROM
	IIC_Init();           //初始化IIC总线
	OLED_Init();          //初始化OLED模块
	
	//显示固件版本号
	OLED_Disp(1,1,10,0,1);//v1.01
	DelayXms(400);        //适当延时
	
	//显示当前电池电量(0~9999,百分比制)
	temp_f = ((6.6*(ADC_Convert(1)/1024.0))-2.6)/1.5;             //计算当前电量百分比
	temp_u = (u16)(temp_f*10000);                                 //放大10000倍，便于分离各位数
	OLED_Disp(1,temp_u/1000,temp_u/100%10,temp_u/10%10,temp_u%10);//OLED显示
	
	//准备进入状态机
	LED_Config();         //初始化LED
	VEML7700_Init();      //初始化环境光传感器VEML7700
	AE_Init();            //初始化AE
	SM_Init();            //初始化状态机
	T2_Init();            //初始化T2定时器
	EXTI_Init();          //初始化EXTI
	
	//进入状态机
	while(1)
	{
		SM_Run();           //轮询状态机
		DelayXms(10);       //适当延时，使中断能够被ISR响应
	}
}

