#ifndef TIMER_H
#define	TIMER_H

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include "Dio.h"
#include "pic.h"
typedef uint8_t Gpt_ChannelType;
typedef uint8_t Gpt_ValueType;
typedef uint8_t Gpt_TickRateType;
typedef void (*Gpt_CallbackFn)(void);

typedef enum {
    TIMER_MODE,COUNTER_MODE
}Gpt_ModeType; 

typedef enum{
    GPT_PRE_1,GPT_PRE_2,GPT_PRE_4,GPT_PRE_8
}Gpt_PrescalerType;

typedef enum{
    GPT_INTERNAL,GPT_EXTERNAL
}Gpt_ClockSource;

typedef struct{
    Gpt_ChannelType Channel;
    Gpt_TickRateType TickRate;
    Gpt_ModeType Mode;
    Gpt_ClockSource ClkSource;
    Gpt_CallbackFn CallbackFn;
}Gpt_ConfigType;

/////////////////////////////////////////////////////////

void Gpt_Init(Gpt_ConfigType*);
void Timer1_init(void);




#endif