//*** exp907A.c --- timer1/3 compare match interrupt, overflow interrupt ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char   process1,process3;

//Timer1,Timer3
#define NORMAL_MODE_HIGH       0x00
#define NORMAL_MODE_LOW        0x00
#define NORMAL_PORT_OPERATION  0x00
#define PRESCALE_1             0x01

#define TCNT1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10  // 0001 0000
#define TCNT1_OVERFLOW_INT_ENABLE                0x04  // 0000 0100
#define TCNT3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10  // 0001 0000
#define TCNT3_OVERFLOW_INT_ENABLE                0x04  // 0000 0100

//ON/OFF process
#define MIN_COMPARE_VALUE 0x0100
#define MAX_COMPARE_VALUE 0xFF00
#define CHANGE_VALUE      0x0080  
#define FIRST_HALF       1
#define SECOND_HALF  2

#define LOW4BIT_CLEAR 	0xF0  
#define LOW4BIT_SET   	0x0F  
#define HIGH4BIT_CLEAR 	0x0F  
#define HIGH4BIT_SET	0xF0  

//Pulse out
#define PF_7_HIGH	0x80
#define PF_7_LOW	0x00

void initialize_PORT();
void initialize_TIMER1_TIMER3();
void initialize_INTERRUPT();
void delay1();

int main(void)
{
    initialize_PORT();
    initialize_TIMER1_TIMER3();
    initialize_INTERRUPT();

    sei();

    while(1){
       PORTF = PF_7_HIGH; //주기측정값 약 320us
       delay1();
       PORTF = PF_7_LOW;
       delay1();
    }
}

void initialize_PORT()
{
     DDRA = DDRB = 0xFF;
     DDRF = 0x80;

     PORTA = PORTB = 0x00; 
}
void initialize_TIMER1_TIMER3()
{
     TCCR1A = TCCR3A = NORMAL_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = TCCR3B = NORMAL_MODE_HIGH| PRESCALE_1; 

     OCR1A = MIN_COMPARE_VALUE;   // 한주기에서 LED ON time 설정
     OCR3A = MAX_COMPARE_VALUE; 
     process1 = process3 = FIRST_HALF;
//   TCNT1 = TCNT3 = 0x0000;      // 동시동작 같은값   
}

void initialize_INTERRUPT()
{
     TIMSK  = TCNT1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE | TCNT1_OVERFLOW_INT_ENABLE; 
     ETIMSK = TCNT3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE | TCNT3_OVERFLOW_INT_ENABLE;
}

void delay1()
{
     unsigned int t1;

     t1=100;
     while(t1--);
}

ISR(TIMER1_COMPA_vect)
{  
    PORTA &= LOW4BIT_CLEAR;  // 1111 0000
    PORTB &= LOW4BIT_CLEAR;  // 1111 0000
//  OCR1값과 match이후(한주기의 나머지시간)에는 Low 4bit LED들은 모두 OFF
}

ISR(TIMER1_OVF_vect)
{
    PORTA |= LOW4BIT_SET;    // 0000 1111
    PORTB |= LOW4BIT_SET;    // 0000 1111
// 새로운 주기가 시작할 때 OCR1값을 새로 설정(->ON time결정) 
// PWM 파형의 duty ratio 변경과 같음
    if     (process1==FIRST_HALF){  
            OCR1A   += CHANGE_VALUE;
            if(OCR1A== MAX_COMPARE_VALUE) process1 = SECOND_HALF;
    }
    else if(process1==SECOND_HALF){
	    OCR1A   -= CHANGE_VALUE;
            if(OCR1A== MIN_COMPARE_VALUE) process1 = FIRST_HALF;
    }
}

ISR(TIMER3_COMPA_vect)
{  
    PORTA &= HIGH4BIT_CLEAR;  // 0000 1111
//  OCR3값과 match이후(한주기의 나머지시간)에는 High 4bit LED들도 모두 OFF
}

ISR(TIMER3_OVF_vect)
{
    PORTA |= HIGH4BIT_SET;    // 1111 0000
    if     (process3==FIRST_HALF){
	    OCR3A -= CHANGE_VALUE;
	    if(OCR3A == MIN_COMPARE_VALUE) process3 = SECOND_HALF;
    }
    else if(process3==SECOND_HALF){                   
	    OCR3A += CHANGE_VALUE;
	    if(OCR3A == MAX_COMPARE_VALUE) process3 = FIRST_HALF;
    }
}