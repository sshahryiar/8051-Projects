#define echo_pin                    P3_2_bit
#define trigger_pin                 P3_1_bit

#define max_time                    38000
#define divisor                     58
#define trigger_pulse_width         10
#define delay_between_samples       40


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


unsigned char samples = 0x00;
unsigned int pulse_time = 0x0000;
unsigned long avg = 0x00000000;


void setup();
unsigned int get_timer();
void set_timer();


void detect_echo() 
iv IVT_ADDR_EX0 
ilevel 0 
ics ICS_AUTO 
{
    if(echo_pin == 0)
    {
        pulse_time = get_timer();
    }
}


void main()
{
    unsigned char txt[0x0C];
    unsigned int pulse_time_avg = 0x0000;

    setup();

    while(1)
    {
        set_timer();
        pulse_time = 0;
        
        trigger_pin = 1;
        delay_us(trigger_pulse_width);
        trigger_pin = 0;
        delay_ms(delay_between_samples);
        avg += pulse_time;
        
        if(samples > 0x0F)
        {
            avg >>= 0x04;
            pulse_time_avg = avg;
            avg /= divisor;
            
            if(avg >= max_time)
            {
                lcd_out(1, 12, "  MAX");
                lcd_out(2, 12, "  MAX");
            }
            else
            {
                WordToStr(((unsigned int)avg), txt);
                lcd_out(1, 12, txt);
                WordToStr(((unsigned int)pulse_time_avg), txt);
                lcd_out(2, 12, txt);
            }
            
            delay_ms(600);
            avg = 0;
            samples = 0;
        }
        
        samples++;

    };
}


void setup()
{
      P0 = 0xFF;
      P1 = 0xFF;
      P2 = 0xFF;
      P3 = 0xFF;

      P2_5_bit = 0;

      IE = 0x81;
      TCON = 0x11;
      TMOD = 0x09;

      Lcd_Init();
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Cmd(_LCD_CURSOR_OFF);
      
      lcd_out(1, 1, "Range/cm:");
      lcd_out(2, 1, "Time./uS:");
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
     TH0 = 0x00;
     TL0 = 0x00;
}