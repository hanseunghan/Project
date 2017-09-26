//*** exp801.c ***

#include <avr/io.h>
#include <avr/interrupt.h>    

#define INT0_FALLING_EDGE_TRIGGER  0x02 // 0000 0010
#define INT0_RISING_EDGE_TRIGGER   0x03 // 0000 0011 
#define EXT_INT0_ENABLE            0x01 // 0000 0001

void initialize_PORT();
void initialize_INTERRUPT();
void delay();

int  main(void)
{
     initialize_PORT();
     initialize_INTERRUPT();

     sei();    // <= #include <avr/interrupt.h> ; Set Global Interrupt Enable 

     while(1){
         PORTA = 0x33;
         delay();
         PORTA = 0xCC;
         delay();
     }
}

void initialize_PORT()
{
     DDRA  = 0xFF;
     PORTA = 0x00;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER; // External Interrupt Control Register A 
     EIMSK = EXT_INT0_ENABLE ;         // External Interrupt Mask Register
}

void delay()
{
//   unsigned int ta,tb,i,j;          //optimization O0로 해야만 동작가능.
     volatile unsigned int ta,tb,i,j; //optimization Os로 해도   동작가능.

     ta=15; tb=50000;
     for(i=0; i<ta; i++)
         for(j=0; j<tb; j++);
}

ISR(INT0_vect)          // <= iom128.h <= io.h

{
     PORTA=0x55;
     delay();
}
