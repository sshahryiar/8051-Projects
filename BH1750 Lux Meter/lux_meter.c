#include "BH1750.c"


sbit LCD_RS at P2.B2;
sbit LCD_EN at P2.B3;

sbit LCD_D4 at P2.B4;
sbit LCD_D5 at P2.B5;
sbit LCD_D6 at P2.B6;
sbit LCD_D7 at P2.B7;


sbit Soft_I2C_Scl at P1_0_bit;
sbit Soft_I2C_Sda at P1_1_bit;


void setup();


void main() 
{
    unsigned char txt[15];
    
    unsigned int tmp = 0;
    
    float lux_value = 0;
    float fc = 0;
    
    setup();
    
    while(1)
    {
        tmp = get_lux_value(cont_L_res_mode, 20);
        
        if(tmp > 10)
        {
            lux_value = tmp;
        }
        else
        {
            lux_value = get_lux_value(cont_H_res_mode1, 140);
        }
        
        fc = (lux_value * 0.0929030436);
        
        IntToStrWithZeros(lux_value, txt);
        Lcd_Out(1, 11, txt);
        
        IntToStrWithZeros(fc, txt);
        Lcd_Out(2, 11, txt);
    };
}


void setup()
{
    Soft_I2C_Init();
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    lcd_out(1, 1, "Lux:");
    lcd_out(2, 1, "F-C:");
}