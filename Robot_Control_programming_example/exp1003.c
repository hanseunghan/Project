//*** exp1003.c --- Fast PWM mode 이용***
//              --- interrupt를 사용하지 않고, register설정만으로 가능. 장점!  

#include <avr/io.h>
#include <avr/interrupt.h>

const unsigned int ocr1_table[]={5999, 19999};  // 6000, 20000

#define PF_7_HIGH	 0x80
#define PF_7_LOW	 0x00	

#define FAST_PWM_MODE_HIGH             0x18  // 0001 1000  Mode-14	
#define FAST_PWM_MODE_LOW              0x02  // 0000 0010
#define OUTPUT_HIGH_ON_COMPARE_MATCH   0xC0  // 1100 0000
#define PRESCALE_8                     0x02  // 0000 0010  2MHz(0.5μs)

void initialize_PORT();
void initialize_TIMER1_PULSE_GENERATION();
void delay();

int  main(void )
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
     DDRB = 0xFF;   // OC1A(PB.5) 출력
     DDRF = 0x80;
     PORTF= 0x00;
}

void initialize_TIMER1_PULSE_GENERATION()
{
     TCCR1A = FAST_PWM_MODE_LOW  | OUTPUT_HIGH_ON_COMPARE_MATCH ;
     TCCR1B = FAST_PWM_MODE_HIGH | PRESCALE_8 ;

     OCR1A= ocr1_table[0];  // Low ->High
     ICR1 = ocr1_table[1];  // High->Low (period)

//   TCNT1= 0x0000;
}

void delay()
{
     unsigned int td=11500;

     while(td--);
}



