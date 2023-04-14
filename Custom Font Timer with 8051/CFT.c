sbit LCD_RS at P2.B2;
sbit LCD_EN at P2.B3;

sbit LCD_D4 at P2.B4;
sbit LCD_D5 at P2.B5;
sbit LCD_D6 at P2.B6;
sbit LCD_D7 at P2.B7;

sbit Sound_Play_Pin at P1_4_bit;


const unsigned char character0[8] = {7, 15, 31, 31, 31, 31, 31, 0};
const unsigned char character1[8] = {31, 31, 31, 0, 0, 31, 31, 0};
const unsigned char character2[8] = {28, 30, 31, 31, 31, 31, 31, 0};
const unsigned char character3[8] = {31, 31, 0, 0, 0, 0, 0, 0};
const unsigned char character4[8] = {31, 31, 31, 31, 31, 15, 7, 0};
const unsigned char character5[8] = {0, 0, 0, 0, 0, 31, 31, 0};
const unsigned char character6[8] = {31, 31, 31, 31, 31, 30, 28, 0};
const unsigned char character7[8] = {31, 31, 31, 31, 31, 31, 31, 0};

unsigned int ms = 0;

signed char s = 0;
signed char min = 0;
signed char hr = 0;
signed char s_t = 0;
signed char min_t = 0;
signed char hr_t = 0;

bit up_down;
bit toggler;
bit set;
bit alarm;
bit stop_timer;


void timer_1_ISR()
org IVT_ADDR_ET1
{
    if(!stop_timer)
    {
          TH1 = 0xFC;
          TL1 = 0x16;
          ms++;
          if(ms > 999)
          {
                P3_0_bit ^= 0x1;
                toggler = ~toggler;
                ms = 0;
                if(up_down)
                {
                      s++;
                      if(s > 59)
                      {
                           s = 0;
                           min++;
                           if(min > 59)
                           {
                                  min = 0;
                                  hr++;
                                  if(hr > 23)
                                  {
                                        hr = 0;
                                  }
                           }
                      }
                }
                else
                {
                      s--;
                      if(s < 0)
                      {
                           s = 59;
                           min--;
                           if(min < 0)
                           {
                                  min = 59;
                                  hr--;
                                  if(hr < 0)
                                  {
                                        hr = 0;
                                  }
                           }
                      }
                }
          }
    }
}


void enable_timer()
{
     if(up_down)
     {
         s = 0;
         min = 0;
         hr = 0;
     }
     else
     {
         s = s_t;
         min = min_t;
         hr = hr_t;
     }
     ms = 0;
     EA_bit = 1;
     ET1_bit = 1;
     TR1_bit = 1;
     TH1 = 0xFC;
     TL1 = 0x16;
     set = 1;
     toggler = 1;
     stop_timer = 0;
     P3_0_bit = 1;
     P3_1_bit = 1;
}


void disable_timer()
{
     P3_0_bit = 0x1;
     toggler = 1;
     stop_timer = 1;
     set = 0;
     EA_bit = 0;
     ET1_bit = 0;
     TR1_bit = 0;
}


void LCD_load()
{
    unsigned char i = 0;
    unsigned char dchar = 0;

    for (dchar = 0; dchar < 8; dchar += 1)
    {

          LCD_Cmd(64 + (dchar * 8));
          for (i = 0; i < 8; i += 1)
          {
            switch(dchar)
            {
               case 0:
               {
                  LCD_Chr_Cp(character0[i]) ;
                  break;
               }
               case 1:
               {
                  LCD_Chr_Cp(character1[i]);
                  break;
               }
               case 2:
               {
                  LCD_Chr_Cp(character2[i]) ;
                  break;
               }
               case 3:
               {
                  LCD_Chr_Cp(character3[i]);
                  break;
               }
               case 4:
               {
                  LCD_Chr_Cp(character4[i]);
                  break;
               }
               case 5:
               {
                  LCD_Chr_Cp(character5[i]);
                  break;
               }
               case 6:
               {
                  LCD_Chr_Cp(character6[i]);
                  break;
               }
               case 7:
               {
                  LCD_Chr_Cp(character7[i]);
                  break;
               }
            }
          }
    }
}


