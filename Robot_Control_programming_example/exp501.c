//*** exp501.c --- Timer0, Fast PWM Mode***

#include <avr/io.h>

#define PWM_MODE                    0x00 // 0000 0000  ; 생략가능
#define FAST_PWM_MODE               0x48 // 0100 1000  ; Mode-3
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 // 0010 0000
#define PRESCALE_8                  0x02 // 0000 0010 
                                    
#define DUTYRATIO                   0x80 // 50%  0x10->0x40->0x80->0xC0->0xFF

void initialize_PORT();
void initialize_TIMER0_PWM();

int  main(void)
{
     initialize_PORT();
     initialize_TIMER0_PWM();

     PORTA=DUTYRATIO; // f=7.8125kHz;Tp=128μs ～ PWM파형의 frequency & period
     while(1);        // PWM_frequency = clock/(prescale*256)
}

void initialize_PORT()
{
     DDRB = 0xFF;   // OC0(PB.4) 출력으로 설정 필수!!
     DDRA = 0xFF;   // duty ratio output

     PORTA= 0x00;
}

void initialize_TIMER0_PWM()
{
     TCCR0= PWM_MODE|FAST_PWM_MODE|OUTPUT_LOW_ON_COMPARE_MATCH|PRESCALE_8;  
     OCR0 = DUTYRATIO;    // set_duty_ratio
//   TCNT0= 0x00;
}
