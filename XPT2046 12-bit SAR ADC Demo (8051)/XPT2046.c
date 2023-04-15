#include "XPT2046.h"


void XPT2046_init()
{
    P3 = 0x8F;

    CLK = 0;
    CS  = 1;
    DI = 1;
    CLK = 1;
    CS  = 0;
}


void XPT2046_write(unsigned char value)
{
    unsigned char i = 0x08;

    while(i > 0)
    {
         DI = (value >> 7);

         value <<= 1;

         CLK = 0;
         CLK = 1;
         delay_ms(1);

         i--;
    }
}


unsigned char XPT2046_read()
{
    unsigned char i = 0x08;
    unsigned char value = 0x00;

    while(i > 0)
    {
        value <<= 1;

        CLK = 1;
        CLK = 0;
        delay_ms(1);

        value |= DO;

        i--;
    }

    return value;
}


unsigned int XPT2046_read_ADC(unsigned char ch, unsigned char cmd)
{
    unsigned int temp1 = 0x0000;
    unsigned int temp2 = 0x0000;

    CS = 0;

    XPT2046_write(start_con | ch | cmd);
    XPT2046_conversion_delay();
    temp1 = XPT2046_read();
    temp2 = XPT2046_read();

    CS = 1;

    temp2 = ((temp1 << 8) | temp2);
    temp1 = (temp2 & 0xFFFF);
    temp1 >>= 4;

    return temp1;
}


void XPT2046_conversion_delay()
{
    CLK = 1;
    asm nop
    asm nop
    CLK = 0;
    asm nop
    asm nop
}


signed char XPT2046_get_temperature()
{
    float t = 0.0;

    t = (float)XPT2046_read_ADC(ch_temp0, (ADC_res_12 | single_ended_ADC | power_down_between_conversions_IRQ_enabled));
    t *= VDD_in_mV;
    t /= ADC_count_max;
    t = (306.85 - (0.467 * t));

    return ((signed char)t);
}