#include "DS1620.c"


#define menu_inc           P2_0_bit
#define menu_dec           P2_1_bit
#define inc                P2_2_bit
#define dec                P2_3_bit


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


const unsigned char degree_symbol[8] = {0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00};
const unsigned char arrow_symbol[8] = {0x10, 0x18, 0x1C, 0x1E, 0x1C, 0x18, 0x10, 0x00};
const unsigned char notify_symbol[8] = {0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x00};


void setup();
void LCD_load();
void lcd_print(unsigned char x_pos, unsigned char y_pos, signed int value);
void show_temperature(unsigned char x_pos, unsigned char y_pos, signed char m, unsigned char f);
signed char change_T(signed char value);


void main() 
{
     bit read_set_points;
     
     signed char tm = 0x00;
     signed char TL = 0x00;
     signed char TH = 0x00;
     
     unsigned char tf = 0x00;
     unsigned char menu = 0x00;
     
     setup();

     lcd_out(1, 2, "T.Curr/ C:");
     lcd_chr(1, 9, 0);
     lcd_out(2, 2, "T.High/ C:");
     lcd_chr(2, 9, 0);
     lcd_out(3, 2, "T.Low / C:");
     lcd_chr(3, 9, 0);
     lcd_out(4, 2, "Alarms:   TH   TL");
     
     read_set_points = 1;

     DS1620_start_conv();
     
     while(1)
     {
             DS1620_read_T(&tm, &tf);
             show_temperature(13, 1, tm, tf);
             
             if(menu_inc == 0)
             {
                 delay_ms(60);
                 while(menu_inc == 0);
                 menu++;
             }
             
             if(menu > 3)
             {
                 menu = 0;
             }
             
             if(menu_dec == 0)
             {
                 delay_ms(60);
                 while(menu_dec == 0);
                 menu--;
             }
             
             if(menu < 1)
             {
                 menu = 0;
             }
             
             switch(menu)
             {
                 case 1:
                 {
                     lcd_chr(2, 1, 1);
                     lcd_chr(3, 1, 0x20);
                     lcd_chr(4, 1, 0x20);
                     TH = change_T(TH);
                     break;
                 }
                 case 2:
                 {
                     lcd_chr(3, 1, 1);
                     lcd_chr(2, 1, 0x20);
                     lcd_chr(4, 1, 0x20);
                     TL = change_T(TL);
                     break;
                 }
                 case 3:
                 {
                     lcd_chr(4, 1, 1);
                     lcd_chr(2, 1, 0x20);
                     lcd_chr(3, 1, 0x20);
                     lcd_chr(4, 11, 0x20);
                     lcd_chr(4, 16, 0x20);
                     DS1620_write_TH(TH);
                     DS1620_write_TL(TL);
                     DS1620_write_config(Reserved | CPU);
                     read_set_points = 1;
                     menu = 0;
                     break;
                 }
                 default:
                 {
                     lcd_chr(2, 1, 0x20);
                     lcd_chr(3, 1, 0x20);
                     lcd_chr(4, 1, 0x20);
                     break;
                 }
             }
             
             if(read_set_points)
             {
                 TH = DS1620_read_TH();
                 TL = DS1620_read_TL();
                 read_set_points = 0;
             }
             
             if((DS1620_read_config() & THF) == THF)
             {
                 lcd_chr(4, 11, 2);
             }
             else
             {
                 lcd_chr(4, 11, 0x20);
             }
             
             if((DS1620_read_config() & TLF) == TLF)
             {
                 lcd_chr(4, 16, 2);
             }
             else
             {
                 lcd_chr(4, 16, 0x20);
             }
             
             lcd_print(13, 2, TH);
             lcd_print(13, 3, TL);
     };
}


void setup()
{
     P2 = 0x0F;
     P2_5_bit = 0;
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     LCD_load();
     DS1620_init();
}


void LCD_load()
{
    unsigned char i = 0;
    unsigned char dchar = 0;

    for (dchar = 0; dchar < 3; dchar++)
    {

          LCD_Cmd(64 + (dchar << 3));
          for (i = 0; i < 8; i++)
          {
            switch(dchar)
            {
               case 1:
               {
                  LCD_Chr_Cp(arrow_symbol[i]);
                  break;
               }
               case 2:
               {
                  LCD_Chr_Cp(notify_symbol[i]);
                  break;
               }
               default:
               {
                  LCD_Chr_Cp(degree_symbol[i]) ;
                  break;
               }
            }
          }
    }
}


void lcd_print(unsigned char x_pos, unsigned char y_pos, signed int value)
{
    unsigned char ch = 0x00;
    
    if(value < 0x00)
    {

        lcd_chr(y_pos, x_pos, 0x2D);
        value = -value;
    }
    else
    {
        lcd_chr(y_pos, x_pos, 0x20);
    }

     if((value > 99) && (value <= 999))
     {
         ch = ((value / 100) + 0x30);
         lcd_chr_cp(ch);
         ch = (((value % 100) / 10) + 0x30);
         lcd_chr_cp(ch);
         ch = ((value % 10) + 0x30);
         lcd_chr_cp(ch);
     }
     else if((value > 9) && (value <= 99))
     {
         lcd_chr_cp(0x20);
         ch = (((value % 100) / 10) + 0x30);
         lcd_chr_cp(ch);
         ch = ((value % 10) + 0x30);
         lcd_chr_cp(ch);
     }
     else if((value >= 0) && (value <= 9))
     {
         lcd_chr_cp(0x20);
         lcd_chr_cp(0x20);
         ch = ((value % 10) + 0x30);
         lcd_chr_cp(ch);
     }
}


void show_temperature(unsigned char x_pos, unsigned char y_pos, signed char m, unsigned char f)
{
    lcd_print(x_pos, y_pos, m);
    lcd_chr(y_pos, (x_pos + 4), 0x2E);
    lcd_chr(y_pos, (x_pos + 5), (0x30 + f));
}


signed char change_T(signed char value)
{
    if(inc == 0)
    {
        delay_ms(40);
        value++;
    }
    
    if(value > max_T)
    {
        value = min_T;
    }
    
    if(dec == 0)
    {
        delay_ms(40);
        value--;
    }

    if(value < min_T)
    {
        value = max_T;
    }
    
    return value;
}