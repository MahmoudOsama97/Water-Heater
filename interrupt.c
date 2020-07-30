#include "interrupt.h"

void (*callbackTable[2])();

void __interrupt() ISR(void)
{
  
  if(TMR1IF)
  {
      callbackTable[0]();
  }
  if(ADIF)
  {
      callbackTable[1]();
  }
}

void Interrupt_Enable(Interrupt_SourceType Interrupt_Source, void (*callback)())
{
    if(Interrupt_Source == ADC)
    {
        ADIF = 0;  // Clear ADC Interrupt Flag
        ADIE = 1;  // ADC Interrupt Enable Bit
        PEIE = 1;  // Peripherals Interrupt Enable Bit
        GIE = 1;   // Global Interrupts Enable Bit
        callbackTable[1] = callback;
    }
    if(Interrupt_Source == TIMER)
    {
        TMR1IF = 0;  
        TMR1IE = 1;  
        PEIE = 1;  
        GIE = 1;   
        callbackTable[0] = callback;
    }
    
}

void Interrupt_Disable(Interrupt_SourceType Interrupt_Source)
{
    if(Interrupt_Source == ADC)
        ADIE = 0; 
    if(Interrupt_Source == ADC)
        TMR1IE = 0; 
}