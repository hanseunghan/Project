//*** exp1001.c ***

#include <avr/io.h>

#define NORMAL_MODE_HIGH  0x00
#define NORMAL_MODE_LOW   0x00
#define PRESCALE_8	  0x02   // 2MHz(0.5μs)
#define OC1A_HIGH_ON_COMPARE_MATCH   0xC0
#define OC1B_HIGH_ON_COMPARE_MATCH   0x30
#define OC1C_HIGH_ON_COMPARE_MATCH   0x0C

#define PF_7_HIGH	  0x80
#define PF_7_LOW	  0x00

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
     DDRB = 0xFF; // 생략하면 안됨! OC1A(PB.5),OC1B(PB.6),OC1C(PB.7) 모두 출력으로!
     DDRF = 0x80;
//   PORTB = 0x00; OC1A,B,C의 초기값은 Low, 생략가능
}

void initialize_TIMER1_PULSE_GENERATION()
{
     TCCR1A = NORMAL_MODE_LOW  | OC1A_HIGH_ON_COMPARE_MATCH
             |OC1B_HIGH_ON_COMPARE_MATCH | OC1C_HIGH_ON_COMPARE_MATCH; 
     TCCR1B = NORMAL_MODE_HIGH | PRESCALE_8;

     OCR1A=3;        // 즉시 High (최소 클럭은 1이 아닌 3)
     OCR1B=20000-1;  // 20000(10ms)후 High
     OCR1C=40000-1;  // 40000(20ms)후 High
}

void delay()
{
     unsigned int td=684;

     while(td--);
}
