#include "ST7565.h"
#include "font.c"


void GLCD_write(unsigned char value, unsigned char type)
{
    GLCD_CS = 0;
    GLCD_RD = 1;
    GLCD_RS = (type & 0x01);
    GLCD_RW = 0;
    delay_us(1);
    GLCD_data_port = value;
    delay_us(1);
    GLCD_RW = 1;
}


void GLCD_init()
{
    unsigned char s = 0;
    GLCD_RST = 0;
    GLCD_Write(NOP_cmd, CMD);
    delay_us(90);
    GLCD_CS = 0;
    GLCD_RST = 1;

    GLCD_Write(Internal_Reset, CMD);
    GLCD_Write(NOP_cmd, CMD);
    delay_us(90);

    GLCD_Write(ADC_Select_Normal, CMD);
    GLCD_Write(Set_Common_Output_Reverse, CMD);
    GLCD_Write(Display_Normal, CMD);
    GLCD_Write(Display_All_Points_Off, CMD);
    GLCD_Write(Set_LCD_Bias_Low, CMD);
    GLCD_Write(Set_Boost_Ratio_Medium, CMD);
    GLCD_Write(Set_Electronic_Volume_Mode, CMD);
    GLCD_Write(0x23, CMD);
    GLCD_Write((Select_Internal_Resistor_Ratio | 0x05), CMD);
    GLCD_Write((Select_Internal_Power_Supply_Mode | 0x07), CMD);
    GLCD_Write(NOP_cmd, CMD);
    delay_us(90);
    
    GLCD_Write(Set_LCD_Start_Line, CMD);
    GLCD_Write(Display_On, CMD);
    GLCD_Write(NOP_cmd, CMD);
    delay_us(90);
}


void GLCD_fill(unsigned char value)
{
    unsigned char i = 0x00;
    unsigned char j = 0x00;

    for(i = row_min; i < row_max; i++)
    {
         GLCD_Write((i + 0xB0), CMD);
         GLCD_Write(0x10, CMD);
         GLCD_Write(0x04, CMD);

            for(j = x_min; j < x_max; j++)
            {
               GLCD_Write(value, DAT);
            }
    }
}


void GLCD_draw_bmp(unsigned char *bmp_data)
{
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned int k = 0;

    for(i = row_min; i < row_max; i++)
    {
         GLCD_Write(Set_LCD_Start_Line, CMD);
         GLCD_Write((i + Set_Page_Address), CMD);
         GLCD_Write(Set_Column_Address_High, CMD);
         GLCD_Write(Set_Column_Address_Low, CMD);

          for(j = x_min; j < x_max; j++)
          {
             GLCD_Write(bmp_data[k++], DAT);
          }
    }
}


void GLCD_clear_screen()
{
     GLCD_fill(0x00);
}


void GLCD_clear_buffer()
{
     unsigned int s = 0x0000;

     for(s = 0; s < buffer_size; s++)
     {
          buffer[s] = 0x00;
     }
}


void GLCD_gotoxy(unsigned char x_pos, unsigned char y_pos)
{
     unsigned char xl = 0x00;
     unsigned char xh = 0x00;
     
     y_pos += Set_Page_Address;
     GLCD_write(y_pos, CMD);
     
     xh = ((x_pos >> 4) & 0x0F);
     xh |= Set_Column_Address_High;
     GLCD_write(xh, CMD);
     xl = (x_pos & 0x0F);
     xl |= Set_Column_Address_Low;
     GLCD_write(xl, CMD);
}


void GLCD_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch)
{
    unsigned char chr = 0x00;
    unsigned char s = 0x00;

    chr = (ch - 32);

    if(x_pos > (x_max - 0x06))
    {
        x_pos = 0;
        y_pos++;
    }
    GLCD_gotoxy(x_pos, y_pos);

    for(s = 0x00; s < 0x06; s++)
    {
        GLCD_write(font_regular[chr][s], DAT);
    }
}


void GLCD_print_string(unsigned char x_pos, unsigned char y_pos, unsigned char *ch)
{
    unsigned char chr = 0x00;
    unsigned char i = 0x00;
    unsigned char j = 0x00;

    while(ch[j] != '\0')
    {
        chr = (ch[j] - 32);

        if(x_pos > (x_max - 0x06))
        {
            x_pos = 0x00;
            y_pos++;
        }
        GLCD_gotoxy(x_pos, y_pos);

        for(i = 0x00; i < 0x06; i++)
        {
            GLCD_write(font_regular[chr][i], DAT);
        }

        j++;
        x_pos += 6;
     }
}


