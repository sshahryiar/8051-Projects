#include "PCF8591.h"


void PCF8591_write(unsigned char control_value, unsigned char data_value)
{
     Soft_I2C_Start();
     Soft_I2C_Write(PCF8591_write_cmd);
     Soft_I2C_Write((control_value & 0xFF));
     Soft_I2C_Write(data_value);
     Soft_I2C_Stop();
}


unsigned char PCF8591_read(unsigned char control_value)
{
     unsigned char value = 0;
     
     Soft_I2C_Start();
     Soft_I2C_Write(PCF8591_write_cmd);
     Soft_I2C_Write((control_value & 0xFF));
     Soft_I2C_Stop();

     Soft_I2C_Start();
     Soft_I2C_Write(PCF8591_read_cmd);
     Soft_I2C_Read(1);
     value = Soft_I2C_Read(0);
     Soft_I2C_Stop();

     return value;
}