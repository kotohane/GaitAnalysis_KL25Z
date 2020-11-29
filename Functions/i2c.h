/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 15/Nov/2020
 * File: I2C module header
 * Email: w20014215@northumbria.ac.uk
********************************/

#define ERR_NONE 0
#define ERR_NO_ACK 0x01
#define ERR_ARB_LOST 0x02
#define ERR_BUS_BUSY 0x03

void i2c0_init(void);
int i2c0_byte_write(char SlaveAddr, char RegAddr,char DataByte);
unsigned char i2c0_byte_read(unsigned char SlaveAddr,unsigned char RegAddr);
void i2c0_burst_read(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char n, unsigned char *buff);

void i2c1_init(void);
int i2c1_byte_write(char SlaveAddr, char RegAddr,char DataByte);
char i2c1_byte_read(char SlaveAddr, char RegAddr);
void i2c1_burst_read(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char n, unsigned char *buff);
