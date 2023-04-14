#define W_CLK                   P0_0_bit
#define FU_UD                   P0_1_bit
#define DATA                    P0_2_bit
#define RESET                   P0_3_bit
#define LED                     P3_7_bit

#define INC                     !P3_0_bit
#define DEC                     !P3_1_bit
#define RNG                     !P3_2_bit
#define PWR                     !P3_3_bit

#define F_const                 34.359738368        //((2^32)/125000000)
#define max_f                   125000000
#define min_f                   1
#define max_increment_size      10000000
#define min_increment_size      0.01
#define increment_step_size     10
#define kbd_dly                 40


sbit LCD_RS at P2_2_bit;
sbit LCD_EN at P2_3_bit;

sbit LCD_D4 at P2_4_bit;
sbit LCD_D5 at P2_5_bit;
sbit LCD_D6 at P2_6_bit;
sbit LCD_D7 at P2_7_bit;

sbit Sound_Play_Pin at P1_0_bit;



void setup();
void AD9850_init();
void AD9850_write(unsigned char value);
void DSS_update(unsigned char cp, float f);


void main() 
{
     bit changed;
     bit last_state;
     unsigned char control = 0x04;
     float freq = min_f;
     float step = min_increment_size;
     
     unsigned char f_txt[15];
     unsigned char r_txt[15];
     
     changed = 1;
     last_state = 0;
     
     setup();

     while(1)
     {
             if(INC)
             {
                    Sound_Play(2600, kbd_dly);
                    freq += step;
                    changed = 1;
             }
             if(DEC)
             {
                    Sound_Play(2200, kbd_dly);
                    freq -= step;
                    changed = 1;
             }
             if(RNG)
             {
                    Sound_Play(3000, kbd_dly);
                    while(RNG);
                    step *= increment_step_size;
                    changed = 1;
             }
             if(PWR)
             {
                    Sound_Play(1800, kbd_dly);
                    while(PWR);
                    changed = 1;
                    last_state ^= 1;
                    
                    if(last_state)
                    {
                        control = 0x00;
                        LED = 0;
                    }
                    else
                    {
                        control = 0x04;
                        LED = 1;
                    }
             }
             
             if(step > max_increment_size)
             {
                    step = min_increment_size;
             }
             
             if(freq > max_f)
             {
                     freq = min_f;
             }
             if(freq < min_f)
             {
                     freq = max_f;
             }
             
             if(changed)
             {
                        Lcd_Cmd(_LCD_CLEAR);
                        lcd_out(1, 1, "F/Hz:");
                        lcd_out(2, 1, "S/Hz:");

                        FloatToStr(freq, f_txt);
                        Lcd_Out(1, 6, f_txt);
                        FloatToStr(step, r_txt);
                        Lcd_Out(2, 6, r_txt);
                        
                        DSS_update(control, freq);
                        changed = 0;
             }
     };
}


void setup()
{
    AD9850_init();
    Sound_Init();
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
}


void AD9850_init()
{
     W_CLK = 0;
     FU_UD = 0;
     
     RESET = 0;
     RESET = 1;
     RESET = 0;

     W_CLK = 0;
     W_CLK = 1;
     W_CLK = 0;

     FU_UD = 0;
     FU_UD = 1;
     FU_UD = 0;
}


void AD9850_write(unsigned char value)
{
     unsigned char s = 0;
     unsigned char val = 0;
     
     val = value;
     
     for(s = 0; s < 8; s++)
     {
           DATA = ((val >> s) & 0x01);
           W_CLK = 1;
           W_CLK = 0;
     }
}


void DSS_update(unsigned char cp, float f)
{
     unsigned char tmp = 0;
     unsigned long temp = 0;
     
     f *=  F_const;
     temp = f;
     
     tmp = ((unsigned char)temp);
     AD9850_write(tmp);
     tmp = ((unsigned char)(temp >> 8));
     AD9850_write(tmp);
     tmp = ((unsigned char)(temp >> 16));
     AD9850_write(tmp);
     tmp = ((unsigned char)(temp >> 24));
     AD9850_write(tmp);
     tmp = cp;
     AD9850_write(tmp);
     
     FU_UD = 1;
     FU_UD = 0;
}