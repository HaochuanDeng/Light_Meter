#include "STATE_MACHINE.h"


//索引   说明                  范围
//
//0：    光圈值                (0~6)
//1~4：  光圈计算值             float
//5：    快门时间              (0~15)
//6~9：  快门时间计算值         float
//10：   ISO/ASA               (0~20)
//11：   当前显示位             (0~4)
//12：   高级测光参数设置开关位 (0~1)
//13：   测光模式选择位         (0~1)
//14：   AE算法单次采样数       (1~AE_SAMPLE_SIZE_MAX)
//15~18：测光表校准系数         float
volatile u8 ROM_DATA[19]; //存放于片上EEPROM(扇区：0，最起始的19个字节)
volatile u8 state     = 0;//状态机状态位(0~1)
volatile u8 disp_mode = 0;//OLED显示模式(0~1)
union       F_S_union     //测光得到的光圈与快门数值
{
	u8    x[4];
	float t;
}F,S;

//0~1:“选择”按键点按、双击标志位
//2~3:“  加”按键点按、双击标志位
//4~5:“  减”按键点按、双击标志位
extern u8    key_flag[6];   //按键标志位
extern u8    AE_SAMPLE_SIZE;//AE算法单次采样数
extern float F_Stop  [7];   //光圈数值对应表
extern float S_Speed [16];  //快门时间对应表(单位：s)
extern float ISO     [21];  //ISO/ASA对应表
extern union K_union        //测光表校准系数K(数值越大，曝光越多)(默认：20.0)
{
	u8    x[4];
	float t;
}K;


//初始化状态机
//
//注意：从EEPROM中加载数据，初始化相关变量
//
//参数：无
//
//返回值：无
void SM_Init(void)
{
	//加载EEPROM数据
	EEPROM_Read(ROM_DATA,0,0,19);
	
	//相关变量初始化
	F.x[0]         = ROM_DATA[1]; //光圈计算值
	F.x[1]         = ROM_DATA[2];
	F.x[2]         = ROM_DATA[3];
	F.x[3]         = ROM_DATA[4];
	S.x[0]         = ROM_DATA[6]; //快门时间计算值
	S.x[1]         = ROM_DATA[7];
	S.x[2]         = ROM_DATA[8];
	S.x[3]         = ROM_DATA[9];
	AE_SAMPLE_SIZE = ROM_DATA[14];//AE算法单次采样数
	K.x[0]         = ROM_DATA[15];//测光表校准系数
	K.x[1]         = ROM_DATA[16];
	K.x[2]         = ROM_DATA[17];
	K.x[3]         = ROM_DATA[18];
}


