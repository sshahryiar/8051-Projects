#include "PCF8591.c"


#define scale_value        0.01960784314
#define ADC_Max                    255.0
#define RL                         100.0
#define LDR_Constant               500.0
#define B                         3950.0
#define RT                       10000.0
#define _25C_K                     298.0


sbit LCD_RS at P2_6_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;


const unsigned char symbol[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};


void setup();
void fixed_lcd_items();
void load_symbol(unsigned char pos_row, unsigned char pos_char);
void lcd_show_value(unsigned char x, unsigned char y, float value);
void show_adc_value(unsigned char y_pos, unsigned char value);


void main() 
{
     unsigned char adc0 = 0;
     unsigned char adc1 = 0;
     unsigned char adc2 = 0;
     unsigned char adc3 = 0;
     
     float lux = 0.0;
     float t = 0.0;
     float R_inf = 0.0;
     float V = 0.0;
     
     setup();
     fixed_lcd_items();
     
     R_inf = (-1.0 * (B / _25C_K));
     R_inf = exp(R_inf);
     R_inf *= RT;
     
     while(1)
     {
         adc0 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN0);
         adc1 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN1);
         adc2 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN2);
         adc3 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN3);
         
         t = (ADC_Max / adc1);
         t -= 1;
         t *= RT;
         t /= R_inf;
         t = (B / (log(t)));
         t = (t - 273.0);
         
         lux = (ADC_Max / adc2);
         lux -= 1;
         lux *= RL;
         lux = (LDR_Constant / lux);
         
         show_adc_value(1, adc0);
         show_adc_value(2, adc1);
         show_adc_value(3, adc2);
         show_adc_value(4, adc3);
         
         lcd_show_value(16, 1, ((float)(adc0 * scale_value)));
         lcd_show_value(16, 2, t);;
         lcd_show_value(16, 3, lux);
         lcd_show_value(16, 4, ((float)(adc3 * scale_value)));
         
         PCF8591_write(AOut_enable, adc0);
         
         delay_ms(400);
     };
}


void setup()
{
     P0 = 0x00;
     P2_5_bit = 0;
     Soft_I2C_Init();
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
}


void fixed_lcd_items()
{
    LCD_Out(1, 1, "A0:");
    LCD_Out(2, 1, "A1:");
    LCD_Out(3, 1, "A2:");
    LCD_Out(4, 1, "A3:");
    
    LCD_Out(1, 9, "DAC/V:");
    LCD_Out(2, 9, "T/ C:");
    LCD_Out(3, 9, "Lux:");
    LCD_Out(4, 9, "Vin/V:");
    load_symbol(2, 11);
    
}


void load_symbol(unsigned char pos_row, unsigned char pos_char)
{
    unsigned char i = 0;
    Lcd_Cmd(64);
    for (i = 0; i <= 7; i++)
    {
        Lcd_Chr_CP(symbol[i]);
    }
    Lcd_Cmd(_LCD_RETURN_HOME);
    Lcd_Chr(pos_row, pos_char, 0);
}


void lcd_show_value(unsigned char x, unsigned char y, float value)
{
     unsigned int tmp = 0;
     unsigned char ch = 0;

     if(value < 0)
     {
         value *= -1;
         LCD_out(y, (x - 1), "-");
     }
     else
     {
         LCD_out(y, (x - 1), " ");
     }

     if((value >= 0) && (value < 10))
     {
         tmp = (value * 1000);
     }
     if((value >= 10) && (value < 100))
     {
         tmp = (value * 100);
     }
     if((value >= 100) && (value < 1000))
     {
         tmp = (value * 10);
     }
     if((value >= 1000) && (value < 10000))
     {
         tmp = value;
     }

     ch = (tmp / 1000);
     LCD_Chr(y, x, (48 + ch));

     if((value >= 0) && (value < 10))
     {
        LCD_Chr_CP(46);
     }
     ch = (tmp / 100) % 10;
     LCD_Chr_CP(48 + ch);

     if((value >= 10) && (value < 100))
     {
        LCD_Chr_CP(46);
     }
     ch = (tmp / 10) % 10;
     LCD_Chr_CP(48 + ch);

     if((value >= 100) && (value < 1000))
     {
        LCD_Chr_CP(46);
     }
     ch = tmp % 10;
     LCD_Chr_CP(48 + ch);

     if((value >= 1000) && (value < 10000))
     {
         LCD_Chr_CP(32);
     }
}


void show_adc_value(unsigned char y_pos, unsigned char value)
{
     unsigned char txt[4];
     
     ByteToStr(value, txt);
     lcd_out(y_pos, 4, txt);
}