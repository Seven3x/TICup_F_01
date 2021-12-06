#include "Wheels.h"
#include "PID.h"
#include "Move.h"

extern u16 LSpeed, RSpeed; 
extern PID_t PID_Structure;






void L298N_Init (u16 arr, u16 psc) {
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);

    //初始化gpio
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = L1_Pin ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init (GPIOC, &GPIO_InitStruct);//控制轮子正反转的四个引脚  推挽输出 10MHz

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin =  R1_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init (GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init (GPIOA, &GPIO_InitStruct);
    //PWM输出引脚 推挽复用输出
    //初始化gpio完毕

    //初始化定时器
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//设置为1分频因子
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;//  向上计数模式
    TIM_InitStruct.TIM_Period = arr;
    TIM_InitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit (TIM5, &TIM_InitStruct);
    //TIM2&TIM5 初始化

    //初始化pwm输出
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//开启引脚输出
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性高
    TIM_OCInitStruct.TIM_Pulse = 0; //初始化占空比
    TIM_OC1Init (TIM5, &TIM_OCInitStruct);
    TIM_OC2Init (TIM5, &TIM_OCInitStruct);
    //TIM2&TIM5 OC2 pwm初始化

    TIM_Cmd (TIM5, ENABLE);
    //TIM2&TIM5使能
    
}


void L_PWM (u16 x) {//设置占空比
    if(x > ARR) x = ARR;
	TIM_SetCompare1 (TIM5, x);
}

void R_PWM (u16 x) {//设置占空比
if (x > ARR) x = ARR;
    TIM_SetCompare2 (TIM5, x);
}

void SetSpeed(u16 Speed) {
    if (Speed > 900) {
        Speed = 900;
    }
    LSpeed = RSpeed = Speed;
}

void R_Fore (int rccr)
{
    if (rccr > 900) {
        rccr = 900;
    }
    if (rccr < 0){
        rccr = - rccr;
        R_Back(rccr);
    }
    if (rccr < -900){
        R_Back(900);
    }
    R1Out = 0;
    R_PWM(rccr);
}
void L_Back (u16 lccr)
{
    L1Out = 1;
    L_PWM(ARR - lccr);
}
void R_Stop (void)
{
    R1Out = 0;
    R_PWM(0);
}
void L_Stop (void)
{
    L1Out = 0;
    L_PWM(0);
}
void R_Back (u16 rccr)
{
    R1Out = 1;
    R_PWM(ARR - rccr);
}
void L_Fore (int lccr)
{
    if (lccr > 900) {
        lccr = 900;
    }
    if (lccr < 0){
        lccr = - lccr;
        L_Back(lccr);
    }
    if (lccr < -900){
        L_Back(900);
    }
    L1Out = 0;
    L_PWM(lccr);
}

