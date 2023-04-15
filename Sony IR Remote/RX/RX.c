#define sync_high           4200
#define sync_low            3000
#define one_high            2040
#define one_low             1560
#define zero_high           1320
#define zero_low            1080

#define no_of_bits            13

sbit LCD_RS at P2_5_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;

bit received;
unsigned char bits = 0;
unsigned int frames[no_of_bits];


void setup();
void erase_frames();
unsigned int get_timer();
void set_timer();
unsigned char decode(unsigned char start_pos, unsigned char end_pos);
void decode_SIRC(unsigned char *addr, unsigned char *cmd);


void receive_IR_pulse()
iv IVT_ADDR_EX0
ilevel 0
ics ICS_AUTO
{
    frames[bits] = get_timer();
    bits++;
    TR0_bit = 1;
    
    if(bits >= no_of_bits)
    {
       received = 1;
       TR0_bit = 0;
    }
    set_timer();
}


void main()
{
     char txt[6];
     
     
     unsigned char address = 0x00;
     unsigned char command = 0x00;

     setup();

     lcd_out(1, 1, "Address:    0x00");
     lcd_out(2, 1, "Command:    0x00");

     while(1)
     {
             if(received)
             {
                 decode_SIRC(&address, &command);

                 BytetoHex(address, txt);
                 lcd_out(1, 15, txt);
                 BytetoHex(command, txt);
                 lcd_out(2, 15, txt);
                 erase_frames();
             }
     };
}


void setup()
{
      P0 = 0xFF;
      P1 = 0xFF;
      P2 = 0xFF;
      P3 = 0xFF;
      
      P2_6_bit = 0;

      IE = 0x81;
      TCON = 0x01;
      TMOD = 0x01;

      Lcd_Init();
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Cmd(_LCD_CURSOR_OFF);

      erase_frames();
}


void erase_frames()
{
     delay_ms(45);
     
     for(bits = 0; bits < no_of_bits; bits++)
     {
           frames[bits] = 0x0000;
     }

     set_timer();
     received = 0;
     bits = 0;
}


unsigned int get_timer()
{
    unsigned int time = 0;

    time = TH0;
    time <<= 8;
    time |= TL0;

    return time;
}


void set_timer()
{
     TH0 = 0;
     TL0 = 0;
}


unsigned char decode(unsigned char start_pos, unsigned char end_pos)
{
     unsigned char value = 0;
     
     bits = start_pos;
     while(bits >= end_pos)
     {
           value <<= 1;
           if((frames[bits] >= one_low) && (frames[bits] <= one_high))
           {
               value |= 1;
           }
           else if((frames[bits] >= zero_low) && (frames[bits] <= zero_high))
           {
               value |= 0;
           }
           else if((frames[bits] >= sync_low) && (frames[bits] <= sync_high))
           {
               return 0xFF;
           }
           bits--;
     }
     
     return value;
}


void decode_SIRC(unsigned char *addr, unsigned char *cmd)
{
     *addr = decode(12, 9);
     *cmd = decode(8, 2);
}