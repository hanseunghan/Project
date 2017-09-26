//*** exp901.c --- timer/counter2의 외부클럭 counting***

#include <avr/io.h>

#define NORMAL_MODE                     0x00  // 0000 0000
#define NORMAL_PORT_OPERATION           0x00  // 0000 0000
#define EXTERNAL_CLOCK_ON_RISING_EDGE   0x07  // 0000 0111
#define EXTERNAL_CLOCK_ON_FALLING_EDGE  0x06  // 0000 0110
#define SW0    0x01
#define CLEAR  0x00

void initialize_PORT();
void initialize_TCNT2();

int  main(void)
{
     initialize_PORT();
     initialize_TCNT2();
 
     while(1){
        PORTA=PORTB=TCNT2;
        if((PIND & SW0) != 0x00) TCNT2 = CLEAR;
     }
}

void initialize_PORT()
{
     DDRA = 0xFF;  // LED
     DDRB = 0xFF;  // 7-seg. LED 
     DDRD = 0x00;  // reset switch

     PORTA=PORTB=0x00;  // count값 표시
}

void initialize_TCNT2()
{
     TCCR2 = NORMAL_MODE|NORMAL_PORT_OPERATION|EXTERNAL_CLOCK_ON_RISING_EDGE;
}

