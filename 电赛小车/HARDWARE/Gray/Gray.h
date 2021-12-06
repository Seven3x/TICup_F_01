#ifndef __GRAY_H
#define __GRAY_H

#include "sys.h"

#define Gray1Pin GPIO_Pin_12
#define Gray2Pin GPIO_Pin_4
#define Gray3Pin GPIO_Pin_5
#define Gray4Pin GPIO_Pin_6
#define Gray5Pin GPIO_Pin_10
#define Gray6Pin GPIO_Pin_8
#define Gray7Pin GPIO_Pin_9

#define Gray_Port GPIOB
#define RCC_APB2Periph_Gray RCC_APB2Periph_GPIOB 

#define LEFT 0
#define RIGHT 1
#define STRAIGHT 2


#define Gray1 PCin(12)
#define Gray2 PBin(4)
#define Gray3 PBin(5)
#define Gray4 PBin(6)
#define Gray5 PCin(10)
#define Gray6 PBin(8)
#define Gray7 PBin(9)

void Gray_Init (void);
void GrayPID_GetState (void);
void Gray_S_Wan(void);
#endif
