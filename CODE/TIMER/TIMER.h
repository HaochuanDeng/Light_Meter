#ifndef __TIMER_H
#define __TIMER_H


#include "STC8xxxx.H"


//双击最长间隔时间(0.5s)，定时器重装值
#define DOUBLE_CLICK_DLY_TIME_H (0x5D)
#define DOUBLE_CLICK_DLY_TIME_L (0x3E)


void T2_Init(void);


#endif

