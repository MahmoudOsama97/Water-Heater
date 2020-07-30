
#include "adc.h"
#include "Dio.h"
#include "Port.h"
#define _XTAL_FREQ 10000000
#include "interrupt.h"
#include "Gpt.h"
#include "OS_Config.h"
#include "OS.h"
#include "7seg.h"
#include "Dio_Regs.h"
#include "I2c eeprom.h"


uint8 address=10;
uint8 x[2];
uint8 dis=0;  
uint8 tempD=60;
uint8 tempR[2];
uint8 desired =60;
uint8 mode=0;
uint8 reset=0;

Port_ConfigType Port_Configxx[5] = {
   {PortC,Pin5,PORT_PIN_OUT,HIGH,NO_PULL},
    {PortC,Pin2,PORT_PIN_OUT,HIGH,NO_PULL},
   {PortB,Pin0,PORT_PIN_IN,LOW,PULL_UP},
    {PortB,Pin1,PORT_PIN_IN,LOW,PULL_UP},
   {PortB,Pin2,PORT_PIN_IN,LOW,PULL_UP}
};

void DISP_Task(void){
    if(!Dio_ReadChannel(10)) reset=!reset;
    if((!Dio_ReadChannel(8))||(!Dio_ReadChannel(9))){
        mode=1;
   Dio_WriteChannel(21,LOW);
    Dio_WriteChannel(18,LOW);
    if(!Dio_ReadChannel(8)){
        desired-=5;
    }
    if(!Dio_ReadChannel(9)){
        desired+=5;
    }
    OS_Sleep(1,250);
    OS_Sleep(2,250);
    OS_Sleep(4,5);
    x[0]=desired;
    }
    if(mode)tempD=(uint8)desired;
    else {tempD=(uint8)(tempR[0]/2);
    }
    if(dis){
        
        display((uint8)tempD%10,4);   
    }
    else{ 
       display((uint8)tempD/10,3);
    }
    dis=!dis;
   
   
}
void Idle_Task(void){

while(1)
    {   
        if(reset){
            
            TRISD = 0x00;
            PORTD = 0;
            Dio_WriteChannel(18,LOW);
            Dio_WriteChannel(21,LOW);
            if(GIE==0)
            Eep_Write(address,x,1);
            GIE=!GIE;
        }
    
    }
}
void ADC_Task(void){
    OS_Sleep(1,0);
    OS_Sleep(4,0);
    mode=0;
    Adc_ReadGroup(2,tempR);

    
}
void HeaterCooler_Task(void){
     OS_Sleep(2,0);
     OS_Sleep(4,0);
     mode=0;
    if(tempD<(desired-5)){
    Dio_WriteChannel(21,HIGH);
    Dio_WriteChannel(18,LOW);
    }
    else if(tempD>(desired+5)){
    Dio_WriteChannel(18,HIGH);
    Dio_WriteChannel(21,LOW);
    }
}

Eep_ConfigType Eep_Config = {0xA0};

void main(void) {
    
    sevenSeg_init();
    
    Eep_Init(&Eep_Config);
    
    ADC_Init();
    
    Port_Init(&Port_Configxx[0]);
    Port_Init(&Port_Configxx[1]);
    Port_Init(&Port_Configxx[2]);
    Port_Init(&Port_Configxx[3]);
    Port_Init(&Port_Configxx[4]);
    
    OS_Init();
    create_task(&ADC_Task , 1,0,0 );
    create_task(&DISP_Task ,4,0,0 );
    create_task(&HeaterCooler_Task , 2 ,0,0);
    run_Scheduler();
    
    Idle_Task();
}
