// *** exp1002.c --- Timer1 CTC mode 이용; duty ratio=50%(반복)일 때 편리 ***
//               --- interrupt 사용하지 않고 register 설정만으로 가능!!   ***
//                   => program 유연성 커짐                               *** 

#include <avr/io.h>
#include <avr/interrupt.h>

#define PF_7_HIGH  0x80
#define PF_7_LOW   0x00	

//Timer1
#define CTC_MODE_HIGH  0x08   // Mode-4
#define CTC_MODE_LOW   0x00
#define PRESCALE_64    0x03   // 250kHz(4μs)
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH    0x40

void initialize_PORT();
void initialize_TIMER1_PULSE_GENERATION();
void delay();

int  main(void)
{   
     initialize_PORT();
     initialize_TIMER1_PULSE_GENERATION();
    
     while(1){
        PORTF = PF_7_HIGH;
        delay();
        PORTF = PF_7_LOW;
        delay();
     } 
}

void initialize_PORT()
{
     DDRB=0xFF;  // PB5(OC1A) output
     DDRF=0x80;
}

void initialize_TIMER1_PULSE_GENERATION()
{
     TCCR1A = CTC_MODE_LOW  | OUTPUT_TOGGLE_ON_COMPARE_MATCH;
     TCCR1B = CTC_MODE_HIGH | PRESCALE_64;

     OCR1A = 625-1;  // timer1 toggle 반복주기 4us*625=2.5ms(pulse주기=5ms), duty ratio=50% 
}

void delay()
{
     unsigned int td=11500;

     while(td--); 
}


