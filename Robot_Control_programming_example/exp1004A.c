//*** exp1004A.c --- Fast PWM방식 이용***
//               --- interrupt를 사용하지 않고, register설정만으로 가능. 장점!  

#include <avr/io.h>
#include <avr/interrupt.h>

const unsigned int ocr1_table[]={5999,19999};       // 6000,20000
const unsigned int ocr3_table[]={7999, 6999,19999}; // 8000,7000,20000

#define PF_7_HIGH	 0x80
#define PF_7_LOW	 0x00	

//Timer1
#define FAST_PWM_MODE_HIGH             0x18  // Mode-14	
#define FAST_PWM_MODE_LOW              0x02
#define OUTPUT_HIGH_ON_COMPARE_MATCH   0xC0
#define PRESCALE_8                     0x02  //2MHz(0.5μs)


//Timer3
#define CTC_MODE_HIGH  	               0x08  //0001 0000 Mode-4 => Mode-14
#define CTC_MODE_LOW   	               0x00  //0000 0000
#define OUTPUT_HIGH_ON_COMPARE_MATCH   0xC0

#define FAST_PWM_MODE_HIGH             0x18  	
#define FAST_PWM_MODE_LOW              0x02
#define OUTPUT_LOW_ON_COMPARE_MATCH    0x80
#define TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10
#define TIMER3_OUTPUT_COMPARE_A_MATCH_INT_DISABLE 0x00

void initialize_PORT();
void initialize_TIMER13_PULSE_GENERATION();
void initialize_INTERRUPT();
void delay();


int  main(void )
{  
     initialize_PORT();
     initialize_TIMER13_PULSE_GENERATION();
     initialize_INTERRUPT();
	  
     sei();
    
     while(1){
        PORTF = PF_7_HIGH;
        delay();
        PORTF = PF_7_LOW;
        delay();
     }
}

void initialize_PORT()
{
     DDRB = DDRE = 0xFF;   // 생략안됨 - OC1A(PB.5), OC3A(PE.3) 출력
     DDRF = 0x80;
     PORTF= 0x00;
}

void initialize_TIMER13_PULSE_GENERATION()
{
     TCCR1A = FAST_PWM_MODE_LOW  | OUTPUT_HIGH_ON_COMPARE_MATCH ;
     TCCR1B = FAST_PWM_MODE_HIGH | PRESCALE_8 ;
     OCR1A  = ocr1_table[0];  // Low ->High
     ICR1   = ocr1_table[1];  // High->Low (period)

     TCCR3A = CTC_MODE_LOW  | OUTPUT_HIGH_ON_COMPARE_MATCH ;
     TCCR3B = CTC_MODE_HIGH | PRESCALE_8 ;
     OCR3A  = ocr3_table[0];   // 최초의 Low->High(rising edge)만 interrupt를 이용

     TCNT1 = TCNT3 = 0x0000;
}

void initialize_INTERRUPT()
{
     ETIMSK = TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;
}

ISR(TIMER3_COMPA_vect)
{  
     ETIMSK = TIMER3_OUTPUT_COMPARE_A_MATCH_INT_DISABLE; //이후 pulse발생은 No interrupt
    
     TCCR3A = FAST_PWM_MODE_LOW  | OUTPUT_LOW_ON_COMPARE_MATCH ;
     TCCR3B = FAST_PWM_MODE_HIGH | PRESCALE_8 ;
     OCR3A  = ocr3_table[1];  // Low ->High
     ICR3   = ocr3_table[2];  // High->Low (period)
}

void delay()
{
     unsigned int i;

     i=11500;
     while(i--);
}

