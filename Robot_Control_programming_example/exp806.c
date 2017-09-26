//*** exp806.c ***

#include <avr/io.h>
#include <avr/interrupt.h>       

#define INT0_RISING_EDGE_TRIGGER   0x03
#define INT1_RISING_EDGE_TRIGGER   0x0C // 0000 1100 
#define EXT_INT0_ENABLE	           0x01 
#define EXT_INT1_ENABLE	           0x02 // 0000 0010 

void initialize_PORT();
void initialize_INTERRUPT();
void delay();

int  main(void)
{
     unsigned char ii;	

     initialize_PORT();
     initialize_INTERRUPT();

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
     DDRA  = 0xFF;
     PORTA = 0x00;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER|INT1_RISING_EDGE_TRIGGER; 
     EIMSK = EXT_INT0_ENABLE|EXT_INT1_ENABLE;
}

void delay()
{
     volatile unsigned int ta,tb,i,j;

     ta=10;  tb=50000;
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

ISR(INT1_vect)
{
     unsigned char kk;

     for(kk=0x0B; kk<=0x0F; kk++){
         PORTA = kk;
         delay();
     }
}
