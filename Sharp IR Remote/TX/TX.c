#define BT1       PINB0_bit
#define BT2       PINB1_bit
#define BT3       PINB2_bit
#define BT4       PINB3_bit

#define TX        PORTB4_bit


void setup();
void send_data(unsigned char address, unsigned char command);
void send(unsigned char value, unsigned char bits);
void burst();


void main()
{
     unsigned char v = 0;

     setup();

     while(1)
     {
             v = (~PINB & 0x0F);
             if(v != 0)
             {
                  send_data(0x09, v);
                  delay_ms(900);
             }
     };
}


void setup()
{
    CLKPR = 0x80;
    CLKPR = 0x00;
    PORTB = 0x0F;
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
    send(address, 5);
    send(command, 8);
    burst();
    delay_us(1000);
    burst();
    delay_ms(40);
    send(address, 5);
    send(~command, 8);
    burst();
    burst();
}


void send(unsigned char value, unsigned char bits)
{
     unsigned char s = bits;

     while(s > 0)
     {
         if((value & 0x01) != 0)
         {
             burst();
             delay_us(1000);
         }
         else
         {
             burst();
         }
         
         value >>= 1;
         s--;
     };
}


void burst()
{
     unsigned char t = 12;

     while(t > 0)
     {
           TX = 1;
           delay_us(8);
           TX = 0;
           delay_us(18);
           t--;
     };
     delay_us(680);
}