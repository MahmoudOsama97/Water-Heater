#include "I2c eeprom.h"



Eep_AddressType* Eep_SlaveAddress;

void Eep_Init(Eep_ConfigType* ConfigPtr)
{
//    I2c_ConfigType I2c_Config = {
//    I2C0,0,100000,I2C_MASTER,
//    BITS_8,INTERNAL_BUFFER,
//    I2C_UNINIT,MSB_FIRST
//    };
    
    I2c_Init();
    *Eep_SlaveAddress = ConfigPtr->Eep_SlaveAddress;
}

Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length)
{
  I2C_Master_Start();
  // Wait Until EEPROM Is IDLE
  while(I2C_Master_Write(EEPROM_Address_W))
    I2C_Master_RepeatedStart();
  I2c_WriteIB(0,&EepromAddress);
  for(unsigned int i=0; i<Length; i++)
    I2c_WriteIB(0,DataBufferPtr++);
  I2C_Master_Stop();
  return E_OK;
}

Std_ReturnType Eep_Read (Eep_AddressType EepromAddress,uint8* DataBufferPtr, Eep_LengthType Length)
{
  I2C_Master_Start();
  // Wait Until EEPROM Is IDLE
  while(I2c_WriteSlaveAddress(EEPROM_Address_W))
    I2C_Master_RepeatedStart();
  I2c_WriteIB(0,&EepromAddress);
  I2C_Master_RepeatedStart();
  I2c_WriteSlaveAddress(EEPROM_Address_R);
  for(unsigned int i=0; i<Length; i++)
  {
    I2c_ReadIB(0,DataBufferPtr++);
    I2C_ACK();
  }
  I2C_Master_Stop();
  return E_OK;
}