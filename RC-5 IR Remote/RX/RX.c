#define IR_pin             P3_2_bit

#define sample_time        800
#define wait_time          1800


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;
sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


bit received;
unsigned long value = 0;


void setup();
void reset_data();
void decode_RC5(unsigned char *tgl, unsigned char *addr, unsigned char *cmd);


void receive_IR_pulse()
iv IVT_ADDR_EX0
ilevel 0
ics ICS_AUTO
{
    unsigned char bits = 0x00;
    
    IE = 0x00;
    delay_us(wait_time);

    for(bits = 0x00; bits < 0x18; bits++)
    {
        value <<= 0x01;
        delay_us(sample_time);
        value |= ~IR_pin;
    }
    received = 0x01;
}


void main()
{
     unsigned char txt[6];

     unsigned char toggle = 0x00;
     unsigned char address = 0x00;
     unsigned char command = 0x00;

     setup();
     lcd_out(1, 5, "RC-5 Decoder");
     lcd_out(2, 1, "TGL: 0x00");
     lcd_out(3, 1, "ADR: 0x00");
     lcd_out(4, 1, "CMD: 0x00");

     while(1)
     {
             if(received)
             {
                 decode_RC5(&toggle, &address, &command);
                 
                 BytetoHex(toggle, txt);
                 lcd_out(2, 8, txt);
                 BytetoHex(address, txt);
                 lcd_out(3, 8, txt);
                 BytetoHex(command, txt);
                 lcd_out(4, 8, txt);
                 
                 reset_data();
             }
     };
}


void setup()
{
      P0 = 0xFF;
      P1 = 0xFF;
      P2 = 0xFF;
      P3 = 0xFF;

      P2_5_bit = 0;

      TMOD = 0x01;

      Lcd_Init();
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Cmd(_LCD_CURSOR_OFF);

      reset_data();
}


void reset_data()
{
     delay_ms(90);
     value = 0x00000000;
     received = 0x00;
     IE = 0x81;
}


void decode_RC5(unsigned char *tgl, unsigned char *addr, unsigned char *cmd)
{
    unsigned char s = 0x00;
    unsigned char tmp = 0x00;
    unsigned long temp = 0x0000;
    
    temp = (value & 0x0FFF);
    for(s = 0x00; s < 0x06; s++)
    {
        tmp <<= 0x01;
        if(temp & 0x0400)
        {
            tmp |= 0x01;
        }
        else if(temp & 0x0800)
        {
            tmp |= 0x00;
        }
        else
        {
            tmp = 0xFF;
        }
        temp <<= 0x02;
    }
    
    *cmd = tmp;


    temp = (value & 0x0FFF000);
    temp >>= 0x0C;
    tmp = 0x00;
    for(s = 0x00; s < 0x06; s++)
    {
        tmp <<= 0x01;
        if(temp & 0x0400)
        {
            tmp |= 0x01;
        }
        else if(temp & 0x0800)
        {
            tmp |= 0x00;
        }
        else
        {
            tmp = 0xFF;
        }
        temp <<= 0x02;
    }

    *addr = (tmp & 0x1F);
    *tgl = ((tmp & 0x20) >> 0x05);
}