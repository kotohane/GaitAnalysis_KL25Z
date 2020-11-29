/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 20/Nov/2020
 * Function: I2C module source
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>
#include "i2c.h"

void i2c0_init(){
    SIM->SCGC4 |= 0x40;     //turn on clock to I2C0 SIM_SCGC4_I2C0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;    //enable portB clock
    //set pins to I2C function
    PORTB->PCR[0] |= PORT_PCR_MUX(2);  //PTB0 for I2C0 SCL 
    PORTB->PCR[1] |= PORT_PCR_MUX(2); //PTB1 for SDA 

    I2C0->F = 0x14;  //set SCL clock rate 
    I2C0->C1 = 0x00; //stop I2C0 
    I2C0->S = 0x02;  //clear interrupt flag
    I2C0->C1 = 0x80; //enable i2c and set to master mode
}

void i2c1_init(){
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;     //turn on clock to I2C0 SIM_SCGC4_I2C0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;    //enable portB clock
    //set pins to I2C function
    PORTC->PCR[10] |= PORT_PCR_MUX(2);  //PTB0 for I2C0 SCL 
    PORTC->PCR[11] |= PORT_PCR_MUX(2); //PTB1 for SDA 
	//PORTB->PCR[2] |= PORT_PCR_MUX(2);  //PTB0 for I2C0 SCL 
    //PORTB->PCR[3] |= PORT_PCR_MUX(2); //PTB1 for SDA 

    I2C1->F = 0x14;  //set SCL clock rate 
    I2C1->C1 = 0x00; //stop I2C0 
    I2C1->S = 0x02;  //clear interrupt flag
    I2C1->C1 = 0x80; //enable i2c and set to master mode
}

void i2c0_start(){
    I2C0_C1 |= 1<<4;  //transmit
    I2C0_C1 |= 1<<5;  //master mode
}
void i2c1_start(){
    I2C1_C1 |= 1<<4;  //transmit
    I2C1_C1 |= 1<<5;  //master mode
}
void i2c0_restart(){
    I2C0->C1 |= 0x04;
}
void i2c1_restart(){
    I2C1->C1 |= 0x04;
}
void i2c0_wait(){
    while((I2C0_S & 0x02)==0);
    I2C0_S |= 0x02;  //clear IF
}
void i2c1_wait(){
    while((I2C1_S & 0x02)==0);
    I2C1_S |= 0x02;  //clear IF
}
void i2c0_stop(){
    I2C0_C1 &= ~0x20; //cancel master mode; 
    I2C0_C1 &= ~0x10;//cancel transmit
}
void i2c1_stop(){
    I2C1_C1 &= ~0x20; //cancel master mode; 
    I2C1_C1 &= ~0x10;//cancel transmit
}
void delay(int number){
    int cnt;
	for(cnt=0; cnt<number; cnt++);
}

