
#include "OS.h"

Gpt_ConfigType ConfigPtr={0,TICK,TIMER_MODE,GPT_INTERNAL,&OS_ISR};

void idletask(void)
{
    for(;;);
}
Std_ReturnType create_task(task T,uint8 P,uint8 rDelay,uint8 cDelay){
    readyQueue[P].T=T;
    readyQueue[P].P=P;
    readyQueue[P].rDelay=rDelay;
    readyQueue[P].cDelay=cDelay;
}

Std_ReturnType OS_Init(void){
     for(int i=0;i<MaxPriority;i++){
        readyQueue[i].T=NULL;
    }
    create_task(&idletask , 0 ,0,0);
    Gpt_Init(&ConfigPtr);
   
}

Std_ReturnType run_Scheduler(void){
    uint8 pri=MaxPriority-1;
    
    while(pri!=0){
    if(readyQueue[pri].T!=NULL){
        if(readyQueue[pri].cDelay==0){
            readyQueue[pri].cDelay=readyQueue[pri].rDelay;
            (*readyQueue[pri].T)();
        }
        else{ 
           readyQueue[pri].cDelay--;
  
        }
        //readyQueue[pri].T=NULL;
        //)(* running)();
        pri--;
    }
    else
        pri--;
    }
    
//     running= NULL;
     //idletask();
     return E_OK;
}
void OS_Sleep(uint8 P,uint8 rDelay){
    
    readyQueue[P].rDelay=rDelay;

}

void OS_ISR(){
   // #if Tickhook==1
    //starttickhook();
//#endif
    //GIE=0;
    run_Scheduler();
    //GIE=1;
}