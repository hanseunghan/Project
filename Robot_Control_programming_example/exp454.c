// *** exp454.c ***

#include <avr/io.h>

#define UP_DATA   0xA0
#define DOWN_DATA 0x60
#define STOP_DATA 0x20

void initialize_PORT();
void move_Elevator(unsigned char move_data);
void delay();

int  main(void)
{
     initialize_PORT();

     if(PINE==0) move_Elevator(DOWN_DATA);
     delay();
     while(PINE==0) move_Elevator(UP_DATA);
     move_Elevator(STOP_DATA);
}

void initialize_PORT()
{
     DDRB=0xFF; 
     DDRA=0xFF; 
     DDRE=0x00;
}

void move_Elevator(unsigned char move_data)
{
     PORTB=PORTA=move_data;
}

void delay()
{
     unsigned int ta,tb,i,j;

     ta=20;
     tb=50000; 
     for (i=0; i<ta; i++) {
     for (j=0; j<tb; j++)
          if (PINE!=0x00) return; 
     }
}
