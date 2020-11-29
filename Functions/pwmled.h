/*******************************
 * Project: Gait Analysis KL25Z
 * Author: Ding Wang
 * Created: 14/Nov/2020
 * File: PWM-LED module header
 * Email: w20014215@northumbria.ac.uk
 * 
 * use TPM0 channel 1, TPM2 channel 0, TPM2 channel 1
********************************/



#define RED 0
#define GREEN 1
#define BLUE 2

#define MAX_INTENSITY 4096

void led_init(void);
void set_intensity(int intensity, int ledtype);
