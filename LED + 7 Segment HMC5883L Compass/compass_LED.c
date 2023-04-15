#include "HMC5883L.c"


unsigned char n = 0;
unsigned char value = 0;
unsigned char bit_pos = 0;

signed long h = 0;


const unsigned char LED[0x08] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
const unsigned char num[0x0A] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


void setup();
void switch_LEDs(unsigned char value);


void Timer_0_ISR()
iv IVT_ADDR_ET0
ilevel 0x00
ics ICS_AUTO
{
    TL0 = 0x18;
    TH0 = 0xFC;

    switch(bit_pos)
    {
        case 1:
        {
            if((value >= 8) && (value < 16))
            {
                P0 = LED[(value - 8)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xFD;
            break;
        }
        case 2:
        {
            if((value >= 16) && (value < 24))
            {
                P0 = LED[(value - 16)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xFB;
            break;
        }
        case 3:
        {
            if((value >= 24) && (value < 32))
            {
                P0 = LED[(value - 24)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xF7;
            break;
        }
        case 4:
        {
            if((value >= 32) && (value < 40))
            {
                P0 = LED[(value - 32)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xEF;
            break;
        }
        case 5:
        {
            if((value >= 40) && (value < 48))
            {
                P0 = LED[(value - 40)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xDF;
            break;
        }
        case 6:
        {
            if((value >= 48) && (value < 56))
            {
                P0 = LED[(value - 48)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xBF;
            break;
        }
        case 7:
        {
            if((value >= 56) && (value < 60))
            {
                P0 = LED[(value - 56)];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0x7F;
            break;
        }
        case 8:
        {
            n = (h / 100);
            P0 = num[n];
            P2 = 0xFF;
            P3 = 0xEF;
            break;
        }
        case 9:
        {
            n = ((h / 10) % 10);
            P0 = num[n];
            P2 = 0xFF;
            P3 = 0xDF;
            break;
        }
        case 10:
        {
            n = (h % 10);
            P0 = num[n];
            P2 = 0xFF;
            P3 = 0xBF;
            break;
        }
        case 11:
        {
            P0 = 0x9C;
            P2 = 0xFF;
            P3 = 0x7F;
            break;
        }
        default:
        {
            if((value >= 0) && (value < 8))
            {
                P0 = LED[value];
            }
            else
            {
                P0 = 0xFF;
            }

            P3 = 0xF0;
            P2 = 0xFE;
            break;
        }
    }

    bit_pos++;

    if(bit_pos > 11)
    {
        bit_pos = 0;
    }
}


void main()
{
     setup();

     while(1)
     {
         h = HMC5883L_heading();
         value = (h / 6);
         delay_ms(100);
     };
}


void setup()
{
     P0 = 0xFF;
     P1 = 0xFF;
     P2 = 0xFF;
     P3 = 0xFC;

     TL0 = 0x18;
     TH0 = 0xFC;

     TMOD = 0x01;
     TCON = 0x10;
     IE = 0x82;

     HMC5883L_init();
}