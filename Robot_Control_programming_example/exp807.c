//*** exp807.c --- interrupt priority ***

#include <avr/io.h>
#include <avr/interrupt.h>
       
#define INT0_RISING_EDGE_TRIGGER  0x03
#define INT1_RISING_EDGE_TRIGGER  0x0C 
#define EXT_INT0_ENABLE           0x01 
#define EXT_INT1_ENABLE           0x02 

#define SW7     0x80  	

void initialize_PORT();
void initialize_INTERRUPT();
void delay();

int main(void)
{
    unsigned char ii;	
 
    initialize_PORT();
    initialize_INTERRUPT();

    while((PIND & SW7)==0x00) PORTB= (EIMSK<<4) | EIFR;
//  INT0과 INT1을 입력한 후(2개의 int가 입력되어 대기상태), 
//  SW7을 누르면 어떤 순서로 실행할까? 
    sei();  

    while(1){
       for(ii=0x01;ii<=0x05;ii++){
           PORTA=ii;
	   delay();
       }
    }
}

void initialize_PORT()
{
     DDRA  = 0xFF;  // LED 출력
     DDRB  = 0xFF;  // 7-seg. 출력
     DDRD  = 0x00;
     PORTA = 0x00;
     PORTB = 0x00;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER|INT1_RISING_EDGE_TRIGGER; 
     EIMSK = EXT_INT0_ENABLE|EXT_INT1_ENABLE;
}

void delay()
{    
     volatile unsigned int ta,tb,i,j;

     ta=10; tb=50000;
     for(i=0; i<ta; i++)
         for(j=0; j<tb; j++);
}

ISR(INT0_vect)
{
    unsigned char jj;

    PORTB= (EIMSK<<4) | EIFR;
    for(jj=0x06; jj<=0x0A; jj++){
        PORTA = jj;
        delay();
    }
}

ISR(INT1_vect)
{
    unsigned char kk;

    PORTB= (EIMSK<<4) | EIFR;
    for(kk=0x0b; kk<=0x0f; kk++){
        PORTA = kk;
        delay();
    }
}


