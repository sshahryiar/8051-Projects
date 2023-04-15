#include "HMC5883L.h"


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
    delay_ms(100);
    Soft_I2C_Break();
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


signed long HMC5883L_heading()
{
    float heading = 0.0;

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