/*******************************
 * Project: TiltIndicator
 * Author: Ding Wang
 * Created: 14/Nov/2020
 * File: UART module source
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>

char outputstack[1000];
int absu(int a){
	if(a<0) return -a;
	return a;
}

void delayms(int delay){
    for(int i=0;i<delay;i++){
        for(int j=0;j<7000;j++);
    }
}

void UART0_init (void){
	SIM ->SCGC4 |= 0x0400 ; // enable clock for UART0
	SIM ->SOPT2 |= 0x04000000 ; // choose clock source
	UART0 ->C2 = 0 ; // turn off UART0 while changing configurations */
	UART0 ->BDH = 0x01;
	UART0 ->BDL = 0x11 ; //57600 Baud rate
	UART0 ->C4 = 0x07 ; // Over Sampling Ratio 16
	
	UART0 ->C1 = 0x00 ; // 8 bit data
	UART0 ->C2 = 0b00001100 ; // enable transmit
	SIM ->SCGC5 |= 0x0200; // enable clock for PORTA
	PORTA ->PCR[1] = 0x0200; // make PTA1 UART0_Rx pin
	PORTA ->PCR[2] = 0x0200; // make PTA2 UART0_Tx pin
}

void UART0_Tx(uint8_t data){
	while(!(UART0 ->S1 & 0x80));
	UART0 ->D = data;
	delayms(1);
}

char receive(void){
	if((UART0 ->S1 & 0x20)){
		return UART0->D;
	}
	return 0;
}

void transmit_enter(void){
	UART0_Tx('\r');
	UART0_Tx('\n');
}

void transmit(char* string){
    for(int i=0;string[i]!='\0';i++){
        UART0_Tx(string[i]);
    }
}

void transmit_char(uint8_t ch){
	UART0_Tx(ch);
}
void transmit_int(int num){
	if(num==0){
        UART0_Tx('0');
        return;
    }    
	if(num<0){
		UART0_Tx('-');
		num = -num;
	}
    int output_flag=0;
    while(num){
        outputstack[output_flag++] = '0' + num%10;
        num/=10;
    }
    for(int i=output_flag-1;i>=0;i--){
        UART0_Tx(outputstack[i]);
    }
}

void transmit_bytes(char datatype, int data){
	UART0_Tx(datatype);
	if(data>=0){
		UART0_Tx('+');
	}
	else{
		UART0_Tx('-');
	}
	char firstbyte = absu(data)/256;
	char secondbyte = absu(data)%256;
	UART0_Tx(firstbyte);
	UART0_Tx(secondbyte);
}
