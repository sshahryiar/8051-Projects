#define GLCD_CS                                       P3_2_bit
#define GLCD_RST                                      P3_3_bit
#define GLCD_RS                                       P2_6_bit
#define GLCD_RW                                       P2_7_bit
#define GLCD_RD                                       P2_5_bit

#define GLCD_data_port                                P0

#define CMD                                           0
#define DAT                                           1

#define x_max                                         128
#define x_min                                         0
#define y_max                                         64
#define y_min                                         0
#define row_max                                       8
#define row_min                                       0
#define buffer_size                                   1024

#define ON                                            1
#define OFF                                           0

#define round                                         1
#define square                                        0

#define Set_Column_Address_Low                        0x00
#define Set_Column_Address_High                       0x10
#define Select_Internal_Resistor_Ratio                0x20
#define Select_Internal_Power_Supply_Mode             0x28
#define Set_LCD_Start_Line                            0x40
#define Set_Electronic_Volume_Mode                    0x81
#define ADC_Select_Normal                             0xA0
#define ADC_Select_Reverse                            0xA1
#define Set_LCD_Bias_Low                              0xA2
#define Set_LCD_Bias_High                             0xA3
#define Display_All_Points_Off                        0xA4
#define Display_All_Points_On                         0xA5
#define Display_Normal                                0xA6
#define Display_Reverse                               0xA7
#define Display_Off                                   0xAE
#define Display_On                                    0xAF
#define Set_Page_Address                              0xB0
#define Set_Common_Output_Normal                      0xC0
#define Set_Common_Output_Reverse                     0xC8
#define Internal_Reset                                0xE2
#define NOP_cmd                                       0xE3
#define End_cmd                                       0xEE
#define Set_Boost_Ratio_Low                           0xF8
#define Set_Boost_Ratio_Medium                        0xF9
#define Set_Boost_Ratio_High                          0xFB
#define Test_Display                                  0xFF



unsigned char buffer[buffer_size];


void GLCD_write(unsigned char value, unsigned char type);
void GLCD_init();
void GLCD_fill(unsigned char value);
void GLCD_draw_bmp(unsigned char *bmp_data);
void GLCD_clear_screen();
void GLCD_clear_buffer();
void GLCD_gotoxy(unsigned char x_pos, unsigned char y_pos);
void GLCD_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch);
void GLCD_print_string(unsigned char x_pos, unsigned char y_pos, unsigned char *ch);
void GLCD_print_chr(unsigned char x_pos, unsigned char y_pos, signed int value);
void GLCD_print_int(unsigned char x_pos, unsigned char y_pos, signed long value);
void GLCD_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned int value, unsigned char points);
void GLCD_print_float(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points);
void GLCD_set_pixel(unsigned char x_pos, unsigned char y_pos, unsigned char colour);
void GLCD_line(signed int x1, signed char y1, signed int x2, signed char y2, unsigned char colour);
void GLCD_rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char colour, unsigned char type);
void GLCD_circle(unsigned char xc, unsigned char yc, unsigned char radius, unsigned char fill, unsigned char colour);