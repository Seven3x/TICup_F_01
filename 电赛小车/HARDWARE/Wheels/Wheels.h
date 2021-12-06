#ifndef __L298N_H
#define __L298N_H


#include "sys.h"

#define L298N_Port GPIOA

#define L1_Pin GPIO_Pin_0    //PB10
#define R1_Pin GPIO_Pin_1   //PB11


#define PSC         72
#define ARR         999   //(ARR+1) * (PSC + 1) = 72000
#define CCR_NORMAL  500
#define CCR_SLOW    400
#define CCR_FAST    300

#define R1Out   PCout(0)
#define L1Out   PCout(1)


void SetSpeed(u16 Speed);
void R_Fore (int rccr);
void L_Back (u16 lccr);
void R_Stop (void);
void L_Stop (void);
void R_Back (u16 rccr);
void L_Fore (int lccr);

void R_PWM (u16 x);//用来修改右轮pwm输出时的占空�?
void L_PWM (u16 x);//用来修改左轮pwm输出时的占空�?

void L298N_Init (u16 arr, u16 psc);//L298N的初始化

#endif
