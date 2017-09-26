//*** exp906.c --- Timer/Counter1, Timer/Counter2 ***

#include <avr/io.h>
#include <avr/interrupt.h>

//Timer/Counter1
#define NORMAL_MODE_HIGH                0x00  // 0000 0000
#define NORMAL_MODE_LOW                 0x00  // 0000 0000
#define NORMAL_PORT_OPERATION           0x00  // 0000 0000
#define EXTERNAL_SOURCE_ON_RISING_EDGE  0x07  // 0000 0111
#define TIMER1_OVERFLOW_INT_ENABLE      0x04  // 0000 0100	

//Timer/Counter2
#define NORMAL_MODE                    0x00  // 0000 0000
#define NORMAL_PORT_OPERATION          0x00  // 0000 0000
#define EXTERNAL_CLOCK_ON_RISING_EDGE  0x07
#define TIMER2_OVERFLOW_INT_ENABLE     0x40  // 0100 0000

//Pulse
#define PF_7_HIGH  0x80
#define PF_7_LOW   0x00	

void initialize_PORT();
void initialize_TCNT1();
void initialize_TCNT2();
void initialize_INTERRUPT();
void delay();

int main(void )
{
    initialize_PORT();
	initialize_TCNT1();
    initialize_TCNT2();
    initialize_INTERRUPT();

    sei();
		
    while(1){
       PORTF = PF_7_HIGH;// 주기 측정값 19.6us
       delay();
       PORTF = PF_7_LOW;
       delay();
    }
}

void initialize_PORT()
{
     DDRA = 0xFF;  // LED 
     DDRB = 0xFF;  // 7-seg. LED
     DDRF = 0xFF;  // pulse 출력

     PORTA=PORTB=0x00; 
     PORTF=0x00;
}

void initialize_TCNT1()
{
     TCCR1A = NORMAL_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = NORMAL_MODE_HIGH| EXTERNAL_SOURCE_ON_RISING_EDGE;
}

void initialize_TCNT2()
{
     TCCR2 = NORMAL_MODE|NORMAL_PORT_OPERATION|EXTERNAL_CLOCK_ON_RISING_EDGE;
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER2_OVERFLOW_INT_ENABLE|TIMER1_OVERFLOW_INT_ENABLE;  
}

void delay()
{
     unsigned char ti;

     ti=5;
     while(ti--);
}

ISR(TIMER1_OVF_vect)
{
    PORTA=PORTA^0x02;
    PORTB=PORTB^0x02;   // 주기 측정값 19.6*65536*2 = 2569ms
}

ISR(TIMER2_OVF_vect)
{
    PORTA=PORTA^0x04;
    PORTB=PORTB^0x04;   // 주기 측정값 19.6*256*2 = 10.035ms
}

