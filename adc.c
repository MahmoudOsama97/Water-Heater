#include "adc.h"

//------------[ AD Converter Routines ]--------------

Port_ConfigType Port_Config={PortA,Pin2,PORT_PIN_IN,0,NO_PULL}  ;

Adc_ConfigType* Adc_ThisConfiguration ; 
Adc_ValueGroupType** Adc_Result;

//void __interrupt() ISR(void)
//{
//  if(ADIF)
//  {
//      **(Adc_Result) = (BUFF_REG_H << 8) + BUFF_REG_L; 
//    // Read A/D Conversion Result Registers ADRESH, ADRESL
//    // Clear The ADC Interrupt Flag Bit
//    ADIF = 0;
//    
//    //if(Adc_ThisConfiguration->Adc_GroupConvMode == ADC_CONV_MODE_CONTINUOUS)
//    //    {SET_BIT(CTL_REG1,BUSY_BIT);}    
//  }
//}

void Interrupt_ADCCallbackFn(void){
   GIE=0;
**(Adc_Result) = (BUFF_REG_H << 8) + BUFF_REG_L; 
    // Read A/D Conversion Result Registers ADRESH, ADRESL
    // Clear The ADC Interrupt Flag Bit
    ADIF = 0;
GIE=1;
}

//void Adc_init(Adc_ConfigType* Adc_Config)
//{
//    for(int i=0 ; i<Adc_Config->Adc_NbChannels; i++)
//    {   
//        Port_Init(&Port_Config[i]);
//        REG_VALUE_SHIFT(CTL_REG1,Adc_Config->ArrayOfAdcChannels[i],3);
//        REG_VALUE_SHIFT(CTL_REG1,(Adc_Config->Adc_Prescale & 0x03),6);
//        REG_VALUE_SHIFT(CTL_REG1,(Adc_Config->Adc_Prescale & 0x04),4);
//        REG_VALUE_SHIFT(CTL_REG1,Adc_Config->Adc_ResultAlignment,7);
//    }
//    CTL_REG2 |=  (0x2<<0); //Port Config Ctrl bits             
//    CTL_REG1 |=  (1>>0);
////    Interrupt_Enable(ADC);
//    Adc_Config = Adc_ThisConfiguration ;
//  //  Adc_ThisConfiguration->Adc_Status = ADC_IDLE;   
//}

#if (ADC_DEINIT_API==STD_ON)
void Adc_DeInit()
{
    CTL_REG1 = 0;
    CTL_REG2 = 0;
}
#endif

Std_ReturnType Adc_SetupResultBuffer( Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr )
{
    *Adc_Result = DataBufferPtr;
}

 Std_ReturnType Adc_ReadGroup( Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr )
{
  //for(uint8_t i = 0; i<Adc_ThisConfiguration->Adc_NbChannels; i++)
  //{
   *Adc_Result = DataBufferPtr ;
    CTL_REG1 &= 0x11000101; // Clear The Channel Selection Bits
     CTL_REG1 |= 2<<3;  
    //REG_VALUE_SHIFT(CTL_REG1,2,3);    // Select The Required Channel (i)
    __delay_us(30);       // The Minimum Tacq = 20us, So That should be enough
    SET_BIT(CTL_REG1,BUSY_BIT);          // Start A/D Conversion
//    Adc_ThisConfiguration->Adc_Status = ADC_BUSY;
    while(BIT_IS_SET(CTL_REG1,BUSY_BIT));
 //   Adc_ThisConfiguration->Adc_Status = ADC_COMPLETED;
  //}
  return E_OK;
}

#if (ADC_ENABLE_START_STOP_GROUP_API==STD_ON)
void Adc_StartGroupConversion ( Adc_GroupType groupId )
{
    SET_BIT(CTL_REG1,BUSY_BIT);
    while(BIT_IS_SET(CTL_REG1,BUSY_BIT));

}
void Adc_StopGroupConversion ( Adc_GroupType groupId )
{
    CLEAR_BIT(CTL_REG1,BUSY_BIT);
}
#endif

//#if (ADC_GRP_NOTIF_CAPABILITY==STD_ON)		
//void Adc_EnableGroupNotification(Adc_GroupType groupId)
//{
//    Interrupt_Enable(ADC);
//}
//void Adc_DisableGroupNotification(Adc_GroupType groupId)
//{
//    Interrupt_Disable(ADC);
//}
//#endif


//NOOOOOooOOoOOOoOoooO
void ADC_Burstt(uint16_t* x,uint16_t* y, uint8_t N){
  for(uint8_t i = 0; i<N; i++)
  {
    ADCON0 &= 0x11000101; // Clear The Channel Selection Bits
    ADCON0 |= i<<3;      // Select The Required Channel (i)
    //  ADCON0bits.CHS = i;                    // Wait The Aquisition Time
    __delay_us(30);       // The Minimum Tacq = 20us, So That should be enough
    GO_DONE = 1;          // Start A/D Conversion
    while(ADCON0bits.GO_DONE); // Polling GO_DONE Bit
                               // Provides Delay Until Conversion Is Complete
    if(i==0) *x = ((ADRESH << 8) + ADRESL);
    if(i==1) *y = ((ADRESH << 8) + ADRESL);
  }
}
uint16_t ADC_Read(uint8_t ANC, uint16_t*  AN_RES_BUF){
    CTL_REG1 &= 0b11000111; // Clear The Channel Selection Bits
    CTL_REG1 |= ANC<<3;     // Select The Required Channel (ANC)
                          // Wait The Aquisition Time
    __delay_us(30);       // The Minimum Tacq = 20us, So That should be enough
    GO_DONE = 1;          // Start A/D Conversion
    //while(ADCON0bits.GO_DONE);
    Adc_Result[0] = AN_RES_BUF ;
    return 0;// ((ADRESH << 8) + ADRESL); // Return The Right-Justified 10-Bit Result
}
void ADC_Init(){
    Port_Init(&Port_Config);
    CTL_REG1 = 0x51;  // Turn ADC ON, Select AN0 Channel, ADC Clock = Fosc/8
    CTL_REG2 = 0x80;  // All 8 Channels Are Analog, Result is "Right-Justified"
                    // ADC Clock = Fosc/8
ADIF = 0;  // Clear ADC Interrupt Flag
ADIE = 1;  // ADC Interrupt Enable Bit
PEIE = 1;  // Peripherals Interrupt Enable Bit
GIE = 1;   // Global Interrupts Enable Bit
    //if(Freq/pres <1.6) error
// TRISD = 0x00;
  //  TRISB = 0x00;
 Interrupt_Enable(ADC,Interrupt_ADCCallbackFn);
}
