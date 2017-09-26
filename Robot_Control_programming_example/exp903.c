//*** exp903.c --- timer/counter1 overflow interrupt ***

#include <avr/io.h>
#include <avr/interrupt.h>

//Timer/Counter1
#define NORMAL_MODE_HIGH                0x00  // 0000 0000
#define NORMAL_MODE_LOW                 0x00  // 0000 0000
#define NORMAL_PORT_OPERATION           0x00  // 0000 0000
#define EXTERNAL_SOURCE_ON_RISING_EDGE  0x07  // 0000 0111
#define TIMER1_OVERFLOW_INT_ENABLE      0x04  // 0000 0100	

//Pulse, switch
#define PF_7_HIGH	0x80
#define PF_7_LOW	0x00	
#define SW0		0x01

void initialize_PORT();
void initialize_TCNT1();
void initialize_INTERRUPT();
void delay1();
void delay2();

int  main(void )
{
     initialize_PORT();
     initialize_TCNT1();
     initialize_INTERRUPT();

     sei();
	 		
     while(1){
        PORTF = PF_7_HIGH;  // 주기측정값 54μs*65536=3539ms후에 int. 발생 
        delay1();
        PORTF = PF_7_LOW;
        delay1();                                         
        PORTA = TCNT1>>8;   		
     }

}

void initialize_PORT()
{
     DDRA = 0xFF;  // LED 
     DDRB = 0xFF;  // 7-seg. LED
     DDRD = 0x00;  // return switch
     DDRF = 0xFF;  // pulse 출력

     PORTA=PORTB=0x00;  // count값 표시
     PORTF=0x00;
}

void initialize_TCNT1()
{
     TCCR1A = NORMAL_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = NORMAL_MODE_HIGH| EXTERNAL_SOURCE_ON_RISING_EDGE;
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER1_OVERFLOW_INT_ENABLE;  
}

ISR(TIMER1_OVF_vect)
{
    while(1){
       PORTA = 0x00;
       delay2();
       PORTA = 0xFF;
       delay2();
       if((PIND & SW0)!=0x00) return;
    }
}

void delay1()
{
     unsigned int t1;

     t1=10;
     while(t1--);
}

void delay2()
{
     unsigned long t2;

     t2=300000;
     while(t2--) if ((PORTD & SW0)!=0x00) return;
}

