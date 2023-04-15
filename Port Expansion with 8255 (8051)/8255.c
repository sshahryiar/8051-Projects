#include "8255.h"


void _8255_init()
{
   write_8255(CTRL_REG, (PORTA_input | PORTB_input | PORTC_lower_output | PORTC_upper_output | Group_A_basic_IO | Group_B_basic_IO | mode_set_flag_active));
}       


void select_reg(unsigned char reg_cmd)
{
   switch(reg_cmd)
    {
        case PORT_A:
        {
            Addr_0 = 0;
            Addr_1 = 0;
            break;
        }
        
        case PORT_B:
        {
            Addr_0 = 1;
            Addr_1 = 0;
            break;
        }
        
        case PORT_C:
        {
            Addr_0 = 0;
            Addr_1 = 1;
            break;
        }
        
        case CTRL_REG:
        {
            Addr_0 = 1;
            Addr_1 = 1;
            break;
        }
    }
}


void write_8255(unsigned char reg_cmd, unsigned char value)
{
    CS = 0;
    RD = 1;
    WR = 0;
    data_port_dir_out;
    select_reg(reg_cmd);
    data_port_out = value;
    CS = 1;
}


unsigned char read_8255(unsigned char reg_cmd)
{
    unsigned char value = 0x00;
    
    CS = 0;
    RD = 0;
    WR = 1;
    data_port_dir_in;
    select_reg(reg_cmd);
    value = data_port_in;
    CS = 1;
    
    return value;
}