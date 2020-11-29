/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 28/Nov/2020
 * Component: IMU mpu6050 header
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>


#define IMU_I2C_Address 0xD0

#define WHO_AM_I_REG 0x75
#define IMU_PWR_MGMT_1_Address 0x6B
#define IMU_SMPLRT_DIV_Address 0x19
#define IMU_ACCEL_Config_Address 0x1C
#define IMU_ACCEL_Output_Address 0x3B
#define IMU_GYRO_Config_Address 0x1B
#define IMU_GYRO_Output_Address 0x43

void imu_init(void);
int read_imu(int* buff);
