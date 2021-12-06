#ifndef __MOVE_H
#define __MOVE_H

#include "sys.h"
#include "Wheels.h"
#include "BlueTeeth.h"
#include "Duoji.h"
#include "delay.h"
#include "TIM6.h"
#include "Gray.h"
#include "PID.h"
#include "TIM7.h"
#include "Beep.h"


#define LED1 PAout(8)
#define LED0 PDout(2)


#define SpeedFast 530
#define SpeedMiddle 340
#define SpeedSlow 340
#define SpeedSWan 350//SWan速度，有时候驱动不了，看情况，不行了用Slow
#define SpeedMoreSlow 270



#define KEY0 PCin(13)
#define KEY1 PCin(1)

#define GD1 PCin(4)
#define GD2 PCin(15)

void LED_Init(void);
void KEY_Init(void);
void GD_Init(void);
void Get_All_Inited(void);
void SuiDao(u8 type);
void HuanDao(u8 type);
void fuckThePingPongs(void);
void findNums(void);

#endif

