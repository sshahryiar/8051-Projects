#include "ADS1115.c"

#define v_max              6.144


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


void setup();
void load_fixed_lcd_texts();
void lcd_print_c(unsigned char x_pos, unsigned char y_pos, signed int value);
void lcd_print_i(unsigned char x_pos, unsigned char y_pos, signed long value);
void lcd_print_f(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points);
void lcd_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned int value, unsigned char points);


void main() 
{
    unsigned char s = 0x00;
    unsigned int channel = 0x00;
    signed int cnt = 0x0000;
    float volt = 0.0;
    
    setup();
    
    load_fixed_lcd_texts();
    
    while(1)
    {
        for(s = 0; s < 4; s++)
        {
            switch(s)
            {
                case 1:
                {
                    channel = AINP_AIN1__AINN_GND;
                    break;
                }
                case 2:
                {
                    channel = AINP_AIN2__AINN_GND;
                    break;
                }
                case 3:
                {
                    channel = AINP_AIN3__AINN_GND;
                    break;
                }
                default:
                {
                    channel = AINP_AIN0__AINN_GND;
                    break;
                }
            }
            
            ADS1115_configure(start_one_conversion | channel | FS_6144mV | power_down_single_shot_mode | data_rate_860SPS | disable_comparator);
            delay_ms(1);
            cnt = ADS1115_read(ADS1115_conversion_reg_pointer);
            volt = cnt;
            volt *= v_max;
            volt /= ((float)ADC_count_max);
            lcd_print_i(4, (s + 1), cnt);
            lcd_print_f(13, (s + 1), volt, 4);
        }
        delay_ms(400);
    };
}


void setup()
{
    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;

    P2_5_bit = 0;
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    
    ADS1115_init();
}


void load_fixed_lcd_texts()
{
    unsigned char s = 0;
    
    for(s = 0; s < 4; s++)
    {
        lcd_out((1 + s), 1, "A :");
        lcd_chr((1 + s), 2, (0x30 + s));
        lcd_out((1 + s), 20, "V");
    }
}


void lcd_print_c(unsigned char x_pos, unsigned char y_pos, signed int value)
{
    unsigned char ch = 0x00;

    if(value < 0)
    {
        value = -value;
        lcd_out(y_pos, x_pos, "-");
    }
    else
    {
        lcd_out(y_pos, x_pos, " ");
    }

    if((value > 99) && (value <= 999))
    {
        ch = ((value / 100) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = (((value / 10) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 3), ch);
    }
    else if((value > 9) && (value <= 99))
    {
        ch = ((value / 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        lcd_chr(y_pos, (x_pos + 3), 0x20);
    }
    else if((value >= 0) && (value <= 9))
    {
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        lcd_chr(y_pos, (x_pos + 2), 0x20);
        lcd_chr(y_pos, (x_pos + 3), 0x20);
    }
}


void lcd_print_i(unsigned char x_pos, unsigned char y_pos, signed long value)
{
    unsigned char ch = 0x00;

    if(value < 0)
    {
        value = -value;
        lcd_out(y_pos, x_pos, "-");
    }
    else
    {
        lcd_out(y_pos, x_pos, " ");
    }

    if((value > 9999) && (value <= 99999))
    {
        ch = ((value / 10000) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = (((value / 1000) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        ch = (((value / 100) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 3), ch);
        ch = (((value / 10) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 4), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 5), ch);
    }
    else if((value > 999) && (value <= 9999))
    {
        ch = ((value / 1000) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = (((value / 100) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        ch = (((value / 10) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 3), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 4), ch);
        lcd_chr(y_pos, (x_pos + 5), 0x20);
    }
    else if((value > 99) && (value <= 999))
    {
        ch = ((value / 100) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = (((value / 10) % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 3), ch);
        lcd_chr(y_pos, (x_pos + 4), 0x20);
        lcd_chr(y_pos, (x_pos + 5), 0x20);
    }
    else if((value > 9) && (value <= 99))
    {
        ch = ((value / 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);
        lcd_chr(y_pos, (x_pos + 3), 0x20);
        lcd_chr(y_pos, (x_pos + 4), 0x20);
        lcd_chr(y_pos, (x_pos + 5), 0x20);
    }
    else if((value > 0) && (value <= 9))
    {
        ch = ((value % 10) + 0x30);
        lcd_chr(y_pos, (x_pos + 1), ch);
        lcd_chr(y_pos, (x_pos + 2), 0x20);
        lcd_chr(y_pos, (x_pos + 3), 0x20);
        lcd_chr(y_pos, (x_pos + 4), 0x20);
        lcd_chr(y_pos, (x_pos + 5), 0x20);
    }
}


void lcd_print_f(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points)
{
    signed long tmp = 0;

    tmp = ((signed long)value);
    lcd_print_i(x_pos, y_pos, tmp);
    tmp = (10000 * (value - tmp));

    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if((value >= 10000) && (value < 100000))
    {
        lcd_print_decimal((x_pos + 6), y_pos, tmp, points);
    }
    else if((value >= 1000) && (value < 10000))
    {
        lcd_print_decimal((x_pos + 5), y_pos, tmp, points);
    }
    else if((value >= 100) && (value < 1000))
    {
        lcd_print_decimal((x_pos + 4), y_pos, tmp, points);
    }
    else if((value >= 10) && (value < 100))
    {
        lcd_print_decimal((x_pos + 3), y_pos, tmp, points);
    }
    else if(value < 10)
    {
        lcd_print_decimal((x_pos + 2), y_pos, tmp, points);

        if(value < 0)
        {
            lcd_out(y_pos, x_pos, "-");
            lcd_chr(y_pos, (x_pos + 1), 0x30);
        }
        else
        {
            lcd_out(y_pos, x_pos, " ");
            if(value < 1)
            {
               lcd_chr(y_pos, (x_pos + 1), 0x30);
            }
        }
    }
}


void lcd_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned int value, unsigned char points)
{
    unsigned char ch = 0x00;

    lcd_chr(y_pos, x_pos, 0x2E);

    ch = ((value / 1000) + 0x30);
    lcd_chr(y_pos, (x_pos + 1), ch);

    if(points > 1)
    {
        ch = (((value % 1000) / 100) + 0x30);
        lcd_chr(y_pos, (x_pos + 2), ch);

        if(points > 2)
        {
            ch = (((value % 100) / 10) + 0x30);
            lcd_chr(y_pos, (x_pos + 3), ch);

            if(points > 3)
            {
                ch = ((value % 10) + 0x30);
                lcd_chr(y_pos, (x_pos + 4), ch);
            }
        }
    }
}