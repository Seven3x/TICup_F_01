#include "BlueTeeth.h"
#include "Wheels.h"
#include "TIM7.h"
#include "Move.h"

extern u8 data;
extern u8 state;
uint16_t u1data;
extern char btflag;
u16 countNums[8] = {0};
u8 map[6] = {0};
u8 flags[8] = {0};
u8 nums = 0;
u8 u1flag;
u8 detect;
extern u8 found90;
extern u8 imgstate;
uint16_t u2data;
u8 r,l;
u8 rebuf[4] = {'A', 8, 8, 'B'};
u8 u3flag;

void BlueTooth_Init(void){
     GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//TX PA9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//RX PA10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//State引脚 PB1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    USART_ClearITPendingBit(USART1,USART_IT_RXNE );
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    

    USART_Cmd(USART1, ENABLE);
}



void SendMsg(u16 msg){
    while(1)
    {
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    
        USART_SendData(USART1, msg);
        Delay_10ms(5);
        u1flag = 0;
        if (u1data == 10){
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  
            return;
        }
    }
    




 }


void USART1_IRQHandler(){
    if(USART_GetITStatus(USART1,USART_IT_RXNE)) {
        u1data = USART_ReceiveData(USART1);
        u1flag = 1;
    }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE );
}







void SendMap(){
    int i = 0;
    while(1){
        USART_Cmd(USART1, ENABLE);
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        USART_SendData(USART1, 'A');
        for (i = 0; i < 6; i ++){        
            USART_SendData(USART1, map[i]);
        }
        
        USART_SendData(USART1, 'B');
        Delay_10ms(4);
        if(u1data == 9) break;
    }
}








void BlueTeeth_Init (void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//TX PB10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//RX PB11
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    

    USART_Cmd(USART3, ENABLE);
}





void OpenMV_Init (void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//TX PA2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//RX PA3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 19200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);    

    USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(){
    if(USART_GetITStatus(USART2,USART_IT_RXNE)) {
        LED0 = LED1 = 0;
        u2data = USART_ReceiveData(USART2);
        if (u2data == 'S')
            found90 = 1;
    }
    USART_ClearITPendingBit(USART2,USART_IT_RXNE );
}






// void USART3_IRQHandler() { 
//     if(USART_GetITStatus(USART3,USART_IT_RXNE)) {
//         data = USART_ReceiveData(USART3);
//         if (data == '0'){
//             countNums[0]++;
//         }
//         if (data == '1') {
//             countNums[1] ++;
//         }
//         if (data == '2') {
//             countNums[2] ++;
//         }
//         if (data == '3') {
//              countNums[3] ++;
//         }
//         if (data == '4') {
//             countNums[4] ++;
//         }
//         if (data == '5') {
//             countNums[5] ++;
//         }
//         if (data == '6') {
//             countNums[6] ++;
//         }
//         if (data == '7') {
//             countNums[7] ++;
//         }
//         if (data == 'F'){
//             found90 = 1;
//         }
        



//     }
//     USART_ClearITPendingBit(USART3,USART_IT_RXNE );
// }



void USART3_IRQHandler() { 
    static u8 k;
    rebuf[k++] =USART_ReceiveData(USART3);	//读取接收到的数据
        if(!(rebuf[0]=='A'))//如果帧头错误，清缓存
        {
            // LED1 = 1;
            //USART_SendData(USART1,'N');
            k=0;
            rebuf[0]=0;
            r = l = 8;
            
        }
        if( k >3) k = 0;
        if(k==3)//数据接收完毕
        {
            if(rebuf[3]=='B')//判断帧尾，正确则解析姿态角
            {
                
                r = rebuf[1]=='8'?r:(rebuf[1] - '0');
                l = rebuf[2]=='8'?r:(rebuf[2] - '0');
                u3flag = 1;
            }
            
            k=0;//清缓存
        }
            
    USART_ClearITPendingBit(USART3,USART_IT_RXNE );
}


void waitForNums(){
    int i;
    for(i = 0; i < 8; i ++){
            flags[i] = 0;
            countNums[i] = 0;
    }
    nums = 0;
    USART_Cmd(USART3, ENABLE);
    USART_SendData(USART3, 'A');//发送给树莓派，开始分辨

    Delay_10ms(200);

    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);    
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_SendData(USART3, 'B');
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_Cmd(USART3, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
    for(i = 0; i < 8; i ++){
        if(countNums[i] >= 4){
            flags[i] = 1;
            nums ++;
        }
    }


}

void waitForLeftNums(){
    int i;
    for(i = 0; i < 8; i ++){
            flags[i] = 0;
            countNums[i] = 0;
    }
    nums = 0;
    USART_Cmd(USART3, ENABLE);
    USART_SendData(USART3, 'Y');//发送给树莓派，开始分辨

    Delay_10ms(200);

    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);    
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_SendData(USART3, 'B');
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_Cmd(USART3, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
    for(i = 0; i < 8; i ++){
        if(countNums[i] >= 3){
            flags[i] = 1;
            nums ++;
        }
    }


}

void waitForRightNums(){
    int i;
    for(i = 0; i < 8; i ++){
            flags[i] = 0;
            countNums[i] = 0;
    }
    nums = 0;
    USART_Cmd(USART3, ENABLE);
    USART_SendData(USART3, 'Z');//发送给树莓派，开始分辨

    Delay_10ms(200);

    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);    
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_SendData(USART3, 'B');
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    USART_Cmd(USART3, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
    for(i = 0; i < 8; i ++){
        if(countNums[i] >= 3){
            flags[i] = 1;
            nums ++;
        }
    }


}