void setup()
{
      P1 = 0x0F;
      P2 = 0xFF;
      disable_timer();
      C_T1_bit = 0;
      M11_bit = 0;
      M01_bit = 1;
      GATE1_bit = 0;
      TH1 = 0xFC;
      TL1 = 0x16;
      up_down = 1;
      toggler = 1;
      alarm = 0;
      Lcd_Init();
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Cmd(_LCD_CURSOR_OFF);
      Sound_Init();
      LCD_load();
      P3_0_bit = 0;
      delay_ms(400);
      P3_0_bit = 1;
}


void custom_graphics(unsigned char number, unsigned char pos)
{
      unsigned char x_pos = 0;

      switch(pos)
      {
            case 1:
            {
                x_pos = 1;
                break;
            }
            case 2:
            {
                x_pos = 5;
                break;
            }
            case 3:
            {
                x_pos = 10;
                break;
            }
            case 4:
            {
                x_pos = 14;
                break;
            }
      }

      switch(number)
      {
            case 0:
            {
                lcd_chr(1, x_pos, 0);
                lcd_chr(1, (x_pos + 1), 3);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_chr(2, x_pos, 4);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 6);
                break;
            }
            case 1:
            {
                lcd_chr(1, x_pos, 3);
                lcd_chr(1, (x_pos + 1), 2);
                lcd_out(1, (x_pos + 2), " ");
                lcd_chr(2, x_pos, 5);
                lcd_chr(2, (x_pos + 1), 7);
                lcd_chr(2, (x_pos + 2), 5);
                break;
            }
            case 2:
            {
                lcd_chr(1, x_pos, 1);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_chr(2, x_pos, 4);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 5);
                break;
            }
            case 3:
            {
                lcd_chr(1, x_pos, 1);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_chr(2, x_pos, 5);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 6);
                break;
            }
            case 4:
            {
                lcd_chr(1, x_pos, 4);
                lcd_chr(1, (x_pos + 1), 5);
                lcd_chr(1, (x_pos + 2), 7);
                lcd_out(2, x_pos, " ");
                lcd_out(2, (x_pos + 1), " ");
                lcd_chr(2, (x_pos + 2), 7);
                break;
            }
            case 5:
            {
                lcd_chr(1, x_pos, 4);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 1);
                lcd_chr(2, x_pos, 5);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 6);
                break;
            }
            case 6:
            {
                lcd_chr(1, x_pos, 0);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 1);
                lcd_chr(2, x_pos, 4);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 6);
                break;
            }
            case 7:
            {
                lcd_chr(1, x_pos, 3);
                lcd_chr(1, (x_pos + 1), 3);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_out(2, x_pos, " ");
                lcd_out(2, (x_pos + 1), " ");
                lcd_chr(2, (x_pos + 2), 7);
                break;
            }
            case 8:
            {
                lcd_chr(1, x_pos, 0);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_chr(2, x_pos, 4);
                lcd_chr(2, (x_pos + 1), 5);
                lcd_chr(2, (x_pos + 2), 6);
                break;
            }
            case 9:
            {
                lcd_chr(1, x_pos, 0);
                lcd_chr(1, (x_pos + 1), 1);
                lcd_chr(1, (x_pos + 2), 2);
                lcd_out(2, x_pos, " ");
                lcd_out(2, (x_pos + 1), " ");
                lcd_chr(2, (x_pos + 2), 7);
                break;
            }
      }
}


