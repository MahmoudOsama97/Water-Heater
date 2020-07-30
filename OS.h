  
#ifndef OS_H
#define	OS_H

#include "OS_Config.h"
#include "Gpt.h"
#include "interrupt.h"

typedef void (*task)(void);
typedef uint8 priority;
typedef struct
{
    task T;
    priority P;
    uint8 rDelay;
    uint8 cDelay;
}TCB;

TCB readyQueue[numTasks];


//  task running;


Std_ReturnType OS_Init(void);
Std_ReturnType run_Scheduler(void);
Std_ReturnType create_task(task T,uint8 P,uint8 rDelay,uint8 cDelay);
void idletask(void);
void OS_ISR();
void starttickhook(void);
void OS_Sleep(uint8 P,uint8 rDelay);
#endif


