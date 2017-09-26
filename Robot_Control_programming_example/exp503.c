// *** exp503.c ***

#include <avr/io.h>

//DC motor
#define CCW_ROTATION   0xA0   
#define CW_ROTATION    0x60   
#define QUICK_STOP     0x20   
#define FREE_STOP      0x00   

//Timer0
#define PWM_MODE                    0x00 
#define FAST_PWM_MODE               0x48  
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 
#define PRESCALE_8                  0x02  
#define DUTYRATIO_MIN    0x00   // 실제 실험치는 0x20(13%) 또는 0x30(19%)으로!
#define DUTYRATIO_MAX    0xFF

void initialize_PORT();
void initialize_TIMER0_PWM();
void pulse_out(unsigned char duty_Ratio);
void delay();

int  main(void)
{
     unsigned int duty_Ratio;    

     initialize_PORT();
     initialize_TIMER0_PWM();

     PORTB = CCW_ROTATION;   // set direction of ratation of DC motor
     
     while (1) {
        for(duty_Ratio=DUTYRATIO_MIN;duty_Ratio<DUTYRATIO_MAX;duty_Ratio++) {
            pulse_out(duty_Ratio); 
            delay();
        }
        for(duty_Ratio=DUTYRATIO_MAX;duty_Ratio>DUTYRATIO_MIN;duty_Ratio--) {
            pulse_out(duty_Ratio); 
            delay(); 
        }
     }
}

void initialize_PORT()
{
     DDRA = 0xFF;
     DDRB = 0xFF;
     PORTA= 0x00;
     PORTB= 0x00;
}

void initialize_TIMER0_PWM()
{
     TCCR0 = PWM_MODE|FAST_PWM_MODE|OUTPUT_LOW_ON_COMPARE_MATCH|PRESCALE_8; 
//   TCNT0 = 0x00; 
}

void pulse_out(unsigned char duty_Ratio)
{
     OCR0  = duty_Ratio;
     PORTA = duty_Ratio;   
} 
                
void delay(void)
{
     unsigned int ta,i;

     ta=30000; 
     for (i=0; i<ta; i++);  
}
