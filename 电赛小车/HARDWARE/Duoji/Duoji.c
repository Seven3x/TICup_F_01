#include "Duoji.h"


void DuoJi_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIm_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO ,ENABLE );
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIO_InitStructure .GPIO_Pin=GPIO_Pin_3 ;
	GPIO_InitStructure .GPIO_Speed=GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA ,&GPIO_InitStructure );
	
	TIm_TimeBaseStructure .TIM_Period =arr;
	TIm_TimeBaseStructure.TIM_Prescaler =psc;
	TIm_TimeBaseStructure.TIM_ClockDivision =0;
	TIm_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up ;
	TIM_TimeBaseInit (TIM5 ,&TIm_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1 ;
	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable ;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ;
	TIM_OC4Init (TIM5 ,&TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig (TIM5,TIM_OCPreload_Enable );
	
	TIM_Cmd (TIM5 ,ENABLE );
}
void DuoJi_Angle(u16 i)
{
	TIM_SetCompare4 (TIM5 ,i);
}
