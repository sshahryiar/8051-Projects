#define DQ_pin                                                          P3_2_bit
#define CLK_pin                                                         P3_3_bit
#define RST_pin                                                         P3_4_bit

//DS1620 Commands

#define write_TH_command                                                0x01
#define write_TL_command                                                0x02
#define write_configuration_command                                     0x0C
#define read_counter_command                                            0xA0
#define read_TH_command                                                 0xA1
#define read_TL_command                                                 0xA2
#define read_slope_command                                              0xA9
#define read_temperature_command                                        0xAA
#define read_configuration_command                                      0xAC
#define stop_temperature_conversion_command                             0x22
#define start_temperature_conversion_command                            0xEE

//DS1620 Flags/Bits

#define done                                                            0x80
#define THF                                                             0x40
#define TLF                                                             0x20
#define NVB                                                             0x10
#define Reserved                                                        0x08
#define CPU                                                             0x02
#define One_Shot                                                        0x01

//Configuration Status

#define write_successful                                                0x00
#define write_unsuccessful                                              0x01
#define bad_configuration                                               0x02

//Temperature Range

#define max_T                                                           125
#define min_T                                                           -55


void DS1620_init();
void DS1620_rst_start();
void DS1620_rst_stop();
void DS1620_start_conv();
void DS1620_stop_conv();
void DS1620_send_command(unsigned char command);
signed int DS1620_get_data();
unsigned char DS1620_read_config();
unsigned char DS1620_write_config(unsigned char value);
signed int DS1620_read_TH();
signed int DS1620_read_TL();
signed int DS1620_read_counter();
signed int DS1620_read_slope();
void DS1620_write_TH(signed int value);
void DS1620_write_TL(signed int value);
void DS1620_read_T(signed char *m, unsigned char *f);