unsigned short scan_kbd()
{
      if(P1_0_bit == 0)
      {
          Sound_Play(2600, 45);
          return 1;
      }
      else if(P1_1_bit == 0)
      {
          Sound_Play(3000, 45);
          return 2;
      }
      else if(P1_2_bit == 0)
      {
          Sound_Play(2200, 45);
          return 3;
      }
      else
      {
          return 0;
      }
}


void display_common(unsigned char x, unsigned char y, signed char convert)
{
    unsigned char t = 0;
    t = (convert / 10);
    Lcd_Chr(y, x, (48 + t));
    t = (convert % 10);
    Lcd_Chr(y, (++x),(48 + t));
}


unsigned char changes(unsigned char x, unsigned char y, signed char value, signed char max, signed char min)
{

    while(1)
    {
          display_common(x, y, value);
          if(scan_kbd() == 2)
          {
              value++;
          }
          if(scan_kbd() == 3)
          {
              value--;
          }
          if(value > max)
          {
              value = min;
          }
          if(value < min)
          {
              value = max;
          }
          if(scan_kbd() == 1)
          {
              delay_ms(9);
              while(scan_kbd() == 1);
              return value;
          }
    }
}


void settings()
{
     if(scan_kbd()==1)
     {
          delay_ms(40);
          if(scan_kbd() == 1)
          {
               disable_timer();
               Lcd_Cmd(_LCD_CLEAR);
               while(1)
               {
                   lcd_out(1, 4, "<Set Time>");
                   lcd_out(2, 5, "--:--:--");
                   delay_ms(200);
                   display_common(5, 2, hr_t);
                   hr_t = changes(5, 2, hr_t, 23, 0);
                   delay_ms(200);
                   display_common(8, 2, min_t);
                   min_t = changes(8, 2, min_t, 59, 0);
                   delay_ms(200);
                   display_common(11, 2, s_t);
                   s_t = changes(11, 2, s_t, 59, 0);
                   delay_ms(200);
                   Lcd_Cmd(_LCD_CLEAR);
                   lcd_out(1, 1, "01 = U / 00 = D");
                   lcd_out(2, 5, "Type --");
                   display_common(10, 2, up_down);
                   up_down = changes(10, 2, up_down, 1, 0);
                   delay_ms(200);
                   break;
               }
               enable_timer();
               Lcd_Cmd(_LCD_CLEAR);
          }
     }
}


void display_numbers(unsigned char data1, unsigned char data2)
{
      unsigned char disp1 = 0;
      unsigned char disp2 = 0;
      disp1=(data1 / 10);
      disp2=(data1 % 10);
      custom_graphics(disp1, 3);
      custom_graphics(disp2, 4);
      disp1=(data2 / 10);
      disp2=(data2 % 10);
      custom_graphics(disp1, 1);
      custom_graphics(disp2, 2);
      if(toggler)
      {
          lcd_out(1, 8, ".");
          lcd_out(2, 8, ".");
      }
      else
      {
          lcd_out(1, 8, " ");
          lcd_out(2, 8, " ");
      }
}


void timer_tasks()
{
     if(set)
     {
         if(up_down)
         {
              if((s == s_t) && (min == min_t) && (hr == hr_t))
              {
                  disable_timer();
                  alarm = 1;
              }
         }
         else
         {
              if((s == 0) && (min == 0) && (hr == 0))
              {
                  disable_timer();
                  alarm = 1;
              }
         }
     }
     else
     {
         disable_timer();
     }
}


void time_out()
{
     unsigned char i = 0;
     
     for(i = 0; i < 9; i += 1)
     {
         P3_1_bit ^= 0x1;
         Sound_Play(600, 45);
         delay_ms(600);
     }
     P3_1_bit = 0;
}


void display()
{
     if(hr == 0)
     {
         display_numbers(s, min);
     }
     else
     {
         display_numbers(min, hr);
     }
     
     if(alarm)
     {
        time_out();
        alarm = 0;
     }
}


void main()
{
     setup();
     while(1)
     {
         settings();
         timer_tasks();
         display();
         delay_ms(100);
     };
}