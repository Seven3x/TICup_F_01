#ifndef __DUOJI_H
#define __DUOJI_H	

#include "sys.h"

#define R90 249
#define R45 499
#define L90 1249
#define L45 999
#define Mid 799

void DuoJi_PWM_Init(u16 arr,u16 psc);
void DuoJi_Angle(u16 i);

#endif
