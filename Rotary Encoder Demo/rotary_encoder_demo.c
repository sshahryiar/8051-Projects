#define encoder_A       !P3_2_bit
#define encoder_B       !P3_3_bit
#define mode_button     !P3_4_bit


void setup();
void display(unsigned char value);
unsigned char decode();
void bar_disp(unsigned char s);
void dot_disp(unsigned char s);


void main() 
{
     unsigned char n = 0;
     setup();
     while(1)
     {
             n = decode();
             display(n);
     };
}


void setup()
{
     P1 = 0x00;
     P3 = 0x1C;
}


void display(unsigned char value)
{
         static bit mode;
         if(mode_button)
         {
             while(mode_button);
             mode = ~mode;
         }
         switch(mode)
         {
             case 1:
             {
                 dot_disp(value);
                 break;
             }
             default:
             {
                 bar_disp(value);
                 break;
             }
         }
         delay_ms(10);
}


unsigned char decode()
{
         static bit previous_A;
         static bit present_A;
         static signed char n;
         present_A = encoder_A;
         if((!previous_A) && (present_A))
         {
             if(encoder_B)
             {
                 n += 1;
                 if(n >= 8)
                 {
                    n = 8;
                 }
             }
             else
             {
                 n -= 1;
                 if(n <= 0)
                 {
                    n = 0;
                 }
             }
         }
         previous_A = present_A;
         return ((unsigned char)n);
}


void bar_disp(unsigned char s)
{
     const unsigned char values[0x09] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
     P1 = ~(values[s]);
}


void dot_disp(unsigned char s)
{
     const unsigned char values[0x09] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
     P1 = ~(values[s]);
}