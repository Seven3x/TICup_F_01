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
#include "Move.h"


extern u8 map[6];
u8 single;//表示是否是单人任务
u8 detected = 8;
u8 firstnum = 10;
u8 target = 8;
extern u8 u3flag;
extern u8 rebuf[4];

extern u8 r,l;
u8 imgstate = 0;
u8 dr = 8, dl = 8;

char btflag = '0';
extern u16 countNums[8];
u8 state = 0;
u8 gray_state = 0;
u16 RSpeed, LSpeed;
int SpeedError;
u8 data;
int PIDSpeed;
extern u16 R_SpeedError, L_SpeedError;
PID_t PID_Structure;
extern u8 flags[8];
extern u8 nums;
u8 turn_flag;
extern u8 u1flag;
extern uint16_t u1data;
u8 found90;

void TIM6_IRQHandler(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update)) {
        switch (state) {
            case 1 :    //蓝牙控制模式
                switch(data) {
                    case 'c':
                        R_Fore(SpeedSlow);
                        L_Fore(SpeedSlow);
                        break;
                    case 'd':
                        R_Fore(SpeedMiddle);
                        L_Back(SpeedMiddle);
                        break;
                    case 'e':
                        R_Back(SpeedMiddle);
                        L_Fore(SpeedMiddle);
                        break;
                    case 'f':
                        R_Stop();
                        L_Stop();
                        break;
                    case 'g':
                        R_Back(SpeedSlow);
                        L_Back(SpeedSlow);
                        break;
                    case 'w':
                        state = 2;
                        break;
                }
                break;
            case 2 :    //循迹模式
                 GrayPID_GetState(); //获取灰度值并赋给调速值PIDSpeed(如果用之前我用的方法则改成直接调用Gray_GetState函数即可，删掉后面的两个函数)
                 R_Fore(RSpeed + PIDSpeed);
                 L_Fore(LSpeed - PIDSpeed);
                  break;
            case 3 :    //不循迹，直走
                R_Fore(RSpeed);
                L_Fore(LSpeed+5);
                break;
            case 4 :    //stop
                R_Stop();
                L_Stop();
                break;
            case 5:     //右转
                R_Fore(RSpeed);
                L_Back(LSpeed);
                break;
            case 6:     //左转
                R_Back(RSpeed);
                L_Fore(LSpeed);
                break;
            case 7://
                break;
            case 8://S弯的循迹
                Gray_S_Wan();
                break;
            case 9://PID循迹
                GrayPID_GetState();
            R_Fore(RSpeed + PIDSpeed);
                 L_Fore(LSpeed - PIDSpeed);
                    break;
            case 10 :    //后撤步
                R_Back(RSpeed);
                L_Back(LSpeed);
                break;
            case 11: //其他的左转
                R_Back(RSpeed);
                L_Fore(LSpeed+10);
        }

    }
    if(imgstate == 1){
            USART_SendData(USART3, 'A');
            
        }
        if(imgstate == 0){USART_SendData(USART3, 'B');
            
        }
        if(imgstate == 2){USART_SendData(USART3, 'Y');
            
        }
        if(imgstate == 3){USART_SendData(USART3, 'Z');
        }
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}
 

void Dbg(u32 time) {//停止一time个10ms并以原速前进
    u16 temp = state;
    state = 4;
    Delay_10ms(time);
    state = temp;
}

#define DEBUG 0   //DEBUG开关，如果为1那么执行的是第一个main，否则是第二个main

#if DEBUG

int main () {
    u16 count = 0;
    Get_All_Inited();
    SetSpeed(SpeedSlow);
    state =4;
    while(GD1 == 1);
    state = 3;
    while(1);


}

#else



