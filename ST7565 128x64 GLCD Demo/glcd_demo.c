#include "background.h"
#include "PCF8591.c"
#include "ST7565.c"


#define scale_value        0.01960784314
#define ADC_Max                    255.0
#define RL                         100.0
#define LDR_Constant               500.0
#define B                         3950.0
#define RT                       10000.0
#define _25C_K                     298.0
#define _0K                        273.0
#define R_inf                0.017515077


void setup();
void draw_bmp();


void main() 
{
     unsigned char adc0 = 0;
     unsigned char adc1 = 0;
     unsigned char adc2 = 0;
     unsigned char adc3 = 0;

     float lux = 0.0;
     float t = 0.0;

     setup();
     draw_bmp();
     
     while(1)
     {
         adc0 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN0);
         adc1 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN1);
         adc2 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN2);
         adc3 = PCF8591_read(AOut_enable | Four_Channel_ADC | Auto_Increment_Disable | AIN3);
         t = ((ADC_Max / adc1) - 1);
         t *= RT;
         t /= R_inf;
         t = (B / (log(t)));
         t = (t - _0K);

         lux = (((ADC_Max / adc2) - 1) * RL);
         lux = (LDR_Constant / lux);
         
         GLCD_print_chr(19, 1, adc0);
         GLCD_print_chr(19, 3, adc1);
         GLCD_print_chr(19, 5, adc2);
         GLCD_print_chr(19, 7, adc3);
         
         GLCD_print_float(85, 1, (adc0 * scale_value), 2);
         GLCD_print_float(85, 3, t, 2);
         GLCD_print_float(85, 5, lux, 1);
         GLCD_print_float(85, 7, (adc3 * scale_value), 2);

         PCF8591_write(AOut_enable, adc0);
         
         delay_ms(400);
     }
}


void setup()
{
     GLCD_init();
}


void draw_bmp()
{
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned int k = 0;
    
    for(i = row_min; i < row_max; i++)
    {
         GLCD_Write(0x40, CMD);
         GLCD_Write((i + 0xB0), CMD);
         GLCD_Write(0x10, CMD);
         GLCD_Write(0x00, CMD);

          for(j = x_min; j < x_max; j++)
          {
             GLCD_Write(img[k++], DAT);
          }
    }
}