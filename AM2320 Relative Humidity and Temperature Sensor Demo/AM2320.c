#include "AM2320.h"


void AM2320_init()
{
     unsigned char s = 0;
     
     for(s = 0; s< 8; s++)
     {
           data_buffer[s] = 0x00;
     }
     
     Soft_I2C_Init();
}


void AM2320_write(unsigned char *value, unsigned char length)
{
     unsigned char s = 0x00;
     
     Soft_I2C_Start();
     Soft_I2C_Write(AM2320_address + I2C_write_cmd);
     
     for(s = 0x00; s < length; s++)
     {
           Soft_I2C_Write(*value++);
     }
     
     Soft_I2C_Stop();
}


void AM2320_read(unsigned char *value, unsigned char length)
{
     unsigned char s = 0x00;

     Soft_I2C_Start();
     Soft_I2C_Write(AM2320_address + I2C_read_cmd);
     
     for(s = 0x00; s < length; s++)
     {
         value[s] = Soft_I2C_read(1);
     }
     
     Soft_I2C_Stop();
}


unsigned int CRC16(unsigned char *ptr, unsigned char length)
{
      unsigned int crc = 0xFFFF;
      unsigned char s = 0x00;

      while(length--)
      {
        crc ^= *ptr++;
        for(s = 0; s < 8; s++)
        {
          if((crc & 0x01) != 0)
          {
            crc >>= 1;
            crc ^= 0xA001;
          }
          else
          {
            crc >>= 1;
          }
        }
      }

      return crc;
}


void get_sensor_data(unsigned char start_address, unsigned char input_length, unsigned char output_length)
{
     unsigned char s = 0x00;

     AM2320_write(0x00, 0x00);
     delay_us(1600);

     data_buffer[0] = AM2320_read_sensor_data;
     data_buffer[1] = start_address;
     data_buffer[2] = input_length;

     AM2320_write(data_buffer, 0x03);
     delay_us(1499);

     for(s = 0x00; s < output_length; s++)
     {
         data_buffer[s] = 0x00;
     }

     AM2320_read(data_buffer, output_length);
}


void get_RH_and_temperature(unsigned int *data1, signed int *data2)
{
     *data1 = ((unsigned int)((data_buffer[2] << 8) | data_buffer[3]));
     *data2 = ((data_buffer[4] << 8) | data_buffer[5]);
}


void get_CRC(unsigned int *CRC_data)
{
     *CRC_data = ((unsigned int)((data_buffer[7] << 8) | data_buffer[6]));
}