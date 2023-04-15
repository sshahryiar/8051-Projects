#include "DS1620.h"


void DS1620_init()
{
     P3 = 0x00;
     DS1620_write_config(0x0A);
}


void DS1620_rst_start()
{
     RST_pin = 0;
     CLK_pin = 1;
     RST_pin = 1;
}


void DS1620_rst_stop()
{
     RST_pin = 0;
}


void DS1620_start_conv()
{
     DS1620_rst_start();
     DS1620_send_command(start_temperature_conversion_command);
     DS1620_rst_stop();
}


void DS1620_stop_conv()
{
     DS1620_rst_start();
     DS1620_send_command(stop_temperature_conversion_command);
     DS1620_rst_stop();
}


void DS1620_send_command(unsigned char command)
{
    unsigned char n = 0;
    unsigned char b = 0;

    for(n = 0; n < 8; n++)
    {
         b = ((command >> n) & 0x1);
         DQ_pin = b;
         CLK_pin = 0;
         CLK_pin = 1;
    }
}


signed int DS1620_get_data()
{
    unsigned char n = 0x00;
    
    signed int b = 0x0000;
    signed int value = 0x0000;

    DQ_pin = 1;
    
    for(n = 0; n < 9; n++)
    {
        CLK_pin = 0;
        b = DQ_pin;
        CLK_pin = 1;
        value = (value | b << n);
    }
    
    DQ_pin = 0;

    return value;
}


unsigned char DS1620_read_config()
{
    unsigned char value = 0x00;

    DS1620_rst_start();
    DS1620_send_command(read_configuration_command);
    value = DS1620_get_data();
    DS1620_rst_stop();
    return value;
}


unsigned char DS1620_write_config(unsigned char value)
{
    if(value > 0) 
    {
        DS1620_rst_start();
        DS1620_send_command(write_configuration_command);
        DS1620_send_command(value);
        delay_ms(20);
        DS1620_rst_stop();
        
        if(DS1620_read_config() == value)
        { 
            return write_successful;
        }
        else 
        { 
            return write_unsuccessful;
        }
    }
    return bad_configuration;
}


signed int DS1620_read_TH()
{
    signed int value = 0x0000;

    DS1620_rst_start();
    DS1620_send_command(read_TH_command);
    value = (DS1620_get_data() >> 1);
    DS1620_rst_stop();
    return value;
}


signed int DS1620_read_TL()
{
    signed int value = 0x0000;

    DS1620_rst_start();
    DS1620_send_command(read_TL_command);
    value = (DS1620_get_data() >> 1);
    DS1620_rst_stop();
    return value;
}


signed int DS1620_read_counter()
{
    signed int value = 0x0000;

    DS1620_rst_start();
    DS1620_send_command(read_counter_command);
    value = DS1620_get_data();
    DS1620_rst_stop();
    return value;
}


signed int DS1620_read_slope()
{
    signed int value = 0x0000;

    DS1620_rst_start();
    DS1620_send_command(read_slope_command);
    value = DS1620_get_data();
    DS1620_rst_stop();
    return value;
}


void DS1620_write_TH(signed int value)
{
    unsigned char n = 0;
    signed int b = 0x0000;

    value = value << 1;
    DS1620_rst_start();
    DS1620_send_command(write_TH_command);
    for(n = 0; n < 9; n++)
    {
        b = ((value >> n) & 0x1);
        DQ_pin = b;
        CLK_pin = 0;
        CLK_pin = 1;
    }
    delay_ms(20);
    DS1620_rst_stop();
}


void DS1620_write_TL(signed int value)
{
    unsigned char n = 0;
    signed int b = 0x0000;

    value = value << 1;
    DS1620_rst_start();
    DS1620_send_command(write_TL_command);
    for(n = 0; n < 9; n++)
    {
        b = ((value >> n) & 0x1);
        DQ_pin = b;
        CLK_pin = 0;
        CLK_pin = 1;
    }
    delay_ms(20);
    DS1620_rst_stop();
}


void DS1620_read_T(signed char *m, unsigned char *f)
{
    signed int value = 0x00;

    DS1620_rst_start();
    DS1620_send_command(read_temperature_command);
    value = DS1620_get_data();
    DS1620_rst_stop();

    if (value & 0x0100)
    {
        value &= 0x00FF;
        value = 0x0100 - value;
        value = -value;
    }

    *m = value >>= 1;
    
    if(value & 0x01)
    {
        *f = 5;
    }
    else
    {
       *f = 0;
    }
}