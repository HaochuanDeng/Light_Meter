#ifndef __TIMER_H
#define __TIMER_H


#include "STC8xxxx.H"


//˫������ʱ��(0.5s)����ʱ����װֵ
#define DOUBLE_CLICK_DLY_TIME_H (0x5D)
#define DOUBLE_CLICK_DLY_TIME_L (0x3E)


void T2_Init(void);


#endif

