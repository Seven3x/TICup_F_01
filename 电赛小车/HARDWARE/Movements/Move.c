#include "Move.h"

extern u8 flags[8];
extern u16 countNums[8];
extern u8 nums;
extern u8 state;
u8 barrier = STRAIGHT;
extern PID_t PID_Structure;
extern u8 imgstate;
extern u8 detected;
extern u8 firstnum;
extern u8 turn_flag;
extern u8 target;

void LED_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;  
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init (GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init (GPIOD, &GPIO_InitStruct);  
}

void KEY_Init () {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_1;
    GPIO_Init (GPIOC, &GPIO_InitStruct);    
}

void GD_Init () {//GD的init，但是只写了正前方那个
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}



/**
*中断优先级分组2
*LED、GD、Gray、TIM6(6,7199)、PID、BlueTeeth
*L298N TIM7(delay) Beep初始化
*设置速度为SpeedMiddle
*模式为stop(4)
*使能TIM6
*LED1,LED2不亮
*延时500ms
**/
void Get_All_Inited(void) {

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

    OpenMV_Init();
    LED_Init(); 
    GD_Init();   
    Gray_Init();
    TIM6_Init (99,7199); //10ms一次定时器中中断  还未使能TIM6
    PID_Init(&PID_Structure, 120, 0, 300, 0);//PID参数设置 locpid 170 0 200;
    BlueTeeth_Init();
    L298N_Init(ARR, PSC);
    BlueTooth_Init();
    // DuoJi_PWM_Init(9999,143);
    TIM7_Init();
    Beep_Init();
    SetSpeed(SpeedMiddle);//初始化速度为Middle
    state = 4;//1蓝牙模式2循迹模式
    // Delay_10ms(50);
    TIM_Cmd(TIM6, ENABLE);//使能TIM6
    // LED1= LED0 =0; 
    // LED0 = 0;
    // DuoJi_Angle(Mid)
}


void findNums()
{
        {
        int i;
        for(i = 0; i < 8; i ++){
                flags[i] = 0;
                countNums[i] = 0;
        }
        nums = 0;
        while(1){
            USART_Cmd(USART3, ENABLE);
            imgstate = 2;
            USART_SendData(USART3, 'Y');//发送给树莓派，开始左分辨

            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
            
            for(i = 0; i < 8; i ++){
                if(countNums[i] >= 5){
                    detected = firstnum = i;
                    goto after_centerleft;
                }
            }
        }
    }
    
    after_centerleft:

    if(detected == target) {turn_flag = 2;imgstate = 0;}//如果左边就是了，直接进行了
    else
    {
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
        USART_SendData(USART3, 'Z');
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
        USART_SendData(USART3, 'Z');
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
        USART_SendData(USART3, 'Z');
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
        USART_SendData(USART3, 'Z');  
        imgstate = 3;

        {
            int i;
            for(i = 0; i < 8; i ++){
                    flags[i] = 0;
                    countNums[i] = 0;
            }
            nums = 0;
            while(1){
                USART_Cmd(USART3, ENABLE);
                USART_SendData(USART3, 'Z');//发送给树莓派，开始右分辨
                imgstate = 3;
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
                
                for(i = 0; i < 8; i ++){
                    if (i == firstnum)
                        continue;
                    if(countNums[i] >= 5){
                        detected = i;
                        goto after_centerright;
                    }
                }
            }
        }
        after_centerright:

        if(detected == target) {turn_flag = 3;imgstate = 0;}//如果右边就是了，直接进行了
        else{
            turn_flag = 1;
            imgstate = 0;
        }
    }

}

