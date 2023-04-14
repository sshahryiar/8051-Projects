sbit LCD_RS at P2_5_bit;
sbit LCD_EN at P2_7_bit;

sbit LCD_D4 at P0_4_bit;
sbit LCD_D5 at P0_5_bit;
sbit LCD_D6 at P0_6_bit;
sbit LCD_D7 at P0_7_bit;

sbit Soft_I2C_Scl at P1_7_bit;
sbit Soft_I2C_Sda at P1_6_bit;


#define HMC5883L_READ_ADDR       0x3D
#define HMC5883L_WRITE_ADDR      0x3C

#define Config_Reg_A             0x00
#define Config_Reg_B             0x01
#define Mode_Reg                 0x02
#define X_MSB_Reg                0x03
#define X_LSB_Reg                0x04
#define Z_MSB_Reg                0x05
#define Z_LSB_Reg                0x06
#define Y_MSB_Reg                0x07
#define Y_LSB_Reg                0x08
#define Status_Reg               0x09
#define ID_Reg_A                 0x0A
#define ID_Reg_B                 0x0B
#define ID_Reg_C                 0x0C

#define PI                      3.142
#define declination_angle     -0.5167        // For Uttara, Dhaka, Bangladesh


const unsigned char symbol[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};

signed int X_axis = 0;
signed int Y_axis = 0;
signed int Z_axis = 0;
float m_scale = 1.0;


void setup();
void CustomChar(unsigned char pos_row, unsigned char pos_char);
void lcd_show_value(unsigned char x, unsigned char y, unsigned int value);
void light_LED(unsigned char pos);
unsigned int make_word(unsigned char HB, unsigned char LB);
void HMC5883L_init();
unsigned char HMC5883L_read(unsigned char reg);
void HMC5883L_write(unsigned char reg_address, unsigned char value);
void HMC5883L_read_data();
void HMC5883L_scale_axes();
void HMC5883L_set_scale(float gauss);
float HMC5883L_heading();


void main() 
{
     unsigned int b = 0;
     
     setup();
     
     while(1)
     {
          b = HMC5883L_heading();
          lcd_show_value(6, 2, b);

          P2_0_bit = 1;
          P2_1_bit = 1;
          P2_2_bit = 1;
          P2_3_bit = 1;
          P3 = 0xFF;

          if((b > 0) && (b <= 30))
          {
              P3_2_bit = 0;
          }
          else if((b > 30) && (b <= 60))
          {
              P3_3_bit = 0;
          }
          else if((b > 60) && (b <= 90))
          {
              P3_4_bit = 0;
          }
          else if((b > 90) && (b <= 120))
          {
              P3_5_bit = 0;
          }
          else if((b > 120) && (b <= 150))
          {
              P3_6_bit = 0;
          }
          else if((b > 150) && (b <= 180))
          {
              P3_7_bit = 0;
          }
          else if((b > 180) && (b <= 210))
          {
              P2_0_bit = 0;
          }
          else if((b > 210) && (b <= 240))
          {
              P2_1_bit = 0;
          }
          else if((b > 240) && (b <= 270))
          {
              P2_2_bit = 0;
          }
          else if((b > 270) && (b <= 300))
          {
              P2_3_bit = 0;
          }
          else if((b > 300) && (b <= 330))
          {
              P3_0_bit = 0;
          }
          else
          {
              P3_1_bit = 0;
          }
          
          delay_ms(200);
     };
}


void setup()
{
     P2_6_bit = 0;
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     HMC5883L_init();
     lcd_out(1, 3, "< Heading. >");
     CustomChar(2, 10);
     lcd_out(2, 11, "N");
}


void CustomChar(unsigned char pos_row, unsigned char pos_char)
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


void lcd_show_value(unsigned char x, unsigned char y, unsigned int value)
{
     unsigned char ch = 0;

     ch = (value / 100);
     lcd_chr(y, x, (ch + 48));
     ch = ((value / 10) % 10);
     lcd_chr_cp((ch + 48));
     ch = (value % 10);
     lcd_chr_cp((ch + 48));
}


unsigned int make_word(unsigned char HB, unsigned char LB)
{
        unsigned int val = 0;

        val = HB;
        val <<= 8;
        val |= LB;

        return val;
}


void HMC5883L_init()
{
        Soft_I2C_Init();
        HMC5883L_write(Config_Reg_A, 0x70);
        HMC5883L_write(Config_Reg_B, 0xA0);
        HMC5883L_write(Mode_Reg, 0x00);
        HMC5883L_set_scale(1.3);
}


unsigned char HMC5883L_read(unsigned char reg)
{
        unsigned char val = 0;

        Soft_I2C_Start();
        Soft_I2C_Write(HMC5883L_WRITE_ADDR);
        Soft_I2C_Write(reg);
        Soft_I2C_Start();
        Soft_I2C_Write(HMC5883L_READ_ADDR);
        val = Soft_I2C_Read(0);
        Soft_I2C_Stop();

        return(val);
}


void HMC5883L_write(unsigned char reg_address, unsigned char value)
{
        Soft_I2C_Start();
        Soft_I2C_Write(HMC5883L_WRITE_ADDR);
        Soft_I2C_Write(reg_address);
        Soft_I2C_Write(value);
        Soft_I2C_Stop();
}

void HMC5883L_read_data()
{
        unsigned char lsb = 0;
        unsigned char msb = 0;

        Soft_I2C_Start();
        Soft_I2C_Write(HMC5883L_WRITE_ADDR);
        Soft_I2C_Write(X_MSB_Reg);
        Soft_I2C_Start();
        Soft_I2C_Write(HMC5883L_READ_ADDR);

        msb = Soft_I2C_Read(1);
        lsb = Soft_I2C_Read(1);
        X_axis = make_word(msb, lsb);

        msb = Soft_I2C_Read(1);
        lsb = Soft_I2C_Read(1);
        Z_axis = make_word(msb, lsb);

        msb = Soft_I2C_Read(1);
        lsb = Soft_I2C_Read(0);
        Y_axis = make_word(msb, lsb);

        Soft_I2C_Stop();
}


void HMC5883L_scale_axes()
{
        X_axis *= m_scale;
        Z_axis *= m_scale;
        Y_Axis *= m_scale;
}


void HMC5883L_set_scale(float gauss)
{
    unsigned char value = 0;

    if(gauss == 0.88)
    {
                value = 0x00;
                m_scale = 0.73;
        }

        else if(gauss == 1.3)
        {
                value = 0x01;
                m_scale = 0.92;
        }

        else if(gauss == 1.9)
        {
                value = 0x02;
                m_scale = 1.22;
        }

        else if(gauss == 2.5)
        {
                value = 0x03;
                m_scale = 1.52;
        }

        else if(gauss == 4.0)
        {
                value = 0x04;
                m_scale = 2.27;
        }

        else if(gauss == 4.7)
        {
                value = 0x05;
                m_scale = 2.56;
        }

        else if(gauss == 5.6)
        {
                value = 0x06;
                m_scale = 3.03;
        }

        else if(gauss == 8.1)
        {
                value = 0x07;
                m_scale = 4.35;
        }

        value <<= 5;
        HMC5883L_write(Config_Reg_B, value);
}


float HMC5883L_heading()
{
    float heading = 0;

    HMC5883L_read_data();
    HMC5883L_scale_axes();
    heading = atan2(Y_axis, X_axis);
    heading += declination_angle;

    if(heading < 0.0)
    {
            heading += (2.0 * PI);
    }

    if(heading > (2.0 * PI))
    {
            heading -= (2.0 * PI);
    }

        heading *= (180.0 / PI);

     return heading;
}