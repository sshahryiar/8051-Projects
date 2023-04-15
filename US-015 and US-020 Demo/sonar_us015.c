#define button                      P3_3_bit

#define echo_pin                    P3_2_bit
#define trigger_pin                 P3_1_bit

#define max_time                    64000
#define divisor_cm                  58
#define divisor_in                  148
#define trigger_pulse_width         10
#define delay_between_samples       40

#define d_cm                        0
#define d_in                        1
#define t_us                        2

#define LOW                         0
#define HIGH                        1


const unsigned char num[10] =
{
      0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};


unsigned char pos = 0x00;
unsigned char data_value[0x08] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned int pulse_time = 0x0000;


void setup();
void display_value(unsigned int value, unsigned char type);
unsigned int get_timer();
void set_timer();


void timer1_ISR()
iv IVT_ADDR_ET1
ilevel 1
ics ICS_AUTO
{
    TL1 = 0x17;
    TH1 = 0xFC;

    P2 = (0xE3 + (pos << 2));
    P0 = data_value[pos];

    pos++;
    if(pos > 7)
    {
        pos = 0;
    }
}


void detect_echo() 
iv IVT_ADDR_EX0 
ilevel 0 ics 
ICS_AUTO
{
    if(echo_pin == LOW)
    {
        pulse_time = get_timer();
    }
}


void main()
{
     unsigned char mode = 0x00;
     unsigned char samples = 0x00;
     unsigned int pulse_time_avg = 0x0000;
     unsigned long avg = 0x00000000;
     
     setup();

     while(1)
     {
              set_timer();
              pulse_time = 0;

              trigger_pin = HIGH;
              delay_us(trigger_pulse_width);
              trigger_pin = LOW;
              delay_ms(delay_between_samples);
              avg += pulse_time;

              if(samples > 0x0F)
              {
                  avg >>= 0x04;
                  pulse_time_avg = avg;

                  if(pulse_time_avg <= max_time)
                  {
                      switch(mode)
                      {
                          case d_cm:
                          {
                              avg /= divisor_cm;
                              display_value(avg, d_cm);
                              break;
                          }
                          case d_in:
                          {
                              avg /= divisor_in;
                              display_value(avg, d_in);
                              break;
                          }
                          case t_us:
                          {
                              display_value(pulse_time_avg, t_us);
                              break;
                          }
                      }
                  }
                  
                  else
                  {
                      display_value(0, t_us);
                  }

                  delay_ms(600);
                  avg = 0;
                  samples = 0;
              }

              samples++;
              
              if(button == LOW)
              {
                 while(button == LOW);
                 mode++;
                 delay_ms(40);
                 
                 if(mode > t_us)
                 {
                    mode = d_cm;
                 }
              }
     }
}


void setup()
{
     P0 = 0xFF;
     P1 = 0xFF;
     P2 = 0xFF;
     P3 = 0xFF;
     TL0 = 0x00;
     TH0 = 0x00;
     TL1 = 0x17;
     TH1 = 0xFC;
     TCON = 0x51;
     TMOD = 0x19;
     IE = 0x89;
}


void display_value(unsigned int value, unsigned char type)
{
     unsigned char ch = 0x00;
     
     ch = ((value / 100) % 10);
     data_value[4] = num[ch];
     
     ch = ((value / 10) % 10);
     data_value[3] = num[ch];
     
     ch = (value % 10);
     data_value[2] = num[ch];
     
     switch(type)
     {
         case d_cm:
         {
             data_value[7] = 0xD0;
             data_value[6] = 0x00;
             data_value[5] = 0x00;
             data_value[1] = 0xD8;
             data_value[0] = 0xD4;
             break;
         }
         case d_in:
         {
             data_value[7] = 0xD0;
             data_value[6] = 0x00;
             data_value[5] = 0x00;
             data_value[1] = 0x86;
             data_value[0] = 0xD4;
             break;
         }
         case t_us:
         {
             data_value[7] = 0xF8;

             ch = value / 10000;
             data_value[6] = num[ch];

             ch = ((value / 1000) % 10);
             data_value[5] = num[ch];

             data_value[1] = 0x9C;
             data_value[0] = 0xED;
             break;
         }
     }
     
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