void GLCD_print_chr(unsigned char x_pos, unsigned char y_pos, signed int value)
{
    unsigned char ch = 0x00;

    if(value < 0x00)
    {
        GLCD_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        GLCD_print_char(x_pos, y_pos,' ');
    }

    if((value > 99) && (value <= 999))
    {
       ch = (value / 100);
       GLCD_print_char((x_pos + 6), y_pos , (48 + ch));
       ch = ((value % 100) / 10);
       GLCD_print_char((x_pos + 12), y_pos , (48 + ch));
       ch = (value % 10);
       GLCD_print_char((x_pos + 18), y_pos , (48 + ch));
    }
    else if((value > 9) && (value <= 99))
    {
       ch = ((value % 100) / 10);
       GLCD_print_char((x_pos + 6), y_pos , (48 + ch));
       ch = (value % 10);
       GLCD_print_char((x_pos + 12), y_pos , (48 + ch));
       GLCD_print_char((x_pos + 18), y_pos , 32);
    }
    else if((value >= 0) && (value <= 9))
    {
       ch = (value % 10);
       GLCD_print_char((x_pos + 6), y_pos , (48 + ch));
       GLCD_print_char((x_pos + 12), y_pos , 32);
       GLCD_print_char((x_pos + 18), y_pos , 32);
    }
}


void GLCD_print_int(unsigned char x_pos, unsigned char y_pos, signed long value)
{
    unsigned char ch = 0x00;

    if(value < 0)
    {
        GLCD_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        GLCD_print_char(x_pos, y_pos,' ');
    }

    if(value > 9999)
    {
        ch = (value / 10000);
        GLCD_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 10000)/ 1000);
        GLCD_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = ((value % 1000) / 100);
        GLCD_print_char((x_pos + 18), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        GLCD_print_char((x_pos + 24), y_pos , (48 + ch));

        ch = (value % 10);
        GLCD_print_char((x_pos + 30), y_pos , (48 + ch));
    }

    else if((value > 999) && (value <= 9999))
    {
        ch = ((value % 10000)/ 1000);
        GLCD_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 1000) / 100);
        GLCD_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        GLCD_print_char((x_pos + 18), y_pos , (48 + ch));

        ch = (value % 10);
        GLCD_print_char((x_pos + 24), y_pos , (48 + ch));
        GLCD_print_char((x_pos + 30), y_pos , 32);
    }
    else if((value > 99) && (value <= 999))
    {
        ch = ((value % 1000) / 100);
        GLCD_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        GLCD_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = (value % 10);
        GLCD_print_char((x_pos + 18), y_pos , (48 + ch));
        GLCD_print_char((x_pos + 24), y_pos , 32);
        GLCD_print_char((x_pos + 30), y_pos , 32);
    }
    else if((value > 9) && (value <= 99))
    {
        ch = ((value % 100) / 10);
        GLCD_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = (value % 10);
        GLCD_print_char((x_pos + 12), y_pos , (48 + ch));

        GLCD_print_char((x_pos + 18), y_pos , 32);
        GLCD_print_char((x_pos + 24), y_pos , 32);
        GLCD_print_char((x_pos + 30), y_pos , 32);
    }
    else
    {
        ch = (value % 10);
        GLCD_print_char((x_pos + 6), y_pos , (48 + ch));
        GLCD_print_char((x_pos + 12), y_pos , 32);
        GLCD_print_char((x_pos + 18), y_pos , 32);
        GLCD_print_char((x_pos + 24), y_pos , 32);
        GLCD_print_char((x_pos + 30), y_pos , 32);
    }
}


void GLCD_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned int value, unsigned char points)
{
    unsigned char ch = 0x00;

    GLCD_print_char(x_pos, y_pos, '.');

    ch = (value / 1000);
    GLCD_print_char((x_pos + 6), y_pos , (48 + ch));

    if(points > 1)
    {
        ch = ((value % 1000) / 100);
        GLCD_print_char((x_pos + 12), y_pos , (48 + ch));


        if(points > 2)
        {
            ch = ((value % 100) / 10);
            GLCD_print_char((x_pos + 18), y_pos , (48 + ch));

            if(points > 3)
            {
                ch = (value % 10);
                GLCD_print_char((x_pos + 24), y_pos , (48 + ch));
            }
        }
    }
}


