#include "ADS1115.h"


void ADS1115_init()
{
     Soft_I2C_Init();
     delay_ms(100);
}


void ADS1115_write(unsigned char pointer, unsigned int value)
{
     Soft_I2C_Start();
     Soft_I2C_Write(ADS1115_write_cmd);
     Soft_I2C_Write(pointer);
     Soft_I2C_Write((value & 0xFF00) >> 8);
     Soft_I2C_Write(value & 0x00FF);
     Soft_I2C_Stop();
}


void ADS1115_configure(unsigned int value)
{
     ADS1115_write(ADS1115_config_reg_pointer, value);
     delay_ms(10);
}


void ADS1115_write_thresholds(unsigned char reg_select, unsigned int value)
{
     unsigned char ptr = 0x00;
     
     switch(reg_select)
     {
         case high_reg:
         {
              ptr = ADS1115_Hi_Thres_reg_pointer;
              break;
         }
         case low_reg:
         {
              ptr = ADS1115_Lo_Thres_reg_pointer;
              break;
         }
     }
     
     ADS1115_write(ptr, value);
     delay_ms(10);
}


unsigned int ADS1115_read(unsigned char pointer)
{
     unsigned char lb = 0x00;
     unsigned int hb = 0x0000;
     
     Soft_I2C_Start();
     Soft_I2C_Write(ADS1115_write_cmd);
     Soft_I2C_Write(pointer);
     Soft_I2C_Stop();
     
     Soft_I2C_Start();
     Soft_I2C_Write(ADS1115_read_cmd);
     hb = Soft_I2C_Read(0);
     lb = Soft_I2C_Read(0);
     Soft_I2C_Stop();
     
     hb <<= 8;
     hb |= lb;
     
     return hb;
}
