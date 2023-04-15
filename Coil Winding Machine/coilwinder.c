#define SET          !P1_0_bit
#define INC          !P1_1_bit
#define DEC          !P1_2_bit 
#define FEEDBACK     P1_3_bit 

#define terminal_A   P3_0_bit
#define terminal_B   P3_1_bit


sbit LCD_RS at P2.B2;
sbit LCD_EN at P2.B3;
          
sbit LCD_D4 at P2.B4;
sbit LCD_D5 at P2.B5;
sbit LCD_D6 at P2.B6;
sbit LCD_D7 at P2.B7; 

sbit Sound_Play_Pin at P1_7_bit;


bit running;
bit rev_fwd;                   

unsigned char turns = 10;
unsigned char target_turns = 0;


void setup();
unsigned short scan_kbd();
void display_common(unsigned char x, unsigned char y, unsigned char convert);
unsigned char changes(unsigned char x, unsigned char y, unsigned char value, unsigned char max, unsigned char min);
void settings();
void start_motor();
void stop_motor();
void general_display();
void motor_stat();


void main()
{
     setup();                  

     while(1)
     {
         settings();
         motor_stat();
         general_display();
     };
}


void setup()
{
    running = 0;
    rev_fwd = 0;              
    P1 = 0x7F;        
    P2 = 0xFF; 
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);   
    Sound_Init();
}


unsigned short scan_kbd()
{
      if(SET)
      {
          Sound_Play(3000, 45);
          return 1;
      }
      else if(INC)
      {
          Sound_Play(2600, 45);
          return 2;
      }
      else if(DEC)
      {
          Sound_Play(2400, 45);
          return 3;
      }
      else
      {
          return 0;
      }
}


void display_common(unsigned char x, unsigned char y, unsigned char convert)
{
    unsigned char t = 0;

    t = (convert / 100);
    Lcd_Chr(y, x, (48 + t));
    t = ((convert / 10) % 10);
    Lcd_Chr(y, (x + 1), (48 + t));
    t = (convert % 10);
    Lcd_Chr(y, (x + 2),(48 + t));
}


unsigned char changes(unsigned char x, unsigned char y, unsigned char value, unsigned char max, unsigned char min)
{
    while(1)
    {
          lcd_out(y, x, "   ");
          delay_ms(100);
          display_common(x, y, value);
          delay_ms(100);

          if(scan_kbd() == 2)
          {
              value += 1;
          }
          if(scan_kbd() == 3)
          {
              value -= 1;
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
              delay_ms(90);
              while(scan_kbd() == 1);
              return value;
          }
    }
}


void settings()
{
    if(SET)
    {
        delay_ms(600);
        if(SET)
        {
            delay_ms(200);
            while(SET);
            running = 0;
            stop_motor();
            Lcd_Cmd(_LCD_CLEAR);
            lcd_out(1, 3, "No. of Turns");
            turns = changes(7, 2, turns, 250, 1);
            delay_ms(900);
            Lcd_Cmd(_LCD_CLEAR);
        }
        else
        {
            running ^= 1;
            delay_ms(100);
        }
    }
}


void start_motor()
{
     switch(rev_fwd)
     {
         case 1:
         {
             terminal_A = 0;
             terminal_B = 1;
             lcd_out(2, 13, " FWD");
             if(FEEDBACK)
             {
                target_turns -= 1;
             }

             if(target_turns == 0)
             {
                 running = 0;
             }
             break;
         }
         default:
         {
             terminal_A = 1;
             terminal_B = 0;
             lcd_out(2, 13, " RWD");

             if(FEEDBACK)
             {
                target_turns += 1;
             }

             if(target_turns == turns)
             {
                 running = 0;
             }
             break;
         }
     }

     delay_ms(60);
}

void stop_motor()
{
    terminal_A = 0;
    terminal_B = 0;
    lcd_out(2, 13, "STOP");
}


void general_display()
{
     lcd_out(1, 4, "Coil Winder");
     lcd_out(2, 1, "Turns:");
     display_common(8, 2, target_turns);
}


void motor_stat()
{
    if(INC)
    {
        rev_fwd = 0;
        target_turns = 0;
    }

    if(DEC)
    {
        rev_fwd = 1;
        target_turns = turns;
    }

    if(running)
    {
        start_motor();
    }
    else
    {
        stop_motor();
    }
}