int main () {
    int i = 0;
    u16 count = 0;

    Get_All_Inited();
    SetSpeed(SpeedFast);
    LED0 = LED1 = 0;
    Beep = 0;
    
    USART_SendData(USART3, 'Y');//发送给树莓派，开始左分辨
    imgstate = 2;
    state = 4;
    // while(1){
    //     if (Gray7 == 0)
    //         xPAout(8) = 0;
    //     else
    //         PAout(8) = 1;g 
    //     if (Gray6 == 0)
    //         PDout(2) = 0;
    //     else 
    //         PDout(2) = 1;
    // }
    
    // goto ceshi_back;
    // goto ceshi1;
    // goto ceshi2;
    // goto ceshi3;
    // goto ceshi4;
    // goto ceshi5;
    // goto ceshi6;
    // goto ceshi7;
    // goto ceshi8;


    //通用读取数字
    // {
    //     int i;
    //     LED0 = LED1 = 0;
    //     for(i = 0; i < 8; i ++){
    //             flags[i] = 0;
    //             countNums[i] = 0;
    //     }
    //     nums = 0;
    //     while(1){
    //         USART_Cmd(USART3, ENABLE);
    //         USART_SendData(USART3, 'A');//发送给树莓派，开始分辨
    //         imgstate = 1;
    //         USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
            
    //         for(i = 0; i < 8; i ++){
    //             if(countNums[i] >= 5){
    //                 target = i;
    //                 goto after_num;
    //             }
    //         }
    //     }
    // }
    
    // after_num:

    // USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);    
    // while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    // USART_SendData(USART3, 'B');
    // while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    // USART_SendData(USART3, 'B');
    // while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    // USART_SendData(USART3, 'B');
    // while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    // USART_SendData(USART3, 'B');
    // USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
    // imgstate = 0;
    USART_Cmd(USART3, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
    Beep = 1;
    r = l = 8;
    target = 8;
    while(target == 8){
        if(r != 8){target = r;}
        if(l != 8){target = l;}
    }
    Beep = 0;


    LED0 = 1;//红灯亮

    while(GD1 == 1);
    LED0 = 0;
    Delay_10ms(90);
    SetSpeed(SpeedMiddle);

    if(!BtConnected){
        single = 1;
    }



    if(0)//用来测试0 ,1------------------
    {
        ceshi2:
        target = 1;
        SetSpeed(SpeedMiddle);
    }
    if(0)
    {
        ceshi1:
        target = 0;
        SetSpeed(SpeedMiddle);
    }



    state = 2;
    Delay_10ms(20);    
    
    while(Gray1 == 0 || Gray7 == 0);
    Delay_10ms(15);
    
    if (target == 0)
    {
        state = 6;
        SetSpeed(SpeedMiddle);
        Delay_10ms(50);
        while(Gray3 == 0);
        state = 5;
        Delay_10ms(2);
        SetSpeed(SpeedSlow);

        state = 2;
        Delay_10ms(160);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;
        LED0 = 1;//亮红灯
        while(GD1 == 0);
        LED0 = 0;
        Delay_10ms(80);


        SetSpeed(SpeedSlow);
        Delay_10ms(50);
        state = 6;
        Delay_10ms(40);
        SetSpeed(SpeedMiddle);
        while(Gray3 == 0);//180°转向                开始回家
        state = 4;

        state = 2;
        SetSpeed(SpeedSlow);
        Delay_10ms(20);
        
        while(Gray7 == 0);//到达十字路口
        Delay_10ms(30);
        state = 10;
        Delay_10ms(6);
        // Dbg(100);
        state = 5;
        SetSpeed(SpeedSlow);
        Delay_10ms(36);
        state = 6;
        Delay_10ms(6);
        SetSpeed(SpeedSlow);


        state = 2;  
        Delay_10ms(70);
        SetSpeed(SpeedMiddle);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        
        state = 4;
        LED1 = 1;
        while(1);
    }
    else if(target == 1)
    {
        state = 5;
        SetSpeed(SpeedMiddle);
        Delay_10ms(30);
        while(Gray4 == 0);
        state = 6;
        Delay_10ms(2);
        SetSpeed(SpeedSlow);

        state = 2;
        Delay_10ms(160);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;
        LED0 = 1;
        while(GD1 == 0);
        LED0 = 0;
        Delay_10ms(90);

        SetSpeed(SpeedSlow);
        Delay_10ms(50);
        state = 6;
        Delay_10ms(40);
        while(Gray4 == 0);//180°转向                开始回家
        state = 5;
        Delay_10ms(5);
        state = 4;

        state = 2;
        SetSpeed(SpeedSlow);
        Delay_10ms(100);
        
        while(Gray1 == 0);//到达十字路口
        Delay_10ms(30);
        state = 10;
        Delay_10ms(6);
        // Dbg(100);
        state = 6;
        SetSpeed(SpeedSlow);
        Delay_10ms(42);
        state = 5;
        Delay_10ms(6);
        SetSpeed(SpeedSlow);


        state = 2;  
        Delay_10ms(70);
        SetSpeed(SpeedMiddle);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        
        state = 4;
        LED1 = 1;
        while(1);
    }











//--------------------------------------中间病房-------------------------------------------------





    else{//如果不在1、2，直走

        state = 2;
        SetSpeed(SpeedMiddle);
    }
    Delay_10ms(100);
    found90 = 0;
    while(found90 == 0);
    found90 = 0;
    Delay_10ms(14);
    state = 4;

    // //识别数字
    // nums = 0;
    // while(nums  == 0)
    // {
    //     waitForNums();
    //     Delay_10ms(10);
    // }
    // for (i = 0; i < 8; i ++){
    //     if (flags[i] == 1){
    //         if (i == target){
    //             turn_flag = 1;
    //         }
    //     }
    // }
    // if(turn_flag == 0)
    //     goto after_middle;
    // if(turn_flag == 1)
    // {
    //     int j = 0;
    //     nums = 0;
    //     while(nums == 0)
    //     {
    //         waitForLeftNums();
    //         Delay_10ms(40);
    //     }
    //     for (j = 0; j < 8; j ++)
    //     {
    //         if (flags[j] == 1)
    //         {
    //             if (j == target)
    //                 turn_flag = 3;//检测到就是3
    //             else
    //                 turn_flag = 2;//检测不到就是2
    //         }
    //     }
    // }



//写的新识别方法-------------------------------------------------------------------------------------------------------
    // {
    //     int i;
    //     for(i = 0; i < 8; i ++){
    //             flags[i] = 0;
    //             countNums[i] = 0;
    //     }
    //     nums = 0;
    //     while(1){
    //         USART_Cmd(USART3, ENABLE);
    //         imgstate = 2;
    //         USART_SendData(USART3, 'Y');//发送给树莓派，开始左分辨

    //         USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
            
    //         for(i = 0; i < 8; i ++){
    //             if(countNums[i] >= 5){
    //                 detected = firstnum = i;
    //                 goto after_centerleft;
    //             }
    //         }
    //     }
    // }
    
    // after_centerleft:

    // if(detected == target) {turn_flag = 2;imgstate = 0;}//如果左边就是了，直接进行了
    // else
    // {
    //     USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
    //     while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    //     USART_SendData(USART3, 'Z');
    //     while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    //     USART_SendData(USART3, 'Z');
    //     while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    //     USART_SendData(USART3, 'Z');
    //     while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);
    //     USART_SendData(USART3, 'Z');  
    //     imgstate = 3;
    //     {
    //         int i;
    //         for(i = 0; i < 8; i ++){
    //                 flags[i] = 0;
    //                 countNums[i] = 0;
    //         }
    //         nums = 0;
    //         while(1){
    //             USART_Cmd(USART3, ENABLE);
    //             USART_SendData(USART3, 'Z');//发送给树莓派，开始右分辨
    //             imgstate = 3;
    //             USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 
                
    //             for(i = 0; i < 8; i ++){
    //                 if (i == firstnum)
    //                     continue;
    //                 if(countNums[i] >= 5){
    //                     detected = i;
    //                     goto after_centerright;
    //                 }
    //             }
    //         }
    //     }
    //     after_centerright:

    //     if(detected == target) {turn_flag = 3;imgstate = 0;}//如果右边就是了，直接进行了
    //     else{
    //         turn_flag = 1;
    //         imgstate = 0;
    //     }
    // }

    
//------------------新检测结束！！！！！------------------------------------------------------------

//==============================新的检测v2.0=============================================================
    u3flag = 0;
    r = l = 8;
    Beep = 1;
    count = 1;
    while(1){
        if(r == 8 && l == 8) ;
        else{
            if (r == target) 
            {
                turn_flag = 3;
                break;
            }
            else if (l == target) 
            {
                turn_flag = 2;
                break;
            }
            if(r < 8 && r >= 0){
                dr = r;
                // map[5] = r;
            }
            if(l < 8 && l >= 0){
                dl = l;
                // map[0] = l;
            }
            if(dr < 8 && dr >= 0 && dl < 8 && dl >= 0){
                turn_flag = 0;
                // map[5] = r;
                // map[0] = l;
                break;
            }
        }
        // count++;
        // if(count == 70000){
        //     count = 0;
        //     state = 2;
        //     Delay_10ms(100);
        //     state = 4;
        // }
    }
    count = 0;
    Beep = 0;
//==============================新的检测v2.0结束了捏=============================================================

    if(0)
    {
        ceshi3:
        turn_flag = 3;
        single = 1;
        SetSpeed(SpeedMiddle);
    }
    if(0)
    {
        ceshi4:
        turn_flag = 2;
        single = 1;
        SetSpeed(SpeedMiddle);
    }

    state = 3;
    Delay_10ms(55);
   // Dbg(100);



    if(turn_flag == 2){//         去左侧中部病房！！！！！！！！！！
        state = 6;
        SetSpeed(SpeedMiddle);
        Delay_10ms(40);
        while(Gray3 == 0);
        state = 5;
        Delay_10ms(2);
        SetSpeed(SpeedSlow);

        state = 2;
        Delay_10ms(170);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;

        LED0 = 1;
        if(single == 0)
        {
            SendMsg('L');

            Delay_10ms(20);
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            while(1)
            {
                if(u1flag)
                {
                    u1flag = 0;
                    if(u1data == 20);//20代表可以走
                        break;
                }
            }
        }

        while(GD1 == 0);
        LED0 = 0;

        SetSpeed(SpeedSlow);
        Delay_10ms(50);
        state = 5;
        Delay_10ms(60);
        while(Gray3 == 0);//180°转向                开始回家
        state = 6;
        Delay_10ms(5);
        state = 2;
        SetSpeed(SpeedSlow);
        Delay_10ms(50);
        while(Gray1 == 0);

        Delay_10ms(23);
        state = 10;
        Delay_10ms(6);
        // Dbg(100);
        state = 5;
        SetSpeed(SpeedSlow);
        Delay_10ms(33);
        state = 6;
        Delay_10ms(6);
        SetSpeed(SpeedSlow);


        state = 2;  
        Delay_10ms(30);
        SetSpeed(SpeedMiddle);
        found90 = 0;
        while(found90 == 0);
        found90 = 0;
        Delay_10ms(100);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;
        LED1 = 1;
        if (single == 0)
            SendMsg('H');


        while(1);//结束程序
    }



    else if(turn_flag == 3)//       去右侧中部病房！！！！！！！！！！
    {//        
        state = 5;
        SetSpeed(SpeedMiddle);
        Delay_10ms(20);
        while(Gray6 == 0);
        state = 6;
        Delay_10ms(2);
        SetSpeed(SpeedSlow);

        state = 2;
        Delay_10ms(120);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;
        LED0 = 1;

        if(single == 0)
        {
            SendMsg('R');

            Delay_10ms(20);
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            while(1)
            {
                if(u1flag)
                {
                    u1flag = 0;
                    if(u1data == 20);//20代表可以走
                        break;
                }
            }
        }

        while(GD1 == 0);
        LED0  = 0;

        SetSpeed(SpeedSlow);
        Delay_10ms(50);
        state = 6;
        Delay_10ms(60);
        while(Gray3 == 0);//180°转向                开始回家
        state = 4;

        state = 2;
        SetSpeed(SpeedSlow);
        Delay_10ms(90);
        
        while(Gray7 == 0);

        Delay_10ms(25);
        state = 10;
        Delay_10ms(6);
        // Dbg(100);
        state = 6;
        SetSpeed(SpeedSlow);
        Delay_10ms(38);
        state = 5;
        Delay_10ms(6);
        SetSpeed(SpeedSlow);


        state = 2;  
        Delay_10ms(30);
        SetSpeed(SpeedMiddle);
        found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(100);
        while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
        state = 4;
        LED1 = 1;
        if (single == 0)
            SendMsg('H');


        while(1);//结束程序
    }














//去远端病房的屑程序---------==========================================================================================================
//---------------------------------------------------------------------------------------------------------================






























    after_middle:
    state = 2;
    SetSpeed(SpeedMiddle);
    Delay_10ms(60);
    found90 = 0;
    while(found90 == 0);
    found90 = 0;
    Delay_10ms(15);
    state = 4;
    
        //写的新识别方法-------------------------------------------------------------------------------------------------------
    turn_flag = 0;
    u3flag = 0;
    r = l = 8;
    rebuf[0]='A';rebuf[1]='8';rebuf[2]='8';rebuf[3]='B';
    Beep = 1;
    while(1){
        if(r == 8 && l == 8) continue;
        if (r == target) 
        {
            turn_flag = 0;//右
            break;
        }
        else if (l == target) 
        {
            turn_flag = 2;//左
            break;
        }
        if(r < 8 && r >= 0){
            dr = r;
            // map[3] = dr;
        }
        if(l < 8 && l >= 0){
            dl = l;
            // map[2] = dl;
        }
        if(dr < 8 && dr >= 0 && dl < 8 && dl >= 0){
            turn_flag = 0;
            // map[3] = dr;
            // map[2] = dl;
            break;
        }
    }
    Beep = 0;
//------------------------------===========检测结束了捏======================--------------------=========

//     if(turn_flag == 0){
//         state = 6;
//         Delay_10ms(23);
//         state = 5;
//         Delay_10ms(3);
//         state = 4;
        


// //写的新识别方法-------------------------------------------------------------------------------------------------------
       
//         u3flag = 0;
//         r = l = 8;
//         rebuf[0]='A';rebuf[1]='8';rebuf[2]='8';rebuf[3]='B';
//         Beep = 1;
//         while(1){
//             if(r == 8 && l == 8) continue;
//             if (l == target) 
//             {
//                 turn_flag = 2;
//                 state = 5;
//                 Delay_10ms(20);
//                 state = 6;
//                 Delay_10ms(3);
//                 state = 4;
//                 break;
//             }
//             if(l < 8 && l >= 0){
//                 dl = l;
//             }
//             if(dl < 8 && dl >= 0){
//                 turn_flag = 3;
//                 state = 5;
//                 Delay_10ms(18);
//                 state = 6; 
//                 Delay_10ms(3);
//                 state = 4;
//                 break;
//             }
//         }
//         Beep = 0;
//------------------------------===========检测结束了捏======================--------------------=========

    //}












//···················没检测到或者检测到在左边就去左边···························



    if (turn_flag == 2 || turn_flag == 0)//单人任务，进行数字识别
    {
        int abc = 0;
        state = 3;
        Delay_10ms(40);
        state = 6;

        SetSpeed(SpeedMiddle);//右转
        Delay_10ms(35);
        while(Gray5 == 0);
        state = 5;
        Delay_10ms(2);
        SetSpeed(SpeedSlow);

        state = 2;
        Delay_10ms(120);

        found90 = 0;
        while(found90 == 0);
        found90 = 0;
        Delay_10ms(15);
        state = 4;//stop

        //写的新识别方法-------------------------------------------------------------------------------------------------------
       
        u3flag = 0;
        r = l = 8;
        rebuf[0]='A';rebuf[1]='8';rebuf[2]='8';rebuf[3]='B';
        Beep = 1;
        if (turn_flag == 2) abc = 1;
        while(1){
            if(r == 8 && l == 8) continue;
            if (r == target) 
            {
                turn_flag = 3;
                break;
            }
            else if (l == target) 
            {
                turn_flag = 2;
                break;
            }
            if(r < 8 && r >= 0){
                dr = r;
            }
            if(l < 8 && l >= 0){
                dl = l;
            }
            if(dr < 8 && dr >= 0 && dl < 8 && dl >= 0){
                if (r != target && abc == 1)
                    turn_flag = 3;
                else if(l != target && abc == 1)
                    turn_flag = 2;
                else
                turn_flag = 0;
                // map[1] = dl;
                break;
            }
        }
        Beep = 0;
//------------------------------===========检测结束了捏======================--------------------=========
        if(0)
        {
            ceshi5:
            while(GD1 == 1);
            turn_flag = 2;
            single = 1;
        }

        if(0)
        {
            ceshi6:
            while(GD1 == 1);
            turn_flag = 3;
            single = 1;
        }

        if(turn_flag == 2 || turn_flag == 3){
            state = 3;
            Delay_10ms(75);
            //Dbg(100);
        }



        if(turn_flag == 2){//         去左左侧部病房！！！！！！！！！！
            state = 6;
            SetSpeed(SpeedMiddle);
            Delay_10ms(50);
            while(Gray3 == 0);
            state = 5;
            SetSpeed(5);
            state = 2;
            Delay_10ms(2);
            SetSpeed(SpeedSlow);

            state = 2;
            Delay_10ms(140);




            
            while(Gray3 == 0 || Gray4 == 0 || Gray5 == 0);
            state = 4;
            LED0 = 0;
            if(single == 0)
            {
                SendMsg('R');

                Delay_10ms(20);
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
                while(1)
                {
                    if(u1flag)
                    {
                        u1flag = 0;
                        if(u1data == 20);//20代表可以走
                            break;
                    }
                }
            }


            
            
            while(GD1 == 0);
            Delay_10ms(90);
            while(GD1 == 0);
            LED0 = 0;

            SetSpeed(SpeedSlow);
            Delay_10ms(50);
            state = 6;
            Delay_10ms(60);
            while(Gray3 == 0);//180°转向                开始回家
            state = 2;
            Delay_10ms(10);

            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(80);

            //Dbg(100);
            // state = 3;
            // Delay_10ms(60);
            state = 5;
            Delay_10ms(32);
            state = 2;
            Delay_10ms(5);//到中间
            

            found90 = 0;
            while(found90 == 0);
            found90 = 0;

            state = 3;
            Delay_10ms(85);
            state = 5;
            Delay_10ms(37);
            state = 2;
            Delay_10ms(150);
            SetSpeed(SpeedMiddle);
            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(100);
            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(100);
            while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
            state = 4;
            LED1 = 1;
        }


        else if(turn_flag == 3){//         去左右侧部病房！！！！！！！！！！
            state = 5;
            SetSpeed(SpeedMiddle);
            Delay_10ms(30);
            while(Gray5 == 0);
            state = 6;
            Delay_10ms(2);
            SetSpeed(SpeedSlow);

            state = 2;
            Delay_10ms(140);




            
            while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
            state = 4;
            LED0 = 1;

            if(single == 0)
            {
                SendMsg('R');

                Delay_10ms(20);
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
                while(1)
                {
                    if(u1flag)
                    {
                        u1flag = 0;
                        if(u1data == 20);//20代表可以走
                            break;
                    }
                }
            }
            
            while(GD1 == 0);
            LED0 = 0;

            SetSpeed(SpeedSlow);
            Delay_10ms(50);
            state = 6;
            Delay_10ms(60);
            while(Gray3 == 0);//180°转向                开始回家
            state = 5;
            Delay_10ms(5);
            state = 2;

            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(90);

            //Dbg(100);
            state = 6;
            Delay_10ms(40);
            state = 2;
            Delay_10ms(5);//到中间
            

            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            state = 3;
            Delay_10ms(84);
            state = 5;
            Delay_10ms(35);
            state = 2;
            Delay_10ms(150);
            SetSpeed(SpeedMiddle);
            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(100);
            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(100);
            while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
            state = 4;
            LED1 = 1;
        }






//························-----------------------------我tm直接去右 边---------------------------







        else
            {//没检测到，去右边的6、7
            
            nums = 0;
            state = 6;
            Delay_10ms(60);
            while(Gray3 == 0);//180°转向
            state = 2;
            Delay_10ms(10);
            
            
            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(40);

            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(27);
            state = 4;//到达右侧红线，后撤步准备检测


  

//写的新识别方法-------------------------------------------------------------------------------------------------------
       
            u3flag = 0;
            r = l = 8;
            Beep = 1;
            turn_flag = 0;
            while(1){
                if(r == 8 && l == 8) continue;
                if (r == target) 
                {
                    turn_flag = 3;
                    break;
                }
                else if (l == target) 
                {
                    turn_flag = 2;
                    break;
                }
                if(r < 8 && r >= 0){
                    dr = r;
                }
                if(l < 8 && l >= 0){
                    dl = l;
                }
                if(dr < 8 && dr >= 0 && dl < 8 && dl >= 0){
                    // map[4] = dr;
                    break;
                }
            }
            Beep = 0;
//------------------------------===========检测结束了捏======================--------------------=========

        

            

            if(0)
            {
                ceshi7:
                while(GD1 == 1);
                turn_flag = 2;
                single = 1;
            }

            if(0)
            {
                ceshi8:
                while(GD1 == 1);
                turn_flag = 3;
                single = 1;
            }

            {
                state = 3;
                Delay_10ms(68);
            }



            if(turn_flag == 2){//         去右左侧部病房！！！！！！！！！！
                state = 6;
            SetSpeed(SpeedMiddle);
            Delay_10ms(50);
            while(Gray5 == 0);
            state = 5;
            Delay_10ms(2);
            SetSpeed(SpeedSlow);

            state = 2;
            Delay_10ms(140);




            
            while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
            state = 4;
            LED0 = 0;
            if(single == 0)
            {
                SendMsg('R');

                Delay_10ms(20);
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
                while(1)
                {
                    if(u1flag)
                    {
                        u1flag = 0;
                        if(u1data == 20);//20代表可以走
                            break;
                    }
                }
            }
            
            while(GD1 == 0);
            LED0 = 0;

            SetSpeed(SpeedSlow);
            Delay_10ms(50);
            state = 6;
            Delay_10ms(60);
            while(Gray2 == 0);//180°转向                开始回家
            state = 5;
            Delay_10ms(5);
            state = 10;
            Delay_10ms(20);
            state = 2;
            Delay_10ms(10);

            found90 = 0;
            while(found90 == 0);
            found90 = 0;
            Delay_10ms(73);

            //Dbg(100);
            // state = 3;
            // Delay_10ms(60);
            state = 5;
            Delay_10ms(31);
            state = 2;
            Delay_10ms(20);//到中间
            

            found90 = 0;
            while(found90 == 0);
            found90 = 0;

            state = 3;
            Delay_10ms(76);
            state = 6;
            Delay_10ms(40);
            state = 5;
            Delay_10ms(5);
            state = 2;
            Delay_10ms(50);
            SetSpeed(SpeedMiddle);
            found90 = 0;
                while(found90 == 0);
                found90 = 0;
                
            Delay_10ms(100);
                found90 = 0;
                while(found90 == 0);
                found90 = 0;
                
            Delay_10ms(100);
            while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
            state = 4;
            LED1 = 1;
            }


            else{//         去右右侧部病房！！！！！！！！！！
                state = 5;
                SetSpeed(SpeedMiddle);
                Delay_10ms(30);
                while(Gray3 == 0);
                state = 6;
                Delay_10ms(2);
                SetSpeed(SpeedSlow);

                state = 2;
                Delay_10ms(140);




                
                while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
                state = 4;
                LED0 = 1;

                if(single == 0)
                {
                    SendMsg('R');

                    Delay_10ms(20);
                    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
                    while(1)
                    {
                        if(u1flag)
                        {
                            u1flag = 0;
                            if(u1data == 20);//20代表可以走
                                break;
                        }
                    }
                }
                
                while(GD1 == 0);
                LED0 = 0;

                SetSpeed(SpeedSlow);
                Delay_10ms(50);
                state = 6;
                Delay_10ms(60);
                while(Gray3 == 0);//180°转向                开始回家
                state = 2;
                Delay_10ms(10);

                found90 = 0;
                while(found90 == 0);
                found90 = 0;
                Delay_10ms(100);

                Dbg(100);
                state = 6;
                Delay_10ms(40);
                state = 2;
                Delay_10ms(30);//到中间
                

                found90 = 0;
                while(found90 == 0);
                found90 = 0;
                state = 3;
                Delay_10ms(90);
                state = 6;
                Delay_10ms(40);
                state = 2;
                Delay_10ms(150);
                SetSpeed(SpeedMiddle);
                while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
                state = 4;
                LED1 = 1;            
            }

        }

    
        
        
    }













//     else//]=====================去右边
//     {



//         state = 3;
//         Delay_10ms(40);
//         state = 5;

//         SetSpeed(SpeedMiddle);//右转
//         Delay_10ms(30);
//         while(Gray5 == 0);
//         state = 6;
//         Delay_10ms(2);
//         SetSpeed(SpeedSlow);

//         state = 2;
//         Delay_10ms(120);


//         found90 = 0;
//         while(found90 == 0);
//         found90 = 0;
//         Delay_10ms(10);
//         state = 4;//到达右侧红线，后撤步准备检测




// //写的新识别方法-------------------------------------------------------------------------------------------------------
    
//         u3flag = 0;
//         r = l = 8;
//         Beep = 1;
//         while(1){
//             if(r == 8 && l == 8) continue;
//             if (r == target) 
//             {
//                 turn_flag = 3;
//                 break;
//             }
//             else if (l == target) 
//             {
//                 turn_flag = 2;
//                 break;
//             }
//             if(r < 8 && r >= 0){
//                 dr = r;
//             }
//             if(l < 8 && l >= 0){
//                 dl = l;
//             }
//             if(dr < 8 && dr >= 0 && dl < 8 && dl >= 0){
//                 turn_flag = 0;
//                 break;
//             }
//         }
//         Beep = 0;
// //------------------------------===========检测结束了捏======================--------------------=========

    

        

//         if(0)
//         {
//             //ceshi7:
//             while(GD1 == 1);
//             turn_flag = 2;
//             single = 1;
//         }

//         if(0)
//         {
//             //ceshi8:
//             while(GD1 == 1);
//             turn_flag = 3;
//             single = 1;
//         }

//         {
//             state = 3;
//             Delay_10ms(84);
//             Dbg(100);
//         }



//         if(turn_flag == 2){//         去右左侧部病房！！！！！！！！！！
//             state = 6;
//         SetSpeed(SpeedMiddle);
//         Delay_10ms(40);
//         while(Gray3 == 0);
//         state = 5;

//         Delay_10ms(4);
//         SetSpeed(SpeedSlow);

//         state = 2;
//         Delay_10ms(120);




        
//         while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
//         state = 4;
//         LED0 = 1;
//         if(single == 0)
//         {
//             SendMsg('R');

//             Delay_10ms(20);
//             USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//             USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//             while(1)
//             {
//                 if(u1flag)
//                 {
//                     u1flag = 0;
//                     if(u1data == 20);//20代表可以走
//                         break;
//                 }
//             }
//         }
        
//         while(GD1 == 0);
//         LED0 = 0;

//         SetSpeed(SpeedSlow);
//         Delay_10ms(50);
//         state = 6;
//         Delay_10ms(60);
//         while(Gray2 == 0);//180°转向                开始回家
//         state = 5;
//         Delay_10ms(5);
//         state = 10;
//         Delay_10ms(20);
//         state = 2;
//         Delay_10ms(10);

//         found90 = 0;
//         while(found90 == 0);
//         found90 = 0;
//         Delay_10ms(73);

//         //Dbg(100);
//         // state = 3;
//         // Delay_10ms(60);
//         state = 5;
//         Delay_10ms(31);
//         state = 2;
//         Delay_10ms(20);//到中间
        

//         found90 = 0;
//         while(found90 == 0);
//         found90 = 0;

//         state = 3;
//         Delay_10ms(76);
//         state = 6;
//         Delay_10ms(40);
//         state = 5;
//         Delay_10ms(5);
//         state = 2;
//         Delay_10ms(50);
//         SetSpeed(SpeedMiddle);
//         found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
            
//         Delay_10ms(100);
//             found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
            
//         Delay_10ms(100);
//         while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
//         state = 4;
//         LED1 = 1;
//         }


//         else{//         去右右侧部病房！！！！！！！！！！
//             state = 5;
//             SetSpeed(SpeedMiddle);
//             Delay_10ms(30);
//             while(Gray5 == 0);
//             state = 6;
//             Delay_10ms(2);
//             SetSpeed(SpeedSlow);

//             state = 2;
//             Delay_10ms(140);




            
//             while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
//             state = 4;
//             LED0 = 1;

//             if(single == 0)
//             {
//                 SendMsg('R');

//                 Delay_10ms(20);
//                 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//                 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//                 while(1)
//                 {
//                     if(u1flag)
//                     {
//                         u1flag = 0;
//                         if(u1data == 20);//20代表可以走
//                             break;
//                     }
//                 }
//             }
            
//             while(GD1 == 0);
//             LED0 = 0;

//             SetSpeed(SpeedSlow);
//             Delay_10ms(50);
//             state = 6;
//             Delay_10ms(60);
//             while(Gray3 == 0);//180°转向                开始回家
//             state = 5;
//             Delay_10ms(5);
//             state = 2;
//             Delay_10ms(10);

//             found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
//             Delay_10ms(80);

//             Dbg(100);
//             state = 6;
//             Delay_10ms(35);
//             state = 5;
//             Delay_10ms(5);
//             state = 2;
//             Delay_10ms(30);//到中间
            

//             found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
//             state = 3;
//             Delay_10ms(75);
//             state = 6;
//             Delay_10ms(40);
//             state = 5;
//             Delay_10ms(4);
//             state = 2;
//             Delay_10ms(150);
//             SetSpeed(SpeedMiddle);
//             found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
            
//         Delay_10ms(100);
//             found90 = 0;
//             while(found90 == 0);
//             found90 = 0;
            
//         Delay_10ms(100);
//             while(Gray1 == 1 || Gray2 == 1 || Gray3 == 1 || Gray4 == 1 || Gray5 == 1 || Gray6 == 1 || Gray7 == 1);
//             state = 4;
//             LED1 = 1;            
//         }
//     }


    



    while(1){
        SendMap();
        USART_SendData(USART1, 'H');
        
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
    }
   
    return 0;
}

#endif 
