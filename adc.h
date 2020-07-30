#include <xc.h>
#include <stdint.h>
#include "Port.h"
#include "interrupt.h"

void ADC_Init();
uint16_t ADC_Read(uint8_t,uint16_t*);
//void ADC_Burst(uint16_t* x,uint16_t* y, uint8_t N);
void ADC_Burstt(uint16_t* x,uint16_t* y, uint8_t N);
void ADC_Burst(uint16_t*  AN_RES_BUF, uint8_t N);

/* ---------------------------------- CONSTANTS ---------------------------------- */

#define MAX_NB_CHANNELS 8
#define MAX_NB_GROUPS 8
#define CTL_REG1 ADCON0
#define CTL_REG2 ADCON1
#define BUFF_REG_H ADRESH
#define BUFF_REG_L ADRESL
#define BUSY_BIT Pin2

#define ADC_READ_GP_CAPABILITY STD_ON
#define ADC_ENABLE_START_STOP_GROUP_API STD_ON
#define ADC_GRP_NOTIF_CAPABILITY STD_ON
#define ADC_DEINIT_API STD_ON

/* ---------------------------------- PRIMITIVE TYPES ---------------------------------- */

typedef uint8_t Adc_ChannelType;
typedef uint8_t Adc_GroupType; //group index in array (0 - 7)
typedef uint16_t Adc_ValueGroupType;
typedef uint8_t Adc_ConversionTimeType; 
typedef uint8_t Adc_SamplingTimeType;
typedef uint8_t Adc_ResolutionType;
typedef uint8_t Adc_GroupPriorityType; 
typedef uint8_t Adc_StreamNumSampleType;

/* ---------------------------------- ENUMS ---------------------------------- */
typedef enum AdcChannel{
    PA0,PA1,PA2,PA3
}AdcChannel;
typedef enum Adc_PrescalerType{
    ADC_PRE_2,ADC_PRE_8,ADC_PRE_32,ADC_RC,ADC_PRE_4,ADC_PRE_16,ADC_PRE_64
}Adc_PrescalerType;
typedef enum Adc_StatusType{
	ADC_IDLE,
	ADC_BUSY,
	ADC_COMPLETED,
	ADC_STREAM_COMPLETED
}Adc_StatusType;
typedef enum Adc_GroupAccessModeType{  
ADC_ACCESS_MODE_SINGLE,
ADC_ACCESS_MODE_STREAMING
}Adc_GroupAccessModeType;
typedef enum Adc_StreamBufferModeType{ 
ADC_STREAM_BUFFER_LINEAR,
ADC_STREAM_BUFFER_CIRCULAR
}Adc_StreamBufferModeType;
typedef enum Adc_GroupConvModeType{  
ADC_CONV_MODE_ONESHOT,
ADC_CONV_MODE_CONTINUOUS
}Adc_GroupConvModeType;
typedef enum Adc_TriggerSourceType{
ADC_TRIG_SRC_SW, 
ADC_TRIG_SRC_HW
}Adc_TriggerSourceType;
typedef enum Adc_HwTriggerSourceType{
ADC_TRIG_EXTERNAL,
ADC_TRIG_TIMER,
NO_HW_TRIG
}Adc_HwTriggerSourceType;
typedef enum Adc_HwTriggerSignalType{
ADC_HW_TRIG_RISING_EDGE, 
ADC_HW_TRIG_FALLING_EDGE, 
ADC_HW_TRIG_BOTH_EDGES
}Adc_HwTriggerSignalType;
typedef enum Adc_ResultAlignmentType{
ADC_ALIGN_LEFT,
ADC_ALIGN_RIGHT
}Adc_ResultAlignmentType;
typedef enum Adc_PriorityImplementationType{
ADC_PRIORITY_NONE,
ADC_PRIORITY_HW,
ADC_PRIORITY_HW_SW
}Adc_PriorityImplementationType;

/* ---------------------------------- STRUCTS ---------------------------------- */

typedef struct Adc_ConfigType{
Adc_GroupType Adc_Group;  //Group Id
Adc_ChannelType Adc_NbChannels;
AdcChannel ArrayOfAdcChannels[MAX_NB_CHANNELS];
Adc_GroupConvModeType Adc_GroupConvMode;
Adc_ResultAlignmentType Adc_ResultAlignment;
Adc_PrescalerType Adc_Prescale;
Adc_StatusType Adc_Status;
//Adc_TriggerSourceType Adc_TriggerSource;
//Adc_HwTriggerSourceType Adc_HwTriggerSource;
//Adc_HwTriggerSignalType Adc_HwTriggerSignal;
//Adc_GroupAccessModeType Adc_GroupAccessMode;
//Adc_StreamNumSampleType Adc_NbSamples;
//Adc_StreamBufferModeType Adc_StreamBufferMode;
//Adc_GroupPriorityType GroupPriority;
}Adc_ConfigType;

/* ---------------------------------- FUNCTION DECLARATIONS ---------------------------------- */

void Adc_init(Adc_ConfigType*);

#if (ADC_DEINIT_API==STD_ON)		
void Adc_DeInit (void);
#endif

Std_ReturnType Adc_SetupResultBuffer( Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr ); 
//void Interrupt_ADCCallbackFn(void);

#if (ADC_READ_GP_CAPABILITY==STD_ON)
Std_ReturnType Adc_ReadGroup ( Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr );
#endif


#if (ADC_ENABLE_START_STOP_GROUP_API==STD_ON)		
void Adc_StartGroupConversion ( Adc_GroupType groupId );
void Adc_StopGroupConversion ( Adc_GroupType groupId );
#endif

#if (ADC_GRP_NOTIF_CAPABILITY==STD_ON)		
void Adc_EnableGroupNotification(Adc_GroupType groupId);
void Adc_DisableGroupNotification(Adc_GroupType groupId);
#endif


#if (ADC_HW_TRIGGER_API == STD_ON)
void Adc_EnableHardwareTrigger ( Adc_GroupType Group );
void Adc_DisableHardwareTrigger ( Adc_GroupType Group );
#endif

#if(ADC_GET_STREAM_API==STD_ON)
Adc_StreamNumSampleType Adc_GetStreamLastPointer ( Adc_GroupType Group, volatile Adc_ValueGroupType** PtrToSamplePtr );
#endif
