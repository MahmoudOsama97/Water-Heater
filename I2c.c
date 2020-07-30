#include "I2c.h"


void I2c_Init()
{
    
    CTL_REG1 |= 0x0F &  0b1000;
    SET_BIT(CTL_REG1,5);
    CTL_REG2 = 0;
    ADDR_REG = (_XTAL_FREQ/(4*100000))-1;
    STAT_REG = 0;
    
    Port_ConfigType I2c_Configurations[2] =
    {
        {PortC,Pin3,PORT_PIN_IN},
        {PortC,Pin4,PORT_PIN_IN}
    };
    
        Port_Init(&I2c_Configurations[0]);
        Port_Init(&I2c_Configurations[1]);
    //Config_Ptr->I2c_Status = I2C_IDLE;
}
void I2C_DeInit()
{
    CLEAR_REG(CTL_REG1);
    CLEAR_REG(CTL_REG2);
    CLEAR_REG(STAT_REG);
    CLEAR_REG(ADDR_REG);
}

void I2C_Master_Wait()
{
  while ((STAT_REG & 0x04) || (CTL_REG2 & 0x1F));
}
void I2C_Master_Start()
{
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,SEN_BIT);
}
void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,RSEN_BIT);
}
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,PEN_BIT);
}
unsigned char I2C_Master_Write(unsigned char data)
{
  I2C_Master_Wait();
  BUFF_REG = data;
  while(!BUSY_BIT); // Wait Until Completion
  BUSY_BIT = 0;
  return ACKSTAT;
}

unsigned char I2c_WriteSlaveAddress(unsigned char data)
{
  I2C_Master_Wait();
  BUFF_REG = data;
  while(!BUSY_BIT); // Wait Until Completion
  BUSY_BIT = 0;
  return ACKSTAT;
}

Std_ReturnType I2c_WriteIB(I2c_ChannelType Channel,
                    I2c_DataBufferType* DataBufferPtr)
{
    if(Channel==0)
    {
        I2C_Master_Wait();
        BUFF_REG = *DataBufferPtr;
        while(!BUSY_BIT); // Wait Until Completion
        BUSY_BIT = 0;
        return ACKSTAT;
    }
    else {/*error*/}
    return E_OK;
}
Std_ReturnType I2c_ReadIB ( I2c_ChannelType Channel, I2c_DataBufferType* DataBufferPtr )
{
    if(Channel==0)
    {
        I2C_Master_Wait();
        SET_BIT(CTL_REG2,RCEN_BIT); // Enable & Start Reception
        while(!BUSY_BIT); // Wait Until Completion
        BUSY_BIT = 0; // Clear The Interrupt Flag Bit
        I2C_Master_Wait();
        *DataBufferPtr = BUFF_REG; // Return The Received Byte
    }
    else {/*error*/}
    return E_OK;
}
unsigned char I2C_Read_Byte(void)
{
  //---[ Receive & Return A Byte ]---
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,RCEN_BIT); // Enable & Start Reception
  while(!BUSY_BIT); // Wait Until Completion
  BUSY_BIT = 0; // Clear The Interrupt Flag Bit
  I2C_Master_Wait();
  return BUFF_REG; // Return The Received Byte
}
void I2C_ACK(void)
{
  CLEAR_BIT(CTL_REG2,ACKDT_BIT);// 0 -> ACK
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,ACKEN_BIT);// Send ACK
}
void I2C_NACK(void)
{
  SET_BIT(CTL_REG2,ACKDT_BIT); // 1 -> NACK
  I2C_Master_Wait();
  SET_BIT(CTL_REG2,ACKEN_BIT);ACKEN = 1; // Send NACK
}



//void I2C_Master_Init(const unsigned long baud)
//{
//  SSPCON = 0b00101000;
//  SSPCON2 = 0;
//  SSPADD = (_XTAL_FREQ/(4*baud))-1;
//  SSPSTAT = 0;
//  Port_Init(&I2c_Configurations[0]);
//  Port_Init(&I2c_Configurations[1]);
//  
//}
