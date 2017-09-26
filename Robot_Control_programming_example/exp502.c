// *** exp502.c ***

#include <avr/io.h>

#define PWM_MODE                    0x00 
#define FAST_PWM_MODE               0x48 
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 
#define PRESCALE_8                  0x02 

#define DUTYRATIO_MIN   	0x00  
#define DUTYRATIO_MAX   	0xFF

void initialize_PORT();
void initialize_TIMER0_PWM();
void pulse_out(unsigned char duty_Ratio);
void delay();

int  main(void)
{
     unsigned int duty_Ratio;   //  unsigned char duty_Ratio; ?

     initialize_PORT();
     initialize_TIMER0_PWM();
 
     while(1){
          for(duty_Ratio=DUTYRATIO_MIN;duty_Ratio<=DUTYRATIO_MAX; duty_Ratio++){
              pulse_out(duty_Ratio);
              delay(); 
          }
     }
}

void initialize_PORT()
{
     DDRB = 0xFF;   
     DDRA = 0xFF;   
     PORTA= 0x00;
}

void initialize_TIMER0_PWM()
{
     TCCR0 = PWM_MODE|FAST_PWM_MODE|OUTPUT_LOW_ON_COMPARE_MATCH|PRESCALE_8; 
     TCNT0 = 0x00; 
}

void pulse_out(unsigned char duty_Ratio)
{
     OCR0  = duty_Ratio;     // set_duty_ratio 
     PORTA = duty_Ratio;     
}                             

void delay()
{
     unsigned int ta,i;

     ta=20000; 
     for (i=0; i<ta; i++);
}
