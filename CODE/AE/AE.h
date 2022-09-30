#ifndef __AE_H
#define __AE_H


#include "math.h"
#include "VEML7700.h"
#include "LED.h"
#include "EEPROM.h"


//AE�㷨���β�����(���/��Сֵ)
#define AE_SAMPLE_SIZE_MAX        (9)
#define AE_SAMPLE_SIZE_MIN        (1)

//����У׼ϵ��K(���/��Сֵ)
#define K_MAX           ((float)99.5)
#define K_MIN            ((float)0.5)

//ָ��ƽ��ϵ��a(0~1.0)
#define EXP_SMOOTH_PARA  ((float)0.5)


//��ʼ������
void  AE_Init          (void);

//�ع���㺯��
float ESAM_Calc_S_Speed(u8 f,u8 iso);
float ESAM_Calc_F_Stop (u8 s,u8 iso);


#endif

