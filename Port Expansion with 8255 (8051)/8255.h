#define WR                             P3_2_bit
#define RD                             P3_3_bit
#define Addr_0                         P3_4_bit
#define Addr_1                         P3_5_bit
#define CS                             P3_7_bit

#define data_port_out                  P1
#define data_port_in                   P1
#define data_port_dir_out              P1 = 0x00
#define data_port_dir_in               P1 = 0xFF

#define CTRL_REG                       0x00
#define PORT_A                         0x01
#define PORT_B                         0x02
#define PORT_C                         0x03

#define PORTC_lower_input              0x01
#define PORTC_lower_output             0x00
#define PORTB_input                    0x02
#define PORTB_output                   0x00
#define Group_B_strobed_IO             0x04
#define Group_B_basic_IO               0x00
#define PORTC_upper_input              0x08
#define PORTC_upper_output             0x00
#define PORTA_input                    0x10
#define PORTA_output                   0x00
#define Group_A_bidirectional_bus      0x60
#define Group_A_strobed_IO             0x20
#define Group_A_basic_IO               0x00
#define mode_set_flag_active           0x80
#define mode_set_flag_inactive         0x00


void _8255_init();       
void select_reg(unsigned char reg_cmd);
void write_8255(unsigned char reg_cmd, unsigned char value);
unsigned char read_8255(unsigned char reg_cmd);