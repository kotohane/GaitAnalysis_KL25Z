/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 28/Nov/2020
 * Component: IMU mpu6050 source
 * Email: w20014215@northumbria.ac.uk
********************************/

#include "imu.h"
#include "../functions/uart.h"
#include "../functions/i2c.h"

void imu_init(){
	i2c0_byte_write(IMU_I2C_Address, IMU_PWR_MGMT_1_Address,0x00);
    //output rate = 1kHz
    i2c0_byte_write(IMU_I2C_Address, IMU_SMPLRT_DIV_Address,0x07);
    //accelerometer scale +-2g
    i2c0_byte_write(IMU_I2C_Address, IMU_ACCEL_Config_Address,0x00);
    //gyroscope scale +-250/s
	i2c0_byte_write(IMU_I2C_Address, IMU_GYRO_Config_Address,0x00);
	unsigned char regval = i2c0_byte_read(IMU_I2C_Address, 0x30);
	if(regval==0x50){
		transmit("imu successful\r\n");
	}
	else{
		transmit("imu fail\r\n");
	}
}

int read_imu(int* buff){
	unsigned char acce_data[6];
	i2c0_burst_read(IMU_I2C_Address, IMU_ACCEL_Output_Address, 6, acce_data);
	buff[0] = ((short)(acce_data[0]<<8 | acce_data[1]));
	buff[1] = ((short)(acce_data[2]<<8 | acce_data[3]));
	buff[2] = ((short)(acce_data[4]<<8 | acce_data[5]));
	unsigned char gyro_data[6];
	i2c0_burst_read(IMU_I2C_Address, IMU_GYRO_Output_Address, 6, gyro_data);
	buff[3] = ((short)(gyro_data[0]<<8 | gyro_data[1]));
	buff[4] = ((short)(gyro_data[2]<<8 | gyro_data[3]));
	buff[5] = ((short)(gyro_data[4]<<8 | gyro_data[5]));
	return 1; 	//read secceed
}