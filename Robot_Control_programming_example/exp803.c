//*** exp803.c ***

#include <avr/io.h>
#include <avr/interrupt.h>   


#define INT0_RISING_EDGE_TRIGGER   0x03 
#define EXT_INT0_ENABLE            0x01 

void initialize_PORT();
void initialize_INTERRUPT();
void delay();

int  main(void)
{
     unsigned char ii;	
 
     initialize_PORT();
     initialize_INTERRUPT();
 
     sei();      
	
     while(1) {
        for(ii=0x01; ii<=0x05; ii++){
            PORTA=ii;
            delay();
        }
     }
}

void initialize_PORT()
{
     DDRA  = 0xFF;
     PORTA = 0x00;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER; 
     EIMSK = EXT_INT0_ENABLE ;
//   EIFR  = 0x00;           // 필요없다? clear ext.int flag
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

     for(jj=0x06; jj<=0x0A; jj++){
         PORTA = jj;
         delay();
     }
}


