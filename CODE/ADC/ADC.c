#include "ADC.h"


//初始化ADC
//
//参数：无
//
//返回值：无
void ADC_Init(void)
{	
	P1M1  |= 0x09;//P1.0->ADC0(高阻输入模式)、P1.3->ADC3(高阻输入模式)
	P1M0  &= 0xF6;
	P1IE  &= 0xF6;//P1.0、P1.3禁止数字信号输入
	
	ADCCFG = 0x21;//转换结果右对齐、ADC时钟频率8MHz
	ADCTIM = 0xFF;//T_setup=2T，T_hold=4T，T_duty=32T，T_convert=10T，F_adc=380.952KHz
}


//ADC对指定通道的模拟电压进行转换
//
//参数：u8 chan：0：3.3V系统供电电压 | ADC0 | P1.0
//               1： 0.5倍锂电池电压 | ADC3 | P1.3
//
//返回值：u16：转换结果，低10位有效
u16 ADC_Convert(u8 chan)
{
	//设置ADC转换通道
	if(chan == 0)       //3.3V电压|ADC0|P1.0
	{
		ADC_CONTR &= 0xF0;//清除ADC_CHS[3:0]，设置转换通道为ADC0
	}
	else if(chan == 1)  //3V电压|ADC3|P1.3
	{
		ADC_CONTR &= 0xF0;//清除ADC_CHS[3:0]
		ADC_CONTR |= 0x03;//设置转换通道为ADC3
	}
	
	//开始转换
	ADC_CONTR |= 0x80;                //打开ADC电源
	DelayXms(1);                      //等待ADC电源稳定
	ADC_CONTR |= 0x40;                //启动ADC转换
	while((ADC_CONTR & 0x20) == 0x00);//等待转换结束
	ADC_CONTR &= 0xDF;                //清除转换结束标志位
	ADC_CONTR &= 0x7F;                //关闭ADC电源
	
	return (((u16)ADC_RES<<8) | ((u16)ADC_RESL));
}

