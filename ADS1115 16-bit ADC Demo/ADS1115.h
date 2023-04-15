#define SDA_pin                                                                          P2_0_bit
#define SCL_pin                                                                          P2_1_bit

#define addr_pin_tied_to_GND                                                             0x90
#define addr_pin_tied_to_VDD                                                             0x92
#define addr_pin_tied_to_SDA                                                             0x94
#define addr_pin_tied_to_SCL                                                             0x96

#define ADS1115_device_address                                                           addr_pin_tied_to_GND
#define ADS1115_write_cmd                                                                ADS1115_device_address
#define ADS1115_read_cmd                                                                 (ADS1115_device_address + 1)

#define high_reg                                                                         0
#define low_reg                                                                          1

#define ADS1115_conversion_reg_pointer                                                   0x00
#define ADS1115_config_reg_pointer                                                       0x01
#define ADS1115_Lo_Thres_reg_pointer                                                     0x02
#define ADS1115_Hi_Thres_reg_pointer                                                     0x03

#define start_one_conversion                                                             0x8000

#define AINP_AIN0__AINN_AIN1                                                             0x0000
#define AINP_AIN0__AINN_AIN3                                                             0x1000
#define AINP_AIN1__AINN_AIN3                                                             0x2000
#define AINP_AIN2__AINN_AIN3                                                             0x3000
#define AINP_AIN0__AINN_GND                                                              0x4000
#define AINP_AIN1__AINN_GND                                                              0x5000
#define AINP_AIN2__AINN_GND                                                              0x6000
#define AINP_AIN3__AINN_GND                                                              0x7000

#define FS_6144mV                                                                        0x0000
#define FS_4096mV                                                                        0x0200
#define FS_2048mV                                                                        0x0400
#define FS_1024mV                                                                        0x0600
#define FS_512mV                                                                         0x0800
#define FS_256mV                                                                         0x0A00
#define FS_256mV                                                                         0x0C00
#define FS_256mV                                                                         0x0E00

#define continuous_conversion_mode                                                       0x0000
#define power_down_single_shot_mode                                                      0x0100

#define data_rate_8SPS                                                                   0x0000
#define data_rate_16SPS                                                                  0x0020
#define data_rate_32SPS                                                                  0x0040
#define data_rate_64SPS                                                                  0x0060
#define data_rate_128SPS                                                                 0x0080
#define data_rate_250SPS                                                                 0x00A0
#define data_rate_475SPS                                                                 0x00C0
#define data_rate_860SPS                                                                 0x00E0

#define comparator_with_hysteresis                                                       0x0000
#define window_comparator                                                                0x0010

#define comparator_polarity_active_low                                                   0x0000
#define comparator_polarity_active_high                                                  0x0008

#define non_latching_comparator                                                          0x0000
#define latching_comparator                                                              0x0004

#define assert_after_one_conversion                                                      0x0000
#define assert_after_two_conversions                                                     0x0001
#define assert_after_four_conversions                                                    0x0002
#define disable_comparator                                                               0x0003

#define ADC_count_max                                                                    0x7FFF


sbit Soft_I2C_Scl at SCL_pin;
sbit Soft_I2C_Sda at SDA_pin;


void ADS1115_init();
void ADS1115_write(unsigned char pointer, unsigned int value);
void ADS1115_configure(unsigned int value);
void ADS1115_write_thresholds(unsigned char reg_select, unsigned int value);
unsigned int ADS1115_read(unsigned char pointer);