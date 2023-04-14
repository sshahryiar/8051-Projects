#define encoder_A       P3_5_bit
#define encoder_B       P3_6_bit
#define button          !P3_7_bit

#define up              1
#define down            0


const unsigned char num[12] =
{
      0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x5E, 0x1C
};

bit toggle;
bit mode;
bit chg;

signed char s = 0;
signed char hr = 0;
signed char min = 0;

signed char s_set = 0;
signed char hr_set = 0;
signed char min_set = 0;

unsigned char n = 0;
unsigned char pos = 0;

unsigned int ms = 0;


void setup();
signed char decode_encoder(signed char max);


void timer0_ISR()
org IVT_ADDR_ET0
{
    TL0 = 0x17;
    TH0 = 0xFC;
    
    switch(pos)
    {
        case 0:
        {
            n = (s % 10);
            P0 = num[n];
            P2 = 0xFE;
            break;
        }
        case 1:
        {
            n = (s / 10);
            P0 = num[n];
            P2 = 0xFD;
            break;
        }
        case 2:
        {
            n = (min % 10);
            if(toggle)
            {
               P0 = (num[n] | 0x80);
            }
            else
            {
               P0 = num[n];
            }
            P2 = 0xFB;
            break;
        }
        case 3:
        {
            n = (min / 10);
            P0 = num[n];
            P2 = 0xF7;
            break;
        }
        case 4:
        {
            n = (hr % 10);
            if(toggle)
            {
               P0 = (num[n] | 0x80);
            }
            else
            {
               P0 = num[n];
            }
            P2 = 0xEF;
            break;
        }
        case 5:
        {
            n = (hr / 10);
            P0 = num[n];
            P2 = 0xDF;
            break;
        }
        case 6:
        {
            if(mode)
            {
               P0 = num[11];
            }
            else
            {
               P0 = num[10];
            }

            P2 = 0x7F;
            break;
        }
    }

    pos++;
    if(pos > 6)
    {
        pos = 0;
    }
}


void timer1_ISR()
org IVT_ADDR_ET1
{
    TL1 = 0x17;
    TH1 = 0xFC;
    
    ms++;
    if(ms > 499)
    {
          toggle = 1;
    }
    if(ms > 999)
    {
          ms = 0;
          toggle = 0;
          chg = 1;
    }
    
    if(mode == up)
    {
        if(chg)
        {
           s++;
           chg = 0;
        }
        if(s > 59)
        {
             s = 0;
             min++;
        }
        if(min > 59)
        {
             min = 0;
             hr++;
        }
        if(hr > 23)
        {
            hr = 0;
        }
    }
    else
    {
        if(chg)
        {
           s--;
           chg = 0;
        }
        if(s < 0)
        {
             s = 59;
             min--;
        }
        if(min < 0)
        {
             min = 59;
             hr--;
        }
        if(hr < 0)
        {
            hr = 23;
        }
    }
}


void main() 
{
     bit set;
     bit alarm;
     
     unsigned char times = 0;
     
     set = 0;
     alarm = 0;
     
     setup();
     
     while(1)
     {
         if(((hr == hr_set) && (min == min_set) && (s == s_set) && (mode == up)) || ((hr == 0) && (min == 0) && (s == 0) && (mode == down)))
         {
            TR1_bit = 0;
            toggle = ~toggle;
            delay_ms(90);
         }
         
         if(button)
         {
            while(button);
            s = 0;
            min = 0;
            hr = 0;
            TR1_bit = 0;
            set = 1;
            toggle = 1;
         }
         
         if(set)
         {
            while(!button)
            {
                s = decode_encoder(59);
            }
            while(button);
            s_set = s;
            
            while(!button)
            {
                min = decode_encoder(59);
            }
            while(button);
            min_set = min;
            
            while(!button)
            {
                hr = decode_encoder(23);
            }
            while(button);
            hr_set = hr;
            
            while(!button)
            {
                mode = decode_encoder(1);
            }
            while(button);
            
            if(mode == up)
            {
                s = 0;
                min = 0;
                hr = 0;
            }
            
            TR1_bit = 1;
            set = 0;
         }
     }
}


void setup()
{
     P0 = 0xFF;
     P2 = 0xFF;
     P3 = 0xFF;
     TL0 = 0x17;
     TH0 = 0xFC;
     TL1 = 0x17;
     TH1 = 0xFC;
     TMOD = 0x11;
     TCON = 0x10;
     IE = 0x8A;
     chg = 0;
     mode = up;
}


signed char decode_encoder(signed char max)
{
     static bit previous_A;
     static bit present_A;
     signed char i;
     
     present_A = encoder_A;
     if((!previous_A) && (present_A))
     {
         if(encoder_B)
         {
             i++;
             if(i > max)
             {
                i = 0x00;
             }
         }
         else
         {
             i--;
             if(i < 0x00)
             {
                i = max;
             }
         }
     }
     
     previous_A = present_A;
     
     if(i > max)
     {
        i = 0x00;
     }
     
     return i;
}