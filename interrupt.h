
#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <xc.h>
#include "Port.h"
#include "Dio.h"
typedef enum {
    ADC,TIMER
}Interrupt_SourceType;

void Interrupt_Enable(Interrupt_SourceType Interrupt_Source, void (*callback)());

void Interrupt_Disable(Interrupt_SourceType Interrupt_Source);

#endif