#ifndef __BLUETEETH_H
#define __BLUETEETH_H

#include "sys.h"


#define BtConnected PCin(3)


void BlueTooth_Init(void);
void SendMsg(u16 msg);

void BlueTeeth_Init (void);
void waitForNums(void);
void waitForLeftNums(void);
void waitForRightNums(void);
void SendMap(void);



#endif
