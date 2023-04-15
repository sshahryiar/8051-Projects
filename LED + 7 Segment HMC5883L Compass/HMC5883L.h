sbit Soft_I2C_Scl at        P1_5_bit;
sbit Soft_I2C_Sda at        P1_6_bit;


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


signed int X_axis = 0;
signed int Y_axis = 0;
signed int Z_axis = 0;
float m_scale = 1.0;


unsigned int make_word(unsigned char HB, unsigned char LB);
void HMC5883L_init();
unsigned char HMC5883L_read(unsigned char reg);
void HMC5883L_write(unsigned char reg_address, unsigned char value);
void HMC5883L_read_data();
void HMC5883L_scale_axes();
void HMC5883L_set_scale(float gauss);
signed long HMC5883L_heading();