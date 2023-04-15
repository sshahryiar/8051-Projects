#define scale_factor                         0.4 //100Hz = 40m/s


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


unsigned int ms = 0x0000;
register unsigned int frequency = 0x0000;


void setup();
unsigned int get_timer_0();
void show_value(unsigned char x_pos, unsigned char y_pos, signed long value);


void Timer_1_Overflow_ISR()
iv IVT_ADDR_ET1
ilevel 0
ics ICS_AUTO
{
    TH1 = 0xFC;
    TL1 = 0x17;
    
    ms++;
    
    if(ms > 999)
    {
        TCON = 0x00;
        frequency = (get_timer_0() * 10);
        TL0 = 0x00;
        TH0 = 0x00;
        ms = 0x0000;
    }
}


void main()
{
     unsigned int wind_speed = 0x0000;

     setup();

     do
     {
         wind_speed = (frequency * scale_factor);
         show_value(11, 1, wind_speed);
         show_value(11, 2, frequency);
         TCON = 0x50;
         delay_ms(200);
     }while(1);
}


void setup()
{
    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;

    P2_5_bit = 0;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    LCD_Out(1, 1, "Speed m/s:");
    LCD_Out(2, 1, "Freq.  Hz:");

    TH0 = 0x00;
    TL0 = 0x00;
    TH1 = 0xFC;
    TL1 = 0x17;

    TCON = 0x50;
    TMOD = 0x15;

    IE = 0x88;
}


unsigned int get_timer_0()
{
    return ((TH0 << 0x08) + TL0);
}


void show_value(unsigned char x_pos, unsigned char y_pos, signed long value)
{
     unsigned char ch = 0x00;

     if(value < 0)
     {
          lcd_out(y_pos, x_pos, "-");
          value = -value;
     }
     else
     {
          lcd_out(y_pos, x_pos, " ");
     }

     if((value > 999) && (value <= 9999))
     {
          ch = ((value / 1000) + 0x30);
          lcd_chr_cp(ch);
          ch = (((value / 100) % 10) + 0x30);
          lcd_chr_cp(ch);
          ch = (((value / 10) % 10) + 0x30);
          lcd_chr_cp(ch);
          lcd_chr_cp('.');
          ch = ((value % 10) + 0x30);
          lcd_chr_cp(ch);
     }
     else if((value > 99) && (value <= 999))
     {
          lcd_chr_cp(0x20);
          ch = (((value / 100) % 10) + 0x30);
          lcd_chr_cp(ch);
          ch = (((value / 10) % 10) + 0x30);
          lcd_chr_cp(ch);
          lcd_chr_cp('.');
          ch = ((value % 10) + 0x30);
          lcd_chr_cp(ch);

     }
     else if((value > 9) && (value <= 99))
     {
          lcd_chr_cp(0x20);
          lcd_chr_cp(0x20);
          ch = (((value / 10) % 10) + 0x30);
          lcd_chr_cp(ch);
          lcd_chr_cp('.');
          ch = ((value % 10) + 0x30);;
          lcd_chr_cp(ch);
     }
     else if((value >= 0) && (value <= 9))
     {
          lcd_chr_cp(0x20);
          lcd_chr_cp(0x20);
          lcd_chr_cp('0');
          lcd_chr_cp('.');
          ch = ((value % 10) + 0x30);
          lcd_chr_cp(ch);
     }
}