void GLCD_print_float(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points)
{
    signed long tmp = 0x00;

    tmp = value;
    GLCD_print_int(x_pos, y_pos, tmp);
    tmp = ((value - tmp) * 10000);

    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if((value >= 9999) && (value < 99999))
    {
        GLCD_print_decimal((x_pos + 36), y_pos, tmp, points);
    }
    else if((value >= 999) && (value < 9999))
    {
        GLCD_print_decimal((x_pos + 30), y_pos, tmp, points);
    }
    else if((value >= 99) && (value < 999))
    {
        GLCD_print_decimal((x_pos + 24), y_pos, tmp, points);
    }
    else if((value >= 9) && (value < 99))
    {
        GLCD_print_decimal((x_pos + 18), y_pos, tmp, points);
    }
    else if(value < 9)
    {
        GLCD_print_decimal((x_pos + 12), y_pos, tmp, points);
        if((value) < 0)
        {
            GLCD_print_char(x_pos, y_pos, '-');
        }
        else
        {
            GLCD_print_char(x_pos, y_pos, ' ');
        }
    }
}


void GLCD_set_pixel(unsigned char x_pos, unsigned char y_pos, unsigned char colour)
{
    unsigned char value = 0x00;
    unsigned char page = 0x00;
    unsigned char bit_pos = 0x00;

    page = (y_pos / y_max);
    bit_pos = (y_pos - (page * y_max));
    value = buffer[((page * x_max) + x_pos)];

    if((colour & 0x01) != 0)
    {
        value |= (1 << bit_pos);
    }
    else
    {
        value &= (~(1 << bit_pos));
    }

    buffer[((page * x_max) + x_pos)] = value;
    GLCD_gotoxy(x_pos, page);
    GLCD_write(value, DAT);
}


void GLCD_line(signed int x1, signed char y1, signed int x2, signed char y2, unsigned char colour)
{
    signed int dx = 0x0000;
    signed int dy = 0x0000;
    signed int stepx = 0x0000;
    signed int stepy = 0x0000;
    signed int fraction = 0x0000;

    dy = (y2 - y1);
    dx = (x2 - x1);

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    GLCD_set_pixel(x1, y1, colour);

    if (dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while (x1 != x2)
        {
            if (fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            GLCD_set_pixel(x1, y1, colour);
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));
        while (y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            GLCD_set_pixel(x1, y1, colour);
        }
    }
}


void GLCD_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char colour, unsigned char type)
{
     unsigned short i = 0x00;
     unsigned short xmin = 0x00;
     unsigned short xmax = 0x00;
     unsigned short ymin = 0x00;
     unsigned short ymax = 0x00;

     if(fill != 0)
     {
        if(x1 < x2)
        {
           xmin = x1;
           xmax = x2;
        }
        else
        {
           xmin = x2;
           xmax = x1;
        }

        if(y1 < y2)
        {
           ymin = y1;
           ymax = y2;
        }
        else
        {
           ymin = y2;
           ymax = y1;
        }

        for(; xmin <= xmax; ++xmin)
        {
           for(i = ymin; i <= ymax; ++i)
           {
               GLCD_set_pixel(x1, y1, colour);
           }
         }
     }

     else
     {
        GLCD_line(x1, y1, x2, y1, colour);
        GLCD_line(x1, y2, x2, y2, colour);
        GLCD_line(x1, y1, x1, y2, colour);
        GLCD_line(x2, y1, x2, y2, colour);
     }

     if(type != square)
     {
         GLCD_set_pixel(x1, y1, ~colour);
         GLCD_set_pixel(x1, y2, ~colour);
         GLCD_set_pixel(x2, y1, ~colour);
         GLCD_set_pixel(x2, y2, ~colour);
     }
}


void GLCD_circle(unsigned char xc, unsigned char yc, unsigned char radius, unsigned char fill, unsigned char colour)
{
   signed int a = 0x0000;
   signed int b = 0x0000;
   signed int P = 0x0000;

   b = radius;
   P = (1 - b);

   do
   {
        if(fill != 0)
        {
           GLCD_line((xc - a), (yc + b), (xc + a), (yc + b), colour);
           GLCD_line((xc - a), (yc - b), (xc + a), (yc - b), colour);
           GLCD_line((xc - b), (yc + a), (xc + b), (yc + a), colour);
           GLCD_line((xc - b), (yc - a), (xc + b), (yc - a), colour);
        }
        else
        {
           GLCD_set_pixel((xc + a), (yc + b), colour);
           GLCD_set_pixel((xc + b), (yc + a), colour);
           GLCD_set_pixel((xc - a), (yc + b), colour);
           GLCD_set_pixel((xc - b), (yc + a), colour);
           GLCD_set_pixel((xc + b), (yc - a), colour);
           GLCD_set_pixel((xc + a), (yc - b), colour);
           GLCD_set_pixel((xc - a), (yc - b), colour);
           GLCD_set_pixel((xc - b), (yc - a), colour);
        }

        if(P < 0)
        {
           P += (3 + (2 * a++));
        }
        else
        {
           P += (5 + (2 * ((a++) - (b--))));
        }
    }while(a <= b);
}