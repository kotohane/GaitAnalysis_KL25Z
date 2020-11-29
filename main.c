/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 20/Nov/2020
 * Function: main function
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>
#include "functions/pwmled.h"
#include "functions/uart.h"
#include "functions/i2c.h"
#include "functions/adc.h"
#include "components/magnetometer.h"
#include "components/imu.h"


#define IMU_TYPE 0
#define MAG_TYPE 6
#define CONTROL_TYPE 9

char datatype[10] = {'x','y','z','a','b','c','l','m','n','s'};


void init(){
	led_init();
	UART0_init();
	i2c0_init();
	imu_init();
	mag_init();
	//ADC_init();
}

int main(){
	init();
	while(1);
	transmit("\r\nstart\r\n");
	//unsigned char data = 0;
	//data = adc_read();
	//transmit_int(data);
	int data_imu[6];
	int data_mag[3];
	while(1){
		if(read_imu(data_imu)){
			for(int i=0;i<6;i++){
				transmit_bytes(datatype[IMU_TYPE+i],data_imu[i]);
			}
		}
		/*if(read_mag(data_mag)){
			for(int i=0;i<3;i++){
				transmit_bytes(datatype[MAG_TYPE+i],data_mag[i]);
			}
		}*/
		//receive control signal process
		char receive_ctrl=receive();
		if(receive_ctrl!=0){
			if(receive_ctrl=='r'){
				set_intensity(50,RED);
			}
			if(receive_ctrl=='m'){
				mag_offset_calibration();
			}
		}
	}
	return 0;
}
