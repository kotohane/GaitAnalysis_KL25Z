/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 14/Nov/2020
 * File: PWM-LED module source
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>
#include "pwmled.h"

void led_init(){
    SIM->SCGC5 |= 1<<12; //enable portD
    SIM->SCGC5 |= 1<<10; //enable portB
    
    PORTD->PCR[1] = 0x0400;  //blue portD used by TPM0 ch1
    PORTB->PCR[18] = 0X0300; //red portB used by TPM2 ch0
    PORTB->PCR[19] = 0X0300; //green portB used by TPM2 ch1

    SIM->SCGC6 |= 1<<24; //enable TPM0
    SIM->SCGC6 |= 1<<26; //enable TPM2
    SIM->SOPT2 |= 0x01000000; //choose clock
    //blue
    TPM0->SC = 0;
    TPM0->CONTROLS[1].CnSC = 0x28;
    TPM0->MOD = MAX_INTENSITY-1;
    TPM0->CONTROLS[1].CnV = MAX_INTENSITY;
    TPM0->SC = 0x08;
	
    //red&green
    TPM2->SC = 0;
    TPM2->CONTROLS[0].CnSC = 0x28;
	TPM2->CONTROLS[1].CnSC = 0x28;
	TPM2->MOD = MAX_INTENSITY-1;
    TPM2->CONTROLS[0].CnV = MAX_INTENSITY;
    TPM2->CONTROLS[1].CnV = MAX_INTENSITY;
    TPM2->SC = 0x08;
}

void set_intensity(int intensity, int ledtype){
    if(ledtype == RED){
		TPM2->CONTROLS[0].CnV = MAX_INTENSITY-intensity;
	}
	if(ledtype == GREEN){
		TPM2->CONTROLS[1].CnV = MAX_INTENSITY-intensity;
	}
	if(ledtype == BLUE){
		TPM0->CONTROLS[1].CnV = MAX_INTENSITY-intensity;
	}
}
