#include "AE.h"


//AE算法单次采样数
volatile u8 AE_SAMPLE_SIZE;

//光圈数值对应表
float xdata F_Stop [7];

//快门时间对应表(单位：s)
float xdata S_Speed[16];

//ISO/ASA对应表
float xdata ISO    [21];

//测光表校准系数K(数值越大，曝光越多)(默认：20.0)
union K_union
{
	u8    x[4];
	float t;
}K;


//初始化AE
//
//注意：将光圈、快门、ISO/ASA对照表从EEPROM中读出
//
//参数：无
//
//返回值：无
void AE_Init(void)
{
	u8    i;
	union temp
  {
	  u8    x[4];
	  float t;
  }D;
 
  //读EEPROM
  for(i=0;i<44;i++)
	{
		EEPROM_Read(D.x,1,i*4,4);
		
		if(i < 7)                    //光圈对应表
			F_Stop[i]    = D.t;
		else if((i > 6)  && (i < 23))//快门对应表
			S_Speed[i-7] = D.t;
		else if((i > 22) && (i < 44))//ISO对应表
			ISO[i-23]    = D.t;
	}
}


//根据ISO、光圈，计算快门时间(光圈优先测光)
//
//注意：使用ESAM算法，即指数平滑、算数平均算法
//
//参数：u8 f：  当前光圈(数组索引)(0~6)
//      u8 iso：当前ISO(数组索引)(0~20)
//
//返回值：float：计算得到的快门时间(单位：s)
float ESAM_Calc_S_Speed(u8 f, u8 iso)
{
	u8 x;
	float data sum                             =   0;
	float data temp       [AE_SAMPLE_SIZE_MAX] = {0};
	float data temp_smooth[AE_SAMPLE_SIZE_MAX] = {0};
	
	//对环境光进行多次采样
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = Lux_Calc();//采样一次
		LED_Blink(1,10);     //闪烁LED指示用户
	}
	
	//根据曝光方程计算快门时间
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = ((F_Stop[f]*F_Stop[f])*K.t) / (temp[x]*ISO[iso]);
	}
	
	//对计算出的快门时间进行指数平滑 & 算数平均
	temp_smooth[0] = temp[0];//初态
	sum           += temp_smooth[0];
	for(x=1;x<AE_SAMPLE_SIZE;x++)
	{
		temp_smooth[x] = (EXP_SMOOTH_PARA*temp[x-1]) + ((1.0-EXP_SMOOTH_PARA)*temp_smooth[x-1]);
		sum           += temp_smooth[x];
	}
	sum /= AE_SAMPLE_SIZE;
	
	//闪烁LED指示用户操作完成
	LED_Blink(2,30);
	
	return sum;//返回快门时间
}


//根据ISO、快门时间，计算光圈(快门优先测光)
//
//注意：使用ESAM算法，即指数平滑、算数平均算法
//
//参数：u8 s：  当前快门时间(数组索引)(0~15)
//      u8 iso：当前ISO(数组索引)(0~20)
//
//返回值：float：计算得到的光圈
float ESAM_Calc_F_Stop(u8 s, u8 iso)
{
	u8 x;
	float data sum                             =   0;
	float data temp       [AE_SAMPLE_SIZE_MAX] = {0};
	float data temp_smooth[AE_SAMPLE_SIZE_MAX] = {0};
	
	//对环境光进行多次采样
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = Lux_Calc();//采样一次
		LED_Blink(1,10);     //闪烁LED指示用户
	}
	
	//根据曝光方程计算光圈
	for(x=0;x<AE_SAMPLE_SIZE;x++)
	{
		temp[x] = sqrt((temp[x]*ISO[iso]*S_Speed[s]) / K.t);
	}
	
	//对计算出的光圈进行指数平滑 & 算数平均
	temp_smooth[0] = temp[0];//初态
	sum           += temp_smooth[0];
	for(x=1;x<AE_SAMPLE_SIZE;x++)
	{
		temp_smooth[x] = (EXP_SMOOTH_PARA*temp[x-1]) + ((1.0-EXP_SMOOTH_PARA)*temp_smooth[x-1]);
		sum           += temp_smooth[x];
	}
	sum /= AE_SAMPLE_SIZE;
	
	//闪烁LED指示用户操作完成
	LED_Blink(2,30);
	
	return sum;//返回光圈
}

