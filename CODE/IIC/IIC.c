#include "IIC.h"


//IIC总线初始化
//
//注意：此函数会初始化IO口、并释放IIC总线
//
//参数：无
//
//返回值：无
void IIC_Init(void)
{
	P1M1  |= 0x30;//P1.4、P1.5设置为开漏输出
	P1M0  |= 0x30;
	P1SR  &= 0xCF;//P1.4、P1.5 IO口电平转换速度设置为快速
	
	SCL = 1;      //SCL拉高
	Delay2us();
	SDA = 1;      //释放SDA
	Delay2us();
}


//产生起始信号
//
//参数：无
//
//返回值：无
void IIC_Start(void)
{
	SDA = 1;  //SDA拉高
	Delay2us();
	SCL = 1;  //SCL拉高
	Delay2us();
	SDA = 0;  //SDA拉低
	Delay2us();
}


//产生停止信号
//
//参数：无
//
//返回值：无
void IIC_Stop(void)
{
	SDA = 0;  //SDA拉低
	Delay2us();
	SCL = 1;  //SCL拉高
	Delay2us();
	SDA = 1;  //释放SDA
	Delay2us();
}


//等待从设备应答
//
//参数：无
//
//返回值：u8：0：从设备未应答(超时)
//            1：从设备有应答
u8 IIC_WaitResp(void)
{
	u8 i = 0;
	
  SDA = 1;                      //释放SDA
	Delay2us();
	SCL = 1;                      //SCL拉高
	Delay2us();
	while((SDA == 1) && (i < 255))//等待从设备将SDA拉低
		i++;
	SCL = 0;                      //SCL拉低
	Delay2us();
	
	//返回值处理
	if(i == 255)
		return 0;//从设备未应答(超时)
	else
		return 1;//从设备有应答
}


//向从设备应答或不应答
//
//参数：u8 resp：0：不向从设备应答
//               1：向从设备应答
//
//返回值：无
void IIC_Resp(u8 resp)
{
	if(resp == 0)     //不向从设备应答
	{
		SCL = 0;        //SCL拉低
		SDA = 1;        //SDA拉高
		Delay2us();
		SCL = 1;        //SCL拉高
		Delay2us();
		SCL = 0;        //SCL拉低
	}
	else if(resp == 1)//向从设备应答
	{
		SCL = 0;        //SCL拉低
		SDA = 0;        //SDA拉低
		Delay2us();
		SCL = 1;        //SCL拉高
		Delay2us();
		SCL = 0;        //SCL拉低
		SDA = 1;        //释放SDA
		Delay2us();
	}
}


//向从设备写一个字节(高位先发)
//
//参数：u8 byte：待写入的字节
//
//返回值：无
void IIC_WriteByte(u8 byte)
{
	u8 i;
	
	for(i=0; i<8; i++)
	{
		byte = byte << 1;//待发送字节最高位移入CY
		SCL  = 0;        //SCL拉低
		Delay2us();
		SDA  = CY;       //SDA送出CY
		Delay2us();
		SCL  = 1;        //SCL拉高
		Delay2us();
	}
	SCL = 0;           //SCL拉低
	Delay2us();
	SDA = 1;           //释放SDA
	Delay2us();
}


//向从设备读一个字节(先接收到的放在高位)
//
//参数：无
//
//返回值：u8：读到的字节
u8 IIC_ReadByte(void)
{
	u8 i,k = 0;
	
	SCL = 0;  //SCL拉低
	Delay2us();
	SDA = 1;  //释放SDA
	for(i=0; i<8; i++)
	{
		SCL = 1;           //SCL拉高
		Delay2us();
		k = (k << 1) | SDA;//存入获取的比特
		SCL = 0;           //SCL拉低
		Delay2us();
	}
	
	return k;
}

