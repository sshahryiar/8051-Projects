#define BT1       PINB0_bit
#define BT2       PINB1_bit
#define BT3       PINB2_bit
#define BT4       PINB3_bit

#define TX        PORTB4_bit


void setup();
void send_data(unsigned char address, unsigned char command);
void send(unsigned char value);
void burst(unsigned char times);


void main()
{
     unsigned char v = 0;
     
     setup();

     while(1)
     {
             v = (PINB & 0x0F);
             if(v != 0)
             {
                  switch(v)
                  {
                      case 1:
                      {
                           send_data(0x00, 0x30);
                           break;
                      }
                      case 2:
                      {
                           send_data(0x00, 0x18);
                           break;
                      }
                      case 3:
                      {
                           send_data(0x00, 0x85);
                           break;
                      }
                      case 4:
                      {
                           send_data(0x00, 0x10);
                           break;
                      }
                      case 5:
                      {
                           send_data(0x00, 0x38);
                           break;
                      }
                      case 6:
                      {
                           send_data(0x00, 0x5A);
                           break;
                      }
                      case 7:
                      {
                           send_data(0x00, 0x42);
                           break;
                      }
                      case 8:
                      {
                           send_data(0x00, 0x2A);
                           break;
                      }
                      case 9:
                      {
                           send_data(0x00, 0x52);
                           break;
                      }
                  }
                  
                  delay_ms(900);
             }
     };
}


void setup()
{
    CLKPR = 0x80;
    CLKPR = 0x00;
    PORTB = 0x00;
    DDRB = 0x10;
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = 0x00;
    OCR0B = 0x00;
    GIMSK = 0x00;
    MCUCR = 0x00;
    TIMSK0 = 0x00;
    ACSR = 0x80;
    ADCSRB = 0x00;
    DIDR0 = 0x00;
    ADCSRA = 0x00;
}


void send_data(unsigned char address, unsigned char command)
{
     burst(172);
     burst(172);
     delay_us(4500);
     
     send(address);
     send(~address);
     send(command);
     send(~command);
}


void send(unsigned char value)
{
     unsigned char s = 0;
     
     for(s = 0; s < 8; s++)
     {
         burst(22);
         if(value & 0x80)
         {
             delay_us(1690);
         }
         else
         {
             delay_us(560);
         }
         value <<= 1;
     }
}


void burst(unsigned char times)
{
     unsigned char t = 0;

     for(t = 0; t < times; t++)
     {
           TX = 1;
           delay_us(13);
           TX = 0;
           delay_us(13);
     }
}