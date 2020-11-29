/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 28/Nov/2020
 * Function: ADC module source
 * Email: w20014215@northumbria.ac.uk
********************************/

#include <MKL25Z4.H>

void ADC_init(void){
	
	SIM->SCGC5 |=SIM_SCGC5_PORTC_MASK;			//enable portB
	PORTC->PCR[0] = PORT_PCR_MUX(0); 			//enable PB0 pin simulation function
	
	SIM->SCGC6 |= 0x8000000;					//clock to ADC0
	ADC0->CFG1 = 0x40 | 0x00 | 0x00 | 0x00 ;	//clock divide by 4, short sample time, single-ended 8-bit conversion, bus clock
	ADC0->SC2 &= ~0x40;							//software trigger
}

unsigned char adc_read(){
	ADC0->SC1[0] = 14;              // choose conversion channel PTB0
	while (!(ADC0->SC1[0] & 0x80));   // wait for conversion complete
	return ADC0->R[0];
}
