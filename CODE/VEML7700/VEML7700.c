#include "VEML7700.h"


//写VEML7700寄存器
//
//注意：调用此函数前需要调用 IIC_Init() 函数初始化IIC总线(初始化一次即可)
//
//参数：u8  addr：待写入的寄存器地址(已在 .h 文件中定义)
//      u16 date：待写入的数据(16位)
//
//返回值：无
static void VEML7700_Write(u8 addr, u16 date)
{
	IIC_Start();                   //产生起始信号
	IIC_WriteByte(VEML7700_ADDR_W);//发送从机地址(写)
	IIC_WaitResp();                //等待从机响应
	IIC_WriteByte(addr);           //发送寄存器地址
	IIC_WaitResp();                //等待从机响应
	IIC_WriteByte((u8)date);       //发送LSB
	IIC_WaitResp();                //等待从机响应
	IIC_WriteByte((u8)(date>>8));  //发送MSB
	IIC_WaitResp();                //等待从机响应
	IIC_Stop();                    //产生停止信号
}


//读VEML7700寄存器
//
//注意：调用此函数前需要调用 IIC_Init() 函数初始化IIC总线(初始化一次即可)
//
//参数：u8 addr：待读取寄存器的地址(已在 .h 文件中定义)
//
//返回值：u16：0xXXXX：读取到的数据(16位)
//             0x0000：读取可能失败(也有可能读到正确的值但全为0)
static u16 VEML7700_Read(u8 addr)
{
	u8 LSB,MSB;
	
	IIC_Start();                        //产生起始信号
	IIC_WriteByte(VEML7700_ADDR_W);     //发送从机地址(写)
	if(IIC_WaitResp() == 0)             //等待从机响应
		return 0;                         //从机未响应，返回
	IIC_WriteByte(addr);                //发送待读取的寄存器地址
	if(IIC_WaitResp() == 0)             //等待从机响应
		return 0;                         //从机未响应，返回
	IIC_Start();                        //重新产生起始信号
	IIC_WriteByte(VEML7700_ADDR_R);     //发送从机地址(读)
	if(IIC_WaitResp() == 0)             //等待从机响应
		return 0;                         //从机未响应，返回
	LSB = IIC_ReadByte();               //接收LSB
	IIC_Resp(1);                        //向从机应答
	MSB = IIC_ReadByte();               //接受MSB
	IIC_Resp(0);                        //不向从机应答
	IIC_Stop();                         //产生停止信号
	
	return (((u16)MSB)<<8) | ((u16)LSB);//返回读到的数据
}


//设置 ALS 增益
//
//参数：VEML7700_ALS_GAIN_TypeDef gain：
//       ALS_gain_1   = ALS gain x 1
//       ALS_gain_2   = ALS gain x 2
//       ALS_gain_1_8 = ALS gain x (1/8)
//       ALS_gain_1_4 = ALS gain x (1/4)
//
//返回值：无
void ALS_GAIN(VEML7700_ALS_GAIN_TypeDef gain)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//读取寄存器
	temp &= 0xE7FF;                   //清除相关位
	temp |= gain << 11;               //设置相关位
	
	VEML7700_Write(ALS_CONF_0,temp);  //写入寄存器
}


//设置 ASL 积分时间
//
//参数：VEML7700_ALS_IT_TypeDef it：
//       ms25  = 25ms
//       ms50  = 50ms
//       ms100 = 100ms
//       ms200 = 200ms
//       ms400 = 400ms
//       ms800 = 800ms
//
//返回值：无
void ALS_IT(VEML7700_ALS_IT_TypeDef it)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//读取寄存器
	temp &= 0xFC3F;                   //清除相关位
	temp |= it << 6;                  //设置相关位
	
	VEML7700_Write(ALS_CONF_0,temp);  //写入寄存器
}

/*
//设置 ALS_PERS 乘积系数
//
//参数：VEML7700_ALS_PERS_TypeDef pers：
//       p1 = 1
//       p2 = 2
//       p4 = 4
//       p8 = 8
//
//返回值：无
void ALS_PERS(VEML7700_ALS_PERS_TypeDef pers)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//读取寄存器
	temp &= 0xFFCF;                   //清除相关位
	temp |= pers << 4;                //设置相关位
	
	VEML7700_Write(ALS_CONF_0,temp);  //写入寄存器
}
*/

//设置 ALS 中断使能/失能
//
//参数：VEML7700_ALS_INT_EN_TypeDef en：
//       EN  = ALS INT enable
//       DIS = ALS INT disable
//
//返回值：无
void ALS_INT_EN(VEML7700_ALS_INT_EN_TypeDef en)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//读取寄存器
	temp &= 0xFFFD;                   //清除相关位
	temp |= en << 1;                  //设置相关位
	
	VEML7700_Write(ALS_CONF_0,temp);  //写入寄存器
}


