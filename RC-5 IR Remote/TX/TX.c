#define BT1                         PINB0_bit
#define BT2                         PINB1_bit
#define BT3                         PINB2_bit
#define BT4                         PINB3_bit

#define TX                          PORTB4_bit

#define address_bit_size            5
#define command_bit_size            6

#define on_time                     8
#define off_time                    19

#define burst_length                31
#define pulse_length                810


bit toggle;


void setup();
void send_data(unsigned char address, unsigned char command);
void send(unsigned char value, unsigned char length);
void burst(unsigned char times);
void send_one();
void send_zero();


void main()
{
     unsigned char previous_data = 0x00;
     unsigned char present_data = 0x00;
     
     setup();

     while(1)
     {
             present_data = (PINB & 0x0F);
             if(present_data != 0x00)
             {
                 if(present_data == previous_data)
                 {
                    toggle = ~toggle;
                 }
                 else
                 {
                    toggle = 0;
                 }
                 
                 send_data(0x09, present_data);
                 previous_data = present_data;
                 
                 delay_ms(114);
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
    toggle = 0;
}


void send_data(unsigned char address, unsigned char command)
{
     send_one();
     send_one();

     if(toggle)
     {
        send_one();
     }
     else
     {
        send_zero();
     }
     
     send(address, address_bit_size);
     send(command, command_bit_size);
}


void send(unsigned char value, unsigned char length)
{
     unsigned char s = 0x00;
     unsigned char n = 0x00;
     
     switch(length)
     {
         case address_bit_size:
         {
             n = 0x10;
             break;
         }
         case command_bit_size:
         {
             n = 0x20;
             break;
         }
     }
     
     for(s = 0x00; s < length; s++)
     {
         if(value & n)
         {
             send_one();
         }
         else
         {
             send_zero();
         }
         value <<= 0x01;
     }
}


void burst(unsigned char times)
{
     unsigned char t = 0x00;

     for(t = 0x00; t < times; t++)
     {
           TX = 1;
           delay_us(on_time);
           TX = 0;
           delay_us(off_time);
     }
}


void send_one()
{
     TX = 0;
     delay_us(pulse_length);
     burst(burst_length);
}


void send_zero()
{
     burst(burst_length);
     delay_us(pulse_length);
}