// *** exp451.c --- elevator up/down check!***

#include <avr/io.h>

#define SW0 0x01
#define SW1 0x02

#define UP_DATA   0xA0
#define DOWN_DATA 0x60
#define STOP_DATA 0x20

void initialize_PORT();
void move_Elevator(unsigned char move_data);

int  main(void)
{
     initialize_PORT();
     move_Elevator(STOP_DATA);

     while(1){
        while((PIND & SW1)!=0) move_Elevator(UP_DATA);
        while((PIND & SW0)!=0) move_Elevator(DOWN_DATA);
        move_Elevator(STOP_DATA);
     }
}

void initialize_PORT()
{
     DDRB=0xFF;  // control signal output => dc motor controller
     DDRA=0xFF;  // control signal output »Æ¿Œ
     DDRD=0x00;  // push button switch input
}

void move_Elevator(unsigned char move_data)
{
     PORTB=PORTA=move_data;
}

