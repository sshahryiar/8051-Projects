#define DO                                            P3_7_bit
#define CLK                                           P3_6_bit
#define CS                                            P3_5_bit
#define DI                                            P3_4_bit


#define power_down_between_conversions_IRQ_enabled    0x00
#define ref_on_ADC_off_IRQ_disabled                   0x01
#define ref_off_ADC_on                                0x02
#define no_power_down                                 0x03

#define differential_ADC                              0x00
#define single_ended_ADC                              0x04

#define ADC_res_12                                    0x00
#define ADC_res_8                                     0x08

#define start_con                                     0x80

#define ch_temp0                                      0x00
#define ch0                                           0x10
#define ch1                                           0x50
#define ch2                                           0x20
#define ch3                                           0x60
#define ch_temp1                                      0x70

#define VDD_in_mV                                   5000.0
#define ADC_count_max                               4095.0


void XPT2046_init();
void XPT2046_write(unsigned char value);
unsigned int XPT2046_read();
unsigned int XPT2046_read_ADC(unsigned char ch, unsigned char cmd);
void XPT2046_conversion_delay();
signed char XPT2046_get_temperature();