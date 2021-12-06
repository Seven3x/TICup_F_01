#include "Gray.h"
#include "PID.h"
#include "Wheels.h"
#include "math.h"

u16 R_SpeedError, L_SpeedError;
extern int PIDSpeed;
extern u16 RSpeed, LSpeed;
u32 LGrayState, RGrayState;
u16 LGray, RGray;
u32 GrayState;
u8 lastDirection = STRAIGHT;
extern PID_t PID_Structure;


void Gray_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_Gray, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = Gray1Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray2Pin;
    GPIO_Init(Gray_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray3Pin;
    GPIO_Init(Gray_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray4Pin;
    GPIO_Init(Gray_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray5Pin;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray6Pin;
    GPIO_Init(Gray_Port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = Gray7Pin;
    GPIO_Init(Gray_Port, &GPIO_InitStruct);
    
}

void Gray_S_Wan(void) {//S弯的
    GrayState = 0;
    PIDSpeed = 0;

    if(Gray2) GrayState += 0x0100000;
    if(Gray3) GrayState += 0x0010000;
    if(Gray4) GrayState += 0x0001000;
    if(Gray5) GrayState += 0x0000100;
    if(Gray6) GrayState += 0x0000010;
    
    switch (GrayState) {
        case 0x0100000:
            L_Back(5);
            R_Fore(500);
            lastDirection = LEFT;
            break;
        case 0x0110000:
            L_Back(5);
            R_Fore(500);
            lastDirection = LEFT;
            break;
        case 0x0111000:
            L_Back(5);
            R_Fore(500);
            lastDirection = LEFT;
            break;
        case 0x0011000:
            L_Back(5);
            R_Fore(500);
            lastDirection = LEFT;
            break;
        case 0x0001000:
            L_Fore(440);
            R_Fore(440);
        lastDirection = STRAIGHT;
            break;
        case 0x0011100:
            L_Fore(440);
            R_Fore(440);            
            lastDirection = STRAIGHT;
            break;
        case 0x0001100:
            L_Fore(500);
            R_Back(5);
            lastDirection = RIGHT;
            break;
        case 0x0001110:
            L_Fore(500);
            R_Back(5);
            lastDirection = RIGHT;
            break;
        case 0x0000110:
            L_Fore(500);
            R_Back(5);
            lastDirection = RIGHT;
            break;
        case 0x0000010:
            L_Fore(500);
            R_Back(5);
            lastDirection = RIGHT;
            break;
            break;
        default:
            if(lastDirection == RIGHT) {
                L_Fore(500);
                R_Back(500);
            }
            if(lastDirection == LEFT) {
                R_Fore(500);
                L_Back(500);
            }
            if(lastDirection == STRAIGHT) {
                R_Fore(450);
                L_Fore(450);
            }
    }
    
}



void GrayPID_GetState () {//PID的
    RGrayState = 0;
    LGrayState = 0; 
    
	RGrayState = Gray5 * 1 + Gray6 * 2 + Gray7 * 4;
	LGrayState = Gray3 * 1 + Gray2 * 2 + Gray1 * 4;
	
	LGray = RGray = 0;
	
	switch(LGrayState) {
        case 0:LGray = 0;break; //000
		case 1:LGray = 1;break; //001
        case 2:LGray = 1;break; //010
		case 3:LGray = 2;break; //011
		case 4:LGray = 2;break; //100
        case 5:LGray = 3;break; //101
		case 6:LGray = 2;break; //110
		case 7:LGray = 2;break; //111
	}
	
	switch(RGrayState) {
        case 0:RGray = 0;break; //000
		case 1:RGray = 1;break; //001
        case 2:RGray = 1;break; //010   
		case 3:RGray = 2;break; //011
		case 4:RGray = 2;break; //100
        case 5:RGray = 3;break; //101
		case 6:RGray = 2;break; //110
		case 7:RGray = 2;break; //111
	}
	
    PIDSpeed = LGray - RGray;
    PIDSpeed = - LocPID_Calc (PIDSpeed, &PID_Structure);//得到左右轮差速PIDSpeed，修改左右轮速度即可
}
