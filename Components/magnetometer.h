/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 28/Nov/2020
 * Component: Magnetometer MAG3110 header
 * Email: w20014215@northumbria.ac.uk
********************************/

//i2C error definition
#define ERR_NONE 0
#define ERR_NO_ACK 0x01
#define ERR_ARB_LOST 0x02
#define ERR_BUS_BUSY 0x03

//MAG3110 sensor
#define MAG_I2C_Address 0x0E
#define MAG_Output_Address 0x01
#define MAG_CTRL_REG_1_Address 0x10
#define MAG_CTRL_REG_2_Address 0x11
#define MAG_STATUS 0x00
#define MAG_OFF_X_MSB 0x0A
#define MAG_OFF_X_LSB 0x0B
#define MAG_OFF_Y_MSB 0x0C
#define MAG_OFF_Y_LSB 0x0D
#define MAG_OFF_Z_MSB 0x0E
#define MAG_OFF_Z_LSB 0x0F


void mag_init(void);
int read_mag(int* buff);
void mag_offset_calibration(void);
