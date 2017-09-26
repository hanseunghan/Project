//*** exp905.c --- timer/counter1 ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int timer1_ovf_number;

//Timer1
#define NORMAL_MODE_HIGH            0x00  // 0000 0000
#define NORMAL_MODE_LOW             0x00  // 0000 0000
#define NORMAL_PORT_OPERATION       0x00  // 0000 0000
#define PRESCALE_8                  0x02  // 0000 0010
#define TIMER1_OVERFLOW_INT_ENABLE  0x04  // 0000 0100
//Pulse
#define PF_7_HIGH   0x80
#define PF_7_LOW    0x00	

void initialize_PORT();
void initialize_TIMER1();
void initialize_INTERRUPT();
void delay();

int  main(void )
{
     initialize_PORT();
     initialize_TIMER1();
     initialize_INTERRUPT();

     timer1_ovf_number = 0x00;

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

     PORTA=PORTB=0x00;  // overflow count값 표시
     PORTF=0x00;
}

void initialize_TIMER1()
{
     TCCR1A = NORMAL_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = NORMAL_MODE_HIGH| PRESCALE_8;
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER1_OVERFLOW_INT_ENABLE;  
}

ISR(TIMER1_OVF_vect)
{
    timer1_ovf_number++;
    PORTA = PORTB = timer1_ovf_number;	// 주기 65.5ms
}

void delay()
{
     unsigned int ti;

     ti=23000;
     while(ti--);
}



