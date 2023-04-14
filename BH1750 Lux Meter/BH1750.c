#include "BH1750.h"
                                      
              
void BH1750_init()
{ 
   delay_ms(100);  
   BH1750_write(power_down);
}                  
              

void BH1750_write(unsigned char cmd)
{ 
   Soft_I2C_start();
   Soft_I2C_write(BH1750_write_cmd);        
   Soft_I2C_write(cmd);                
   Soft_I2C_stop();   
}

              
                                   
unsigned int BH1750_read_word()
{                      
   unsigned long value = 0; 
   unsigned char lb = 0;
   unsigned char hb = 0;
   
   Soft_I2C_start();
   Soft_I2C_write(BH1750_read_cmd);      
   hb = Soft_I2C_read(1);  
   lb = Soft_I2C_read(0);
   Soft_I2C_stop();                           
   value = ((hb << 8) | lb);     
   return value;
} 


unsigned int get_lux_value(unsigned char mode, unsigned int delay_time)
{
   unsigned long lux_value = 0;  
   unsigned char dly = 0;
   unsigned char s = 0;
   
   for(s = 0; s < 8; s += 1)
   {
       BH1750_write(power_up);
       BH1750_write(mode);
       lux_value += BH1750_read_word();
       for(dly = 0; dly < delay_time; dly += 1)
       {
           delay_ms(1);
       }
       BH1750_write(power_down);
   }
   lux_value >>= 3;
   
   return ((unsigned int)lux_value);
}                                