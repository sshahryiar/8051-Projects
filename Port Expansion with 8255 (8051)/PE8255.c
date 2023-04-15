#include "8255.c"


void setup();


void main()
{
    unsigned char data_value = 0x00;

    setup();

    while(1)
    {
        data_value = read_8255(PORT_A);
        write_8255(PORT_C, ~data_value);
    };
}


void setup()
{
    P1 = 0xFF;
    P3 = 0xFF;
    _8255_init();
    delay_ms(100);
}