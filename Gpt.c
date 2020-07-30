#include "Gpt.h"

Gpt_ConfigType* Gpt_ThisConfig ;
uint32_t Gpt_ReloadValue ;
 
void Interrupt_TimerCallbackFn()
{
    TMR1=Gpt_ReloadValue;
    TMR1IF = 0;
    Gpt_ThisConfig->CallbackFn();
  
}

void Gpt_Init(Gpt_ConfigType* ConfigPtr)
{
    T1CON |= (ConfigPtr->ClkSource<<1);
    
    if(ConfigPtr->TickRate>39)
    {
        T1CON = (GPT_PRE_1 <<4);
        Gpt_ReloadValue=(65535-(_XTAL_FREQ/(ConfigPtr->TickRate*4*(pow(2,GPT_PRE_1)))));
    }
    else if(ConfigPtr->TickRate>20)
    {
        T1CON = (GPT_PRE_2 <<4);
        Gpt_ReloadValue=(65535-(_XTAL_FREQ/(ConfigPtr->TickRate*4*(pow(2,GPT_PRE_2)))));
    }
    else if(ConfigPtr->TickRate>10)
    {
        T1CON = (GPT_PRE_4 <<4);
        Gpt_ReloadValue=(65535-(_XTAL_FREQ/(ConfigPtr->TickRate*4*(pow(2,GPT_PRE_4)))));
    }
    else if(ConfigPtr->TickRate>5)
    {
        T1CON = (GPT_PRE_8 <<4);
        Gpt_ReloadValue=(65535-(_XTAL_FREQ/(ConfigPtr->TickRate*4*(pow(2,GPT_PRE_8)))));
    }
    else {/*error*/}
    TMR1=Gpt_ReloadValue; 
    T1CON |= 1 ;
    Interrupt_Enable(TIMER,Interrupt_TimerCallbackFn);
    Gpt_ThisConfig = ConfigPtr ;
}
