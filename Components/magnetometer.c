/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 28/Nov/2020
 * Component: IMU mpu6050 header
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>
#include "../functions/i2c.h"
#include "magnetometer.h"
#include "../functions/uart.h"

#define CalibrationCycle 200

void mag_init(){
	i2c0_byte_write(MAG_I2C_Address, MAG_CTRL_REG_2_Address, 0x80); //enable auto reset
	i2c0_byte_write(MAG_I2C_Address, MAG_CTRL_REG_1_Address, 0x01); //80Hz, actice mode
	if(i2c0_byte_read(MAG_I2C_Address, 0x07)==0xC4){
		transmit("MAG successful\r\n");
	}
	else{
		transmit("MAG fail\r\n");
	}
}

/* read mag data
 * 0: data not ready
 * 1: successful read
*/
int read_mag(int* buff){
	unsigned char reg_val = 0;
	reg_val = i2c0_byte_read(MAG_I2C_Address, MAG_STATUS);
	if(!(reg_val & 0x80)){
		return 0;   //read failed (data not ready)
	}
	unsigned char mag_data[6];
	i2c0_burst_read(MAG_I2C_Address, MAG_Output_Address, 6, mag_data);
	buff[0] = ((short)(mag_data[0]<<8 | mag_data[1]));
	buff[1] = ((short)(mag_data[2]<<8 | mag_data[3]));
	buff[2] = ((short)(mag_data[4]<<8 | mag_data[5]));
	return 1; 	//read secceed
}

void mag_offset_calibration(){
    short Xout_16_bit_avg, Yout_16_bit_avg, Zout_16_bit_avg;
    short Xout_16_bit_max, Yout_16_bit_max, Zout_16_bit_max;
    short Xout_16_bit_min, Yout_16_bit_min, Zout_16_bit_min;
	for(int i=0;i<CalibrationCycle;i++){ //Calibration process
        int mag_data[3];
        if(read_mag(mag_data)){
            short Xout_16_bit = (short)mag_data[0];
            short Yout_16_bit = (short)mag_data[1];
            short Zout_16_bit = (short)mag_data[2];
            if(i==0){
                Xout_16_bit_max = Xout_16_bit;
                Xout_16_bit_min = Xout_16_bit;
                Yout_16_bit_max = Yout_16_bit;
                Yout_16_bit_min = Yout_16_bit;
                Zout_16_bit_max = Zout_16_bit;
                Zout_16_bit_min = Zout_16_bit;
            }
            // Check to see if current sample is the maximum or minimum X-axis value
            if (Xout_16_bit > Xout_16_bit_max) {Xout_16_bit_max = Xout_16_bit;}
            if (Xout_16_bit < Xout_16_bit_min) {Xout_16_bit_min = Xout_16_bit;}

            // Check to see if current sample is the maximum or minimum X-axis value
            if (Yout_16_bit > Yout_16_bit_max) {Yout_16_bit_max = Yout_16_bit;}
            if (Yout_16_bit < Yout_16_bit_min) {Yout_16_bit_min = Yout_16_bit;}

            // Check to see if current sample is the maximum or minimum X-axis value
            if (Zout_16_bit > Zout_16_bit_max) {Zout_16_bit_max = Zout_16_bit;}
            if (Zout_16_bit < Zout_16_bit_min) {Zout_16_bit_min = Zout_16_bit;}
            transmit("calibratingr\r\n");
        }
    }
    Xout_16_bit_avg = (Xout_16_bit_max + Xout_16_bit_min) / 2;    // X-axis hard-iron offset
    Yout_16_bit_avg = (Yout_16_bit_max + Yout_16_bit_min) / 2;    // Y-axis hard-iron offset
    Zout_16_bit_avg = (Zout_16_bit_max + Zout_16_bit_min) / 2;    // Z-axis hard-iron offset

    // Left-shift by one as magnetometer offset registers are 15-bit only, left justified
    Xout_16_bit_avg <<= 1;
    Yout_16_bit_avg <<= 1;
    Zout_16_bit_avg <<= 1;

    i2c0_byte_write(MAG_I2C_Address, MAG_CTRL_REG_1_Address, 0x00);  // Standby mode

    // Set Offset
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_X_LSB, (char)(Xout_16_bit_avg & 0xFF));
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_X_MSB, (char)((Xout_16_bit_avg >>8) & 0xFF));
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_Y_LSB, (char)(Yout_16_bit_avg & 0xFF));
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_Y_MSB, (char)((Yout_16_bit_avg >>8) & 0xFF));
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_Z_LSB, (char)(Zout_16_bit_avg & 0xFF));
    i2c0_byte_write(MAG_I2C_Address, MAG_OFF_Z_MSB, (char)((Zout_16_bit_avg >>8) & 0xFF));

    i2c0_byte_write(MAG_I2C_Address, MAG_CTRL_REG_1_Address, 0x11);  //  Active mode again
    transmit("\r\n- calibration finished -\r\n");
}