//设置 ALS 掉电使能/失能
//
//参数：VEML7700_ALS_SD_TypeDef power：
//       ON  = ALS power on
//       OFF = ALS shut down
//
//返回值：无
void ALS_SD(VEML7700_ALS_SD_TypeDef power)
{
	u16 temp;
	
	temp  = VEML7700_Read(ALS_CONF_0);//读取寄存器
	temp &= 0xFFFE;                   //清除相关位
	temp |= power;                    //设置相关位
	
	VEML7700_Write(ALS_CONF_0,temp);  //写入寄存器
}

/*
//设置 INT 触发高阈值
//
//参数：u16 H_Thre：待设置的高阈值
//
//返回值：无
void ALS_H_THRE(u16 H_Thre)
{
	VEML7700_Write(ALS_WH,H_Thre);//写入寄存器
}


//设置 INT 触发低阈值
//
//参数：u16 L_Thre：待设置的低阈值
//
//返回值：无
void ALS_L_THRE(u16 L_Thre)
{
	VEML7700_Write(ALS_WL,L_Thre);//写入寄存器
}


//设置 PSM 省电模式
//
//参数：VEML7700_PSM_TypeDef mode：
//       m1 = mode 1
//       m2 = mode 2
//       m3 = mode 3
//       m4 = mode 4
//
//返回值：无
void PSM(VEML7700_PSM_TypeDef mode)
{
	u16 temp;
	
	temp  = VEML7700_Read(POWER_SAVING);//读取寄存器
	temp &= 0xFFF9;                     //清除相关位
	temp |= mode << 1;                  //设置相关位
	
	VEML7700_Write(POWER_SAVING,temp);  //写入寄存器
}
*/

//设置 PSM_EN 省电模式使能/失能
//
//参数：VEML7700_PSM_EN_TypeDef en：
//       enable  = enable
//       disable = disable
//
//返回值：无
void PSM_EN(VEML7700_PSM_EN_TypeDef en)
{
	u16 temp;
	
	temp  = VEML7700_Read(POWER_SAVING);//读取寄存器
	temp &= 0xFFFE;                     //清除相关位
	temp |= en;                         //设置相关位
	
	VEML7700_Write(POWER_SAVING,temp);  //写入寄存器
}


//读取 ALS 输出数据
//
//参数：无
//
//返回值：u16：0xXXXX：读取到的数据(16位)
//             0x0000：读取可能失败(也有可能读到正确的值但全为0)
u16 ALS_DATA_Read(void)
{
	return (VEML7700_Read(ALS));
}

/*
//读取 WHITE 通道数据
//
//参数：无
//
//返回值：u16：0xXXXX：读取到的数据(16位)
//             0x0000：读取可能失败(也有可能读到正确的值但全为0)
u16 WHITE_DATA_Read(void)
{
	return (VEML7700_Read(WHITE));
}


//获取 INT 低阈值状态
//
//参数：无
//
//返回值：u8：位状态(0/1)
u8 INT_L_Read(void)
{
	return ((u8)((VEML7700_Read(ALS_INT) & 0x8000) >> 15));
}


//获取 INT 高阈值状态
//
//参数：无
//
//返回值：u8：位状态(0/1)
u8 INT_H_Read(void)
{
	return ((u8)((VEML7700_Read(ALS_INT) & 0x4000) >> 14));
}
*/

//初始化VEML7700
//
//参数：无
//
//返回值：无
void VEML7700_Init(void)
{
	ALS_INT_EN(DIS);//关闭 INT 功能
	PSM_EN(disable);//关闭省电模式
}


