#define d_LED     P1_0_bit
#define s_LED     P1_1_bit

#define B0        P1_2_bit
#define B1        P1_3_bit
#define B2        P1_4_bit


register unsigned int t=0;
unsigned short s=0;
unsigned short min=0;
unsigned short hr=0;
bit d;


void timer_ISR()
org 0x000B
{
     EA_bit=0;
     TH0=0xD8;
     TL0=0xF0;
     t++;
     if(t==760)
     {
           t=0;
           s_LED^=1;
           s++;
           if(s==60)
           {
                    s=0;
                    min++;
                    if(min==60)
                    {
                               min=0;
                               hr++;
                               if(hr=24)
                               {
                                        hr=0;
                               }
                    }
           }
           
     }
     EA_bit=1;
}


void main ()
{
     TCON=0x30;
     TMOD=0x10;
     IE=0x82;
     TH0=0xD8;
     TL0=0xF0;
     B0=1;
     B1=1;
     B2=1;
     P3=0x00;
     while(1)
     {
             if(!B0)
             {
                    delay_ms(50);
                    P3=~min;
                    while(B2)
                    {
                            TCON=0x00;
                            TMOD=0x00;
                            IE=0x00;
                            s_LED=0;
                            d_LED=1;
                            if(!B1)
                            {
                                   EA_bit=0;
                                   P3=~min;
                                   min++;
                                   delay_ms(250);
                                   if(min>59)
                                   {
                                             min=0;
                                   }
                            }
                    }
                    P3=0xFF;
                    delay_ms(500);
                    P3=~hr;
                    while(B2)
                    {
                            TCON=0x00;
                            TMOD=0x00;
                            IE=0x00;
                            d_LED=0;
                            s_LED=1;
                            if(!B1)
                            {
                                   EA_bit=0;
                                   P3=~hr;
                                   hr++;
                                   delay_ms(250);
                                   if(hr>23)
                                   {
                                             hr=0;
                                   }
                            }
                    }
                    P3=0xFF;
                    delay_ms(100);
                    d_LED=1;
                    s_LED=1;
                    TCON=0x30;
                    TMOD=0x10;
                    IE=0x82;
                    TH0=0xD8;
                    TL0=0xF0;
             }

             if((s%2)==0)
             {
                         d_LED=0;
                         P3=~min;
             }
             else
             {
                         d_LED=1;
                         P3=~hr;
             }
     };
}
