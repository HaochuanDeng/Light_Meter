#ifndef __AE_H
#define __AE_H


#include "math.h"
#include "VEML7700.h"
#include "LED.h"
#include "EEPROM.h"


//AE算法单次采样数(最大/最小值)
#define AE_SAMPLE_SIZE_MAX        (9)
#define AE_SAMPLE_SIZE_MIN        (1)

//测光表校准系数K(最大/最小值)
#define K_MAX           ((float)99.5)
#define K_MIN            ((float)0.5)

//指数平滑系数a(0~1.0)
#define EXP_SMOOTH_PARA  ((float)0.5)


//初始化函数
void  AE_Init          (void);

//曝光计算函数
float ESAM_Calc_S_Speed(u8 f,u8 iso);
float ESAM_Calc_F_Stop (u8 s,u8 iso);


#endif

