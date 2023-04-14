#define data_length   14

#define Wait_LED      P3_7_bit

sbit LCD_RS at P2_2_bit;
sbit LCD_EN at P2_3_bit;

sbit LCD_D4 at P2_4_bit;
sbit LCD_D5 at P2_5_bit;
sbit LCD_D6 at P2_6_bit;
sbit LCD_D7 at P2_7_bit;


void receive_and_show()
{
     unsigned char card[data_length];
     unsigned char s = 0;

     for(s = 0; s < data_length; s += 1)
     {
         card[s] = 0x00;
     }

     for(s = 0; s < data_length; s += 1)
     {
         while(!RI_bit);
         card[s] =  SBUF;
         RI_bit = 0;
     }

     for(s = 1; s < data_length; s += 1)
     {
         SBUF = card[s];
         if((card[s] >= 48) && (card[s] <= 122))
         {
             lcd_chr(2, (s + 2), card[s]);
         }
         while(!TI_bit);
         TI_bit = 0;
     }
     Wait_LED = 0;
}

void main()
{
    TMOD=0x20;
    TH1=0xFD;
    SCON=0x50;
    TR1_bit=1;
    SMOD_bit = 0;
    delay_ms(100);
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    lcd_out(1, 2, "Barcode Reader");
    while(1)
    {
        receive_and_show();
        delay_ms(2000);
        Wait_LED = 1;
    }
}