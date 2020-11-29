/*******************************
 * Project: TiltIndicator
 * Author: Ding Wang
 * Created: 14/Nov/2020
 * File: UART module header 
 * Email: w20014215@northumbria.ac.uk
********************************/

void UART0_init (void);

char receive(void);

void transmit_char(char ch);
void transmit(char* string);
void transmit_enter(void);
void transmit_int(int num);
void transmit_bytes(char datatype, int data);