int i2c0_byte_write(char SlaveAddr, char RegAddr,char DataByte){
	int retry = 1000;
	while (I2C0->S & 0x20) { // wait until bus is available
		if (--retry <= 0)
		return ERR_BUS_BUSY;
		delay(10);
	}
    i2c0_start();	        //send start
 	I2C0->D = SlaveAddr<<1;	//send A6-A0 slave addr + 0(write mode)
    i2c0_wait();	        //wait for ACK	
 	if(I2C0->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	I2C0->D = RegAddr;		//send register address to write
    i2c0_wait();             //wait for ACK
 	if(I2C0->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	I2C0->D = DataByte;     //send data
  	i2c0_wait();             //wait for ACK
 	if(I2C0->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	i2c0_stop();             //send stop
 	delay(50);              //make some delay
 	return ERR_NONE;
}

int i2c1_byte_write(char SlaveAddr, char RegAddr,char DataByte){
	int retry = 1000;
	while (I2C1->S & 0x20) { // wait until bus is available
		if (--retry <= 0)
		return ERR_BUS_BUSY;
		delay(10);
	}
    i2c1_start();	        //send start
 	I2C1->D = SlaveAddr<<1;	//send A6-A0 slave addr + 0(write mode)
    i2c1_wait();	        //wait for ACK	
 	if(I2C1->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	I2C1->D = RegAddr;		//send register address to write
    i2c1_wait();             //wait for ACK
 	if(I2C1->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	I2C1->D = DataByte;     //send data
  	i2c1_wait();             //wait for ACK
 	if(I2C1->S & 0x01){    //I2C0_S[0]==1 => No ACK signal received
        return ERR_NO_ACK;
    }
 	i2c1_stop();             //send stop
 	delay(50);              //make some delay
 	return ERR_NONE;
}

unsigned char i2c0_byte_read(unsigned char SlaveAddr,unsigned char RegAddr){	
    unsigned char result;
 	i2c0_start();	           //send start    
 	I2C0->D = SlaveAddr<<1;	   //send slave addr, write
 	i2c0_wait();                //wait for ACK
	I2C0->D = RegAddr;          //send register address
 	i2c0_wait();                //wait for ACK
 	i2c0_restart();             //send restart
 	I2C0->D = (SlaveAddr<<1)|0x01;//send slave addr, receive
 	i2c0_wait();                //wait for ACK
 	I2C0->C1 &= ~0x10;          //receive TX
 	I2C0->C1 |= 0x08;           //disable ACK
 	result = I2C0->D;          //dummy read
 	i2c0_wait();                //wait for NACK
 	i2c0_stop();                //send stop
 	result = I2C0->D;          //read received data
 	delay(50);
 	return result;
}

char i2c1_byte_read(char SlaveAddr, char RegAddr){	
    char result;
 	i2c1_start();	           //send start    
 	I2C1->D = SlaveAddr<<1;	   //send slave addr, write
 	i2c1_wait();                //wait for ACK
	I2C1->D = RegAddr;          //send register address
 	i2c1_wait();                //wait for ACK
 	i2c1_restart();             //send restart
 	I2C1->D = (SlaveAddr<<1)|0x01;//send slave addr, receive
 	i2c1_wait();                //wait for ACK
 	I2C1->C1 &= ~0x10;          //receive TX
 	I2C1->C1 |= 0x08;           //disable ACK
 	result = I2C1->D;          //dummy read
 	i2c1_wait();                //wait for NACK
 	i2c1_stop();                //send stop
 	result = I2C1->D;          //read received data
 	delay(50);
 	return result;
}

void i2c0_burst_read(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char n, unsigned char *buff){
	char i;
	i2c0_start();	     	//send start     
	I2C0->D = SlaveAddr<<1; //send slave addr, write
	i2c0_wait();				//wait for ACK
	I2C0->D = RegAddr;		//send register address
	i2c0_wait();				//wait for ACK
	i2c0_restart();			//send restart
	I2C0->D = (SlaveAddr << 1) | 0x01;//send slave addr, receive
	i2c0_wait();				//wait for ACK
	I2C0->C1 &= ~I2C_C1_TX_MASK;	//receive TX
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;	//enable Ack
	i = I2C0->D;			//dummy read
	i2c0_wait();
	for(i=0; i<n-2; i++){
	    *buff++ = I2C0_D;
	    i2c0_wait();
	}
	I2C0_C1 |= I2C_C1_TXAK_MASK; //disable Ack
	*buff++ = I2C0->D;
	i2c0_wait();
	i2c0_stop();
	*buff = I2C0->D;
	delay(50);		
}
void i2c1_burst_read(unsigned char SlaveAddr, unsigned char RegAddr, unsigned char n, unsigned char *buff){
	char i;
	i2c1_start();	     	//send start     
	I2C1->D = SlaveAddr<<1; //send slave addr, write
	i2c1_wait();				//wait for ACK
	I2C1->D = RegAddr;		//send register address
	i2c1_wait();				//wait for ACK
	i2c1_restart();			//send restart
	I2C1->D = (SlaveAddr << 1) | 0x01;//send slave addr, receive
	i2c1_wait();				//wait for ACK
	I2C1->C1 &= ~I2C_C1_TX_MASK;	//receive TX
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;	//enable Ack
	i = I2C1->D;			//dummy read
	i2c1_wait();
	for(i=0; i<n-2; i++){
	    *buff++ = I2C1_D;
	    i2c1_wait();
	}
	I2C1_C1 |= I2C_C1_TXAK_MASK; //disable Ack
	*buff++ = I2C1->D;
	i2c1_wait();
	i2c1_stop();
	*buff = I2C1->D;
	delay(50);		
}

