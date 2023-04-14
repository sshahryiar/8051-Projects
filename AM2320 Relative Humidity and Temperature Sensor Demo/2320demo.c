#include "AM2320.c"


sbit LCD_RS at P2.B2;
sbit LCD_EN at P2.B3;

sbit LCD_D4 at P2.B4;
sbit LCD_D5 at P2.B5;
sbit LCD_D6 at P2.B6;
sbit LCD_D7 at P2.B7;


const unsigned char symbol[0x08]=
{
   0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
};


void setup();
void LCD_load();
void lcd_print(unsigned char x_pos, unsigned char y_pos, signed long value);


void main() 
{
     unsigned char txt[6];
     signed int T = 0x0000;
     unsigned int RH = 0x0000;
     unsigned int CRC_data = 0x0000;
     unsigned int CRC_temp = 0x0000;
     
     setup();

     lcd_out(1, 2, "AM2320 Sensor Data");
     lcd_out(2, 1, "R.H/%:");
     lcd_out(3, 1, "T./ C:");
     lcd_out(4, 1, "CRC16:");
     lcd_chr(3, 4, 0);

     while(1)
     {
             
             get_sensor_data(AM2320_RH_hb, 0x04, 0x08);
             get_RH_and_temperature(&RH, &T);
             get_CRC(&CRC_temp);
             CRC_data = crc16(data_buffer, 6);

             if(CRC_temp == CRC_data)
             {
                 lcd_print(8, 2, RH);
                 lcd_print(8, 3, T);
                 WordToHex(CRC_data, txt);
                 lcd_out(4, 8, "0x");
                 lcd_out(4, 10, txt);
             }
             else
             {
                 lcd_out(2, 7, " --.- ");
                 lcd_out(3, 7, " --.- ");
                 lcd_out(4, 8, "ERROR!");
             }
             
             delay_ms(4000);
     };
}


void setup()
{
     AM2320_init();
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     LCD_load();
}


void LCD_load()
{
    unsigned char n = 0;
    LCD_Cmd(64);
    for (n = 0; n < 8; n++)
    {
        Lcd_Chr_CP(symbol[n]);
    }
    Lcd_Cmd(_LCD_RETURN_HOME);
}


void lcd_print(unsigned char x_pos, unsigned char y_pos, signed long value)
{
     unsigned char ch = 0x00;
     
     if(value >= 0)
     {
        lcd_chr(y_pos, x_pos, 32);
     }
     if(value < 0)
     {
        lcd_chr(y_pos, x_pos, 45);
        value = -value;
     }

     if((value > 999) && (value <= 9999))
     {
         ch = (value / 1000);
         ch += 48;
         lcd_chr_cp(ch);
         ch = ((value / 100) % 10);
         ch += 48;
         lcd_chr_cp(ch);
         ch = ((value / 10) % 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(46);
         ch = (value % 10);
         ch += 48;
         lcd_chr_cp(ch);
     }
     else if((value > 99) && (value <= 999))
     {
         ch = (value / 100);
         ch += 48;
         lcd_chr_cp(ch);
         ch = ((value / 10) % 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(46);
         ch = (value % 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(32);
     }
     else if((value > 9) && (value <= 99))
     {
         ch = (value / 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(46);
         ch = (value % 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(32);
         lcd_chr_cp(32);
     }
     else if((value > 0) && (value <= 9))
     {
         lcd_chr_cp(48);
         lcd_chr_cp(46);
         ch = (value % 10);
         ch += 48;
         lcd_chr_cp(ch);
         lcd_chr_cp(32);
         lcd_chr_cp(32);
     }
}