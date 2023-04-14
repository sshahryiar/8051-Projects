#define DHT11_pin       P1_0_bit

sbit LCD_RS at P2.B2;
sbit LCD_EN at P2.B3;

sbit LCD_D4 at P2.B4;
sbit LCD_D5 at P2.B5;
sbit LCD_D6 at P2.B6;
sbit LCD_D7 at P2.B7;


unsigned char DHT11_data[5];

const unsigned char symbol[8]=
{
   0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00
};


void setup();
void LCD_load();
void DHT11_init();
unsigned short get_byte();
unsigned short get_data();


void main() 
{
     unsigned char text[6];
     unsigned short state = 0;

     setup();
     
     while(1)
     {
            state = get_data();

            switch(state)
            {
                  case 1:
                  {
                  }
                  case 2:
                  {
                     lcd_cmd(_LCD_CLEAR);
                     lcd_out(1, 1, "No Sensor Found!");
                     break;
                  }
                  case 3:
                  {
                     lcd_cmd(_LCD_CLEAR);
                     lcd_out(1, 1, "Checksum Error!");
                     break;
                  }
                  default:
                  {
                     lcd_cmd(_LCD_CLEAR);
                     lcd_out(1, 4, "R.H: ");
                     lcd_out(2, 4, "Tmp: ");
                     lcd_out(1, 13, "%");
                     lcd_chr(2, 12, 0);
                     lcd_out(2, 13, "C");
                     ByteToStr(DHT11_data[0], Text);
                     Lcd_Out(1, 9, Text);
                     ByteToStr(DHT11_data[2], Text);
                     Lcd_Out(2, 9, Text);
                     break;
                  }
            }
            delay_ms(600);
     };
}


void setup()
{
      P0 = 0xFF;
      P1 = 0xFE;
      P2 = 0xFF;
      P3 = 0xFF;
      Lcd_Init();
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Cmd(_LCD_CURSOR_OFF);
      LCD_load();
}


void LCD_load()
{
    unsigned char n = 0;
    LCD_Cmd(64);
    for (n = 0; n < 8; n += 1)
    {
        Lcd_Chr_CP(symbol[n]);
    }
    Lcd_Cmd(_LCD_RETURN_HOME);
}


void DHT11_init()
{
   P1_0_bit = 1;
   delay_ms(1000);
}


unsigned short get_byte()
{
   unsigned short s = 0;
   unsigned short value = 0;
   for(s = 0; s < 8; s += 1)
   {
      value <<= 1;
      while((DHT11_pin == 0));
      delay_us(30);
      if(DHT11_pin == 1)
      {
          value |= 1;
      }
      while(DHT11_pin == 1);
   }
   return value;
}


unsigned short get_data()
{
   bit chk;
   unsigned short s = 0;
   unsigned char check_sum = 0;

   DHT11_pin = 1;
   DHT11_pin = 0;
   delay_ms(18);
   DHT11_pin = 1;
   delay_us(26);

   chk = DHT11_pin;
   
   if(chk)
   {
      return 1;
   }
   delay_us(80);

   chk = DHT11_pin;
   if(!chk)
   {
      return 2;
   }
   delay_us(80);

   for(s = 0; s <= 4; s += 1)
   {
       DHT11_data[s] = get_byte();
   }

   DHT11_pin = 1;

   for(s = 0; s < 4; s += 1)
   {
       check_sum += DHT11_data[s];
   }

   if(check_sum != DHT11_data[4])
   {
      return 3;
   }
   else
   {
      return 0;
   }
}