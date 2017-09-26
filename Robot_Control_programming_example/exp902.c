//*** exp902.c --- timer/counter2 overflow int ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int timer2_ovf_number;

//Timer/Counter2
#define NORMAL_MODE                    0x00  // 0000 0000
#define NORMAL_PORT_OPERATION          0x00  // 0000 0000
#define EXTERNAL_CLOCK_ON_RISING_EDGE  0x07
#define TIMER2_OVERFLOW_INT_ENABLE     0x40  // 0100 0000

//pulse
#define PF_7_HIGH  0x80
#define PF_7_LOW   0x00

void initialize_PORT();
void initialize_TCNT2();
void initialize_INTERRUPT();
void delay();

int  main(void)
{
     initialize_PORT();
     initialize_TCNT2();
     initialize_INTERRUPT();

     timer2_ovf_number = 0x00;

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
     DDRA = 0xFF;  // LED 
     DDRB = 0xFF;  // 7-seg. LED
     DDRF = 0xFF;  // pulse 출력

     PORTA=PORTB=0x00;  // count값 표시
}

void initialize_TCNT2()
{
     TCCR2 = NORMAL_MODE|NORMAL_PORT_OPERATION|EXTERNAL_CLOCK_ON_RISING_EDGE;
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER2_OVERFLOW_INT_ENABLE;  
}

ISR(TIMER2_OVF_vect)
{
    timer2_ovf_number++;
    PORTA=PORTB=timer2_ovf_number;	
}

void delay()
{
     unsigned char ti;

     ti=100;
     while(ti--);
}