//状态机
//
//注意：在主函数中不断运行
//
//参数：无
//
//返回值：无
void SM_Run(void)
{
	u8 d[4];
	
	EA = 0;                          //进入临界区
	
	/****************逻辑部分****************/
	//状态转移 & 变量操作
	if(state == 0)                   //状态0，主界面维护
	{
		if(key_flag[0]      == 1)      //设置光圈、快门、ISO
		{
			key_flag[0]   = 0;
			if((ROM_DATA[11] < 3) || (ROM_DATA[12] == 1))
			{
			  disp_mode   = 1;
			  state       = 1;
			}
		}
		else if(key_flag[1] == 1)      //测光
		{
			key_flag[1] = 0;
			if(ROM_DATA[13]   == 0)      //光圈优先模式测光
				S.t   = ESAM_Calc_S_Speed(ROM_DATA[0],ROM_DATA[10]);
			else                         //快门优先模式测光
				F.t   = ESAM_Calc_F_Stop (ROM_DATA[5],ROM_DATA[10]);
		}
		else if(key_flag[2] == 1)      //显示上一个
		{
			key_flag[2] = 0;
			if(ROM_DATA[11] > 0)
				ROM_DATA[11]--;
		}
		else if(key_flag[3] == 1)      //光圈/快门优先模式切换
		{
			key_flag[3]    = 0;
			if(ROM_DATA[13]   == 1)
				ROM_DATA[13] = 0;          //切换为光圈优先模式
			else
				ROM_DATA[13] = 1;          //切换为快门优先模式
		}
		else if(key_flag[4] == 1)      //显示下一个
		{
			key_flag[4] = 0;
			if(ROM_DATA[11] < 2)
				ROM_DATA[11]++;
			else if((ROM_DATA[11] < 4) && (ROM_DATA[12] == 1))
				ROM_DATA[11]++;
		}
		else if(key_flag[5] == 1)      //打开/关闭测光表高级参数设置
		{
			key_flag[5]    = 0;
			if(ROM_DATA[12]   == 1)
				ROM_DATA[12] = 0;          //关闭测光表高级参数设置
			else
				ROM_DATA[12] = 1;          //打开测光表高级参数设置
		}
		else
			goto L1;
	}
	else if(state == 1)              //状态1，设置光圈、快门、ISO、AE算法单次采样数、测光表校准系数
	{
		if(key_flag[0]      == 1)      //退出设置
		{
			key_flag[0] = 0;
			disp_mode   = 0;
			state       = 0;
		}
		else if(key_flag[2] == 1)      //加
		{
			key_flag[2] = 0;
			if(ROM_DATA[11]      == 0)   //修改光圈
			{
				if(ROM_DATA[0]  < 6)
					ROM_DATA[0]++;
			}
			else if(ROM_DATA[11] == 1)   //修改快门
			{
				if(ROM_DATA[5]  > 0)
					ROM_DATA[5]--;
			}
			else if(ROM_DATA[11] == 2)   //修改ISO
			{
				if(ROM_DATA[10] < 20)
					ROM_DATA[10]++;
			}
			else if(ROM_DATA[11] == 3)   //修改AE算法单次采样数
			{
				if(AE_SAMPLE_SIZE < AE_SAMPLE_SIZE_MAX)
					AE_SAMPLE_SIZE++;
			}
			else if(ROM_DATA[11] == 4)   //修改测光表校准系数
			{
				if(K.t < K_MAX)
					K.t += 0.5;
			}
		}
		else if(key_flag[4] == 1)      //减
		{
			key_flag[4] = 0;
			if(ROM_DATA[11]      == 0)   //修改光圈
			{
				if(ROM_DATA[0] > 0)
					ROM_DATA[0]--;
			}
			else if(ROM_DATA[11] == 1)   //修改快门
			{
				if(ROM_DATA[5] < 15)
					ROM_DATA[5]++;
			}
			else if(ROM_DATA[11] == 2)   //修改ISO
			{
				if(ROM_DATA[10] > 0)
					ROM_DATA[10]--;
			}
			else if(ROM_DATA[11] == 3)   //修改AE算法单次采样数
			{
				if(AE_SAMPLE_SIZE > AE_SAMPLE_SIZE_MIN)
					AE_SAMPLE_SIZE--;
			}
			else if(ROM_DATA[11] == 4)   //修改测光表校准系数
			{
				if(K.t > K_MIN)
					K.t -= 0.5;
			}
		}
		else
			goto L1;
	}
	
	
	/****************显示部分****************/
	Disp_NOR_INV(disp_mode);                                               //OLED显示模式设置
	
	//显示对应数据
	if(     (ROM_DATA[11] == 0) && (ROM_DATA[13] == 1) && (disp_mode == 0))//显示光圈，快门优先模式，  正常显示
	{
		if(F.t < 0.01)                                                       //光圈数值小于小数显示范围
		{
			OLED_Disp(0,14,14,14,11);
		}
		else if(F.t > 99.9)                                                  //光圈数值超出小数显示范围
		{
			INT_Conv(F.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //光圈数值位于小数显示范围
		{
			FLOAT_Conv(F.t,d);
		  OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 0)                                            //显示光圈，光圈优先模式 或 反转显示
	{
		if(ROM_DATA[0] < 3)                                                  //光圈，小数
		{
			FLOAT_Conv(F_Stop[ROM_DATA[0]],d);
		}
		else                                                                 //光圈，整数
		{
			INT_Conv(F_Stop[ROM_DATA[0]],d);
		}
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	}
	else if((ROM_DATA[11] == 1) && (ROM_DATA[13] == 0) && (disp_mode == 0))//显示快门，光圈优先模式，  正常显示
	{
		if(S.t < 0.0001)                                                     //快门过大(数值溢出导致数值过小)
		{
			OLED_Disp(0,14,14,14,11);
		}
		else if(S.t > 99.9)                                                  //快门超出小数显示范围
		{
			INT_Conv(S.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else if(S.t <= 1.0)                                                  //快门小于或等于1s
		{
			INT_Conv(1/S.t,d);
			OLED_Disp(1,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //快门位于小数显示范围
		{
			FLOAT_Conv(S.t,d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 1)                                            //显示快门，快门优先模式 或 反转显示
	{
		if(ROM_DATA[5] < 5)                                                  //快门为整数
		{
			INT_Conv(S_Speed[ROM_DATA[5]],d);
			OLED_Disp(0,d[0],d[1],d[2],d[3]);
		}
		else                                                                 //快门为小数
		{
			INT_Conv(1/S_Speed[ROM_DATA[5]],d);
			OLED_Disp(1,d[0],d[1],d[2],d[3]);
		}
	}
	else if( ROM_DATA[11] == 2)                                            //显示ISO
	{
		if(ROM_DATA[10] < 2)                                                 //ISO为小数
		{
			FLOAT_Conv(ISO[ROM_DATA[10]],d);
		}
		else                                                                 //ISO为整数
		{
			INT_Conv(ISO[ROM_DATA[10]],d);
		}
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	 }
	else if( ROM_DATA[11] == 3)                                            //显示AE算法单次采样数
	{
		OLED_Disp(0,14,14,14,AE_SAMPLE_SIZE);
	}
	else if( ROM_DATA[11] == 4)                                            //显示测光表校准系数
	{
		FLOAT_Conv(K.t,d);
		OLED_Disp(0,d[0],d[1],d[2],d[3]);
	}
	

	/****************数据写回部分****************/
	//写回EEPROM
	ROM_DATA[1]  = F.x[0];        //光圈计算值
	ROM_DATA[2]  = F.x[1];
	ROM_DATA[3]  = F.x[2];
	ROM_DATA[4]  = F.x[3];
	ROM_DATA[6]  = S.x[0];        //快门时间计算值
	ROM_DATA[7]  = S.x[1];
	ROM_DATA[8]  = S.x[2];
	ROM_DATA[9]  = S.x[3];
	ROM_DATA[14] = AE_SAMPLE_SIZE;//AE算法单次采样数
	ROM_DATA[15] = K.x[0];        //测光表校准系数
	ROM_DATA[16] = K.x[1];
	ROM_DATA[17] = K.x[2];
	ROM_DATA[18] = K.x[3];
	EEPROM_Write(ROM_DATA,0,0,19);
	
L1:	
	EA = 1;                       //退出临界区
}

