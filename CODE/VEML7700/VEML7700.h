#ifndef __VEML7700_H
#define __VEML7700_H


#include "IIC.h"
#include "DELAY.h"


//VEML7700 IIC 地址定义
#define VEML7700_ADDR_W 0x20
#define VEML7700_ADDR_R 0x21

//VEML7700寄存器地址定义
#define ALS_CONF_0      0x00
#define ALS_WH          0x01
#define ALS_WL          0x02
#define POWER_SAVING    0x03
#define ALS             0x04
#define WHITE           0x05
#define ALS_INT         0x06

//ALS_Data非线性校准方程系数
#define C1              ((float)(0.00000000000060135))
#define C2              ((float)(0.0000000093924))
#define C3              ((float)(0.000081488))
#define C4              ((float)(1.0023))


//VEML7700 ALS增益(低2位有效)
//ALS_gain_1   = ALS gain x 1
//ALS_gain_2   = ALS gain x 2
//ALS_gain_1_8 = ALS gain x (1/8)
//ALS_gain_1_4 = ALS gain x (1/4)
typedef enum
{
	ALS_gain_1   = (u16)0x0000,
	ALS_gain_2   = (u16)0x0001,
	ALS_gain_1_8 = (u16)0x0002,
	ALS_gain_1_4 = (u16)0x0003
}VEML7700_ALS_GAIN_TypeDef;

//VEML7700 ALS积分时间(低4位有效)
//ms25  = 25ms
//ms50  = 50ms
//ms100 = 100ms
//ms200 = 200ms
//ms400 = 400ms
//ms800 = 800ms
typedef enum
{
	ms25  = (u16)0x000C,
	ms50  = (u16)0x0008,
	ms100 = (u16)0x0000,
	ms200 = (u16)0x0001,
	ms400 = (u16)0x0002,
	ms800 = (u16)0x0003
}VEML7700_ALS_IT_TypeDef;
/*
//VEML7700 PERS乘积系数(低2位有效)
//p1 = 1
//p2 = 2
//p4 = 4
//p8 = 8
typedef enum
{
	p1 = (u16)0x0000,
	p2 = (u16)0x0001,
	p4 = (u16)0x0002,
	p8 = (u16)0x0003
}VEML7700_ALS_PERS_TypeDef;
*/
//VEML7700 中断控制(低1位有效)
//EN  = ALS INT enable
//DIS = ALS INT disable
typedef enum
{
	EN  = (u16)0x0001,
	DIS = (u16)0x0000
}VEML7700_ALS_INT_EN_TypeDef;

//VEML7700 掉电控制(低1位有效)
//ON  = ALS power on
//OFF = ALS shut down
typedef enum
{
	ON  = (u16)0x0000,
	OFF = (u16)0x0001
}VEML7700_ALS_SD_TypeDef;
/*
//VEML7700 省电模式(低2位有效)
//m1 = mode 1
//m2 = mode 2
//m3 = mode 3
//m4 = mode 4
typedef enum
{
	m1 = (u16)0x0000,
	m2 = (u16)0x0001,
	m3 = (u16)0x0002,
	m4 = (u16)0x0003
}VEML7700_PSM_TypeDef;
*/
//VEML7700 省电控制(低1位有效)
//enable  = enable
//disable = disable
typedef enum
{
	enable  = (u16)0x0001,
	disable = (u16)0x0000
}VEML7700_PSM_EN_TypeDef;


//寄存器操作函数
void    ALS_GAIN       (VEML7700_ALS_GAIN_TypeDef   gain);
void    ALS_IT         (VEML7700_ALS_IT_TypeDef     it);
//void    ALS_PERS       (VEML7700_ALS_PERS_TypeDef   pers);
void    ALS_INT_EN     (VEML7700_ALS_INT_EN_TypeDef en);
void    ALS_SD         (VEML7700_ALS_SD_TypeDef     power);
//void    ALS_H_THRE     (u16                         H_Thre);
//void    ALS_L_THRE     (u16                         L_Thre);
//void    PSM            (VEML7700_PSM_TypeDef        mode);
void    PSM_EN         (VEML7700_PSM_EN_TypeDef     en);
u16     ALS_DATA_Read  (void);
//u16     WHITE_DATA_Read(void);
//u8      INT_L_Read     (void);
//u8      INT_H_Read     (void);

//应用层函数
void    VEML7700_Init  (void);
float   Lux_Calc       (void);


#endif

