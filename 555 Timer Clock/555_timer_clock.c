const unsigned char num[10] =
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};


unsigned int ms = 0x0000;

unsigned char n = 0x00;
unsigned char i = 0x00;
unsigned char hr = 0x0A;
unsigned char min = 0x0A;
unsigned char sec = 0x0A;
unsigned char pos = 0x00;

bit toggle;
bit settings;


void timer0_ISR()
org IVT_ADDR_ET0
{
    TL0 = 0x17;
    TH0 = 0xFC;
    
    switch(pos)
    {
        case 0x00:
        {
            n = (hr / 0x0A);
            if((i == 0x01) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x00;
            break;
        }
        case 0x01:
        {
            n = (hr % 0x0A);
            if((i == 0x02) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x01;
            break;
        }
        case 0x02:
        {
            if(toggle)
            {
               P1 = 0x40;
            }
            else
            {
               P1 = 0x00;
            }
            P3 = 0x02;
            break;
        }
        case 0x03:
        {
            n = (min / 0x0A);
            if((i == 0x03) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x03;
            break;
        }
        case 0x04:
        {
            n = (min % 0x0A);
            if((i == 0x04) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x04;
            break;
        }
        case 0x05:
        {
            if(toggle)
            {
               P1 = 0x40;
            }
            else
            {
               P1 = 0x00;
            }
            P3 = 0x05;
            break;
        }
        case 0x06:
        {
            n = (sec / 0x0A);
            if((i == 0x05) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x06;
            break;
        }
        case 0x07:
        {
            n = (sec % 0x0A);
            if((i == 0x06) && toggle)
            {
               P1 = 0x00;
            }
            else
            {
               P1 = num[n];
            }
            P3 = 0x07;
            break;
        }
    }

    pos++;
    if(pos > 0x07)
    {
        pos = 0x00;
    }
}


void timer1_ISR()
org IVT_ADDR_ET1
{
    TL1 = 0xFF;
    TH1 = 0x7F;
    
    toggle = ~toggle;
    if(!settings)
    {
        sec++;
    
        if(sec > 59)
        {
            sec = 0;
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
}


void setup();
unsigned char read_keypad();


void main()
{
     bit key_pressed;
     unsigned char tmp = 0x00;
     
     setup();
     
     settings = 0;
     key_pressed = 0;

     while(1)
     {
         if(read_keypad() == 0x0F)
         {
            settings = 1;
            i = 1;
            delay_ms(900);
         }

         if(settings)
         {
            if((read_keypad() >= 0x00) && (read_keypad() <= 0x09) && (!key_pressed))
            {
               tmp = read_keypad();
               key_pressed = 1;
               delay_ms(40);
            }
            
            if(key_pressed)
            {
              switch(i)
              {
                 case 1:
                 {
                     hr = tmp;
                     hr *= 10;
                     break;
                 }
                 case 2:
                 {
                     hr += tmp;
                     break;
                 }
                 case 3:
                 {
                     min = tmp;
                     min *= 10;
                     break;
                 }
                 case 4:
                 {
                     min += tmp;
                     break;
                 }
                 case 5:
                 {
                     sec = tmp;
                     sec *= 10;
                     break;
                 }
                 case 6:
                 {
                     sec += tmp;
                     break;
                 }
              }
              
              i++;
              if(i > 6)
              {
                   settings = 0;
                   i = 0;
              }
              key_pressed = 0;
            }
         }
     };
}


void setup()
{
     P0 = 0xFF;
     P1 = 0xFF;
     P2 = 0xF0;
     P3 = 0xFF;

     TL0 = 0x00;
     TH0 = 0x00;

     TL1 = 0x17;
     TH1 = 0xFC;

     TMOD = 0x51;
     TCON = 0x50;
     IE = 0x8A;
}


unsigned char read_keypad()
{
    P2 = 0x7F;
    delay_ms(9);
    if((P2 & 0x0F) == 0x0E)
    {
        return 0x0A;
    }
    if((P2 & 0x0F) == 0x0D)
    {
        return 0x03;
    }
    if((P2 & 0x0F) == 0x0B)
    {
        return 0x02;
    }
    if((P2 & 0x0F) == 0x07)
    {
        return 0x01;
    }
    
    P2 = 0xBF;
    delay_ms(9);
    if((P2 & 0x0F) == 0x0E)
    {
        return 0x0B;
    }
    if((P2 & 0x0F) == 0x0D)
    {
        return 0x06;
    }
    if((P2 & 0x0F) == 0x0B)
    {
        return 0x05;
    }
    if((P2 & 0x0F) == 0x07)
    {
        return 0x04;
    }
    
    P2 = 0xDF;
    delay_ms(9);
    if((P2 & 0x0F) == 0x0E)
    {
        return 0x0C;
    }
    if((P2 & 0x0F) == 0x0D)
    {
        return 0x09;
    }
    if((P2 & 0x0F) == 0x0B)
    {
        return 0x08;
    }
    if((P2 & 0x0F) == 0x07)
    {
        return 0x07;
    }
    
    P2 = 0xEF;
    delay_ms(9);
    if((P2 & 0x0F) == 0x0E)
    {
        return 0x0D;
    }
    if((P2 & 0x0F) == 0x0D)
    {
        return 0x0F;
    }
    if((P2 & 0x0F) == 0x0B)
    {
        return 0x00;
    }
    if((P2 & 0x0F) == 0x07)
    {
        return 0x0E;
    }
    else
    {
        return 0xFF;
    }
}