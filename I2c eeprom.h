#ifndef EEPROM_H
#define	EEPROM_H

#include "I2c.h"

#define EEPROM_Address_R 0xA1
#define EEPROM_Address_W 0xA0

typedef uint8_t Eep_AddressType;
typedef uint8_t Eep_LengthType;

typedef enum{
    EEP_UNINIT,
    EEP_IDLE,
    EEP_BUSY
}Eep_StatusType;

typedef struct {
    Eep_AddressType Eep_SlaveAddress;
    Eep_StatusType Eep_Status;
}Eep_ConfigType;

void Eep_Init(Eep_ConfigType* ConfigPtr);
Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length);
Std_ReturnType Eep_Read (Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length);

//---[ EEPROM Routines ]---
Std_ReturnType Eep_Erase ( Eep_AddressType EepromAddress, 
        Eep_LengthType Length );
Std_ReturnType Eep_Compare ( Eep_AddressType EepromAddress,
        uint8* DataBufferPtr, Eep_LengthType Length );
//MemIf_StatusType Eep_GetStatus ( void );

void EEPROM_Write(unsigned int *add, unsigned char *data);
void EEPROM_Read(unsigned int *add, uint8_t *data);

#endif