//计算当前照度
//
//注意：本函数参照官方应用指南，包含AGC、AITC、以及非线性矫正
//
//参数：无
//
//返回值：float：当前照度(单位：lx 或 Cd/m^2)
float Lux_Calc(void)
{
	char IT      = 0;      //-2、-1、0、1、2、3
	char G       = 1;      //1、2、3、4
	u16 ALS_Data = 0;      //ALS原始数据
	float temp   = 0.0;
	
	ALS_SD(ON);            //芯片上电
	DelayXms(3);           //上电延时3ms
	
	ALS_IT(ms100);         //积分时间初始化为100ms
	ALS_GAIN(ALS_gain_1_8);//ALS增益初始化为1/8

L1:	
	switch(IT)             //延时，等待数据有效
	{
		case 0:
			DelayXms(200);
		  break;
		
		case 1:
			DelayXms(400);
		  break;
		
		case 2:
			DelayXms(800);
		  break;
		
		case 3:
			DelayXms(1600);
		  break;
	}
             
	ALS_Data = ALS_DATA_Read();  //读ALS_Data
	if(ALS_Data <= 100)          //ALS_Data <= 100
	{
		ALS_SD(OFF);               //芯片掉电
		
		if(G != 4)
		  G  += 1;                 //切换ALS增益，缩小量程
		switch(G)                  //切换ALS增益，执行
		{
			case 2:
				ALS_GAIN(ALS_gain_1_4);//ALS增益设置为1/4
			  break;
				
			case 3:
				ALS_GAIN(ALS_gain_1);  //ALS增益设置为1
			  break;
				
			case 4:
				ALS_GAIN(ALS_gain_2);  //ALS增益设置为2
			  break;
		}
		
		if(G == 4)                 //量程已达最小
		{
			IT += 1;                 //增加积分时间
			switch(IT)               //增加积分时间，执行
			{
				case 1:
					ALS_IT(ms200);       //积分时间设置为200ms
					break;
					
				case 2:
					ALS_IT(ms400);       //积分时间设置为400ms
					break;
					
				case 3:
					ALS_IT(ms800);       //积分时间设置为800ms
					break;
					
				case 4:                //变量已超出范围
					break;
			}
			
			if(IT == 4)              //积分时间已达最大
			{
				ALS_SD(OFF);                        //芯片掉电
				return (ALS_Data * ((float)0.0036));//返回当前照度值
			}
			else                     //积分时间未达最大
			{
				ALS_SD(ON);            //芯片上电
	      DelayXms(3);           //上电延时3ms
			
			  goto L1;               //再次测量
			}
		}
		else                       //量程未达最小
		{
			ALS_SD(ON);              //芯片上电
	    DelayXms(3);             //上电延时3ms
			
			goto L1;                 //再次测量
		}
	}
	else                         //ALS_Data >  100
	{
L2:
		if(ALS_Data > 10000)       //ALS_Data > 10000
		{
			ALS_SD(OFF);             //芯片掉电
		
			IT -= 1;                 //缩短积分时间
			switch(IT)               //缩短积分时间，执行
			{
				case 2:
					ALS_IT(ms400);       //积分时间设置为400ms
					break;
					
				case 1:
					ALS_IT(ms200);       //积分时间设置为200ms
					break;
					
				case 0:
					ALS_IT(ms100);       //积分时间设置为100ms
					break;
					
				case -1:
					ALS_IT(ms50);        //积分时间设置为50ms
					break;
					
				case -2:
					ALS_IT(ms25);              //积分时间设置为25ms
					
					ALS_SD(ON);                //芯片上电
	        DelayXms(3);               //上电延时3ms
				
				  DelayXms(50);              //等待数据有效
				  ALS_Data = ALS_DATA_Read();//重新读取ALS_Data
					break;
			}
			
			if(IT == -2)                   //积分时间已达最短
			{
				switch(G)                    //根据当前量程与积分时间计算照度
				{
					case 1:
						temp = ALS_Data * ((float)1.8432);
					  break;
					
					case 2:
						temp = ALS_Data * ((float)0.9216);
					  break;
					
					case 3:
						temp = ALS_Data * ((float)0.2304);
					  break;
					
					case 4:
						temp = ALS_Data * ((float)0.1125);
					  break;
				}
				temp = temp*(temp*(temp*(C1*temp-C2)+C3)+C4);
				ALS_SD(OFF);       //芯片掉电
				return temp;       //返回当前照度
			}
			else                 //积分时间未达最短
			{
				ALS_SD(ON);        //芯片上电
	      DelayXms(3);       //上电延时3ms
				
				switch(IT)         //延时，等待数据有效
				{
					case 2:
						DelayXms(800);
					  break;
					
					case 1:
						DelayXms(400);
					  break;
					
					case 0:
						DelayXms(200);
					  break;
					
					case -1:
						DelayXms(100);
					  break;
				}
				
				ALS_Data = ALS_DATA_Read();//重新读取ALS_Data
				goto L2;                   //再次判断ALS_Data范围
			}
		}
		else                           //100 < ALS_Data <= 10000
		{
			switch(IT)                   //根据当前量程与积分时间计算照度
			{
				case 3:                    //800ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.0288);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0072);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0036);
							break;
					}
					break;
				
				case 2:                    //400ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0144);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0072);
							break;
					}
					break;
				
				case 1:                    //200ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.2304);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0288);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0144);
							break;
					}
					break;
				
				case 0:                    //100ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.4608);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.2304);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.0576);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0288);
							break;
					}
					break;
				
				case -1:                   //50ms
					switch(G)
					{
						case 1:
							temp = ALS_Data * ((float)0.9216);
							break;
						
						case 2:
							temp = ALS_Data * ((float)0.4608);
							break;
						
						case 3:
							temp = ALS_Data * ((float)0.1152);
							break;
						
						case 4:
							temp = ALS_Data * ((float)0.0576);
							break;
					}
					break;
			}
			temp = temp*(temp*(temp*(C1*temp-C2)+C3)+C4);
			ALS_SD(OFF);                 //芯片掉电
			return temp;                 //返回当前照度
		}
	}
}

