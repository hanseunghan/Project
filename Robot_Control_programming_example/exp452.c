// *** exp452.c ***

#include <avr/io.h>

#define UP_DATA   0xA0
#define DOWN_DATA 0x60
#define STOP_DATA 0x20

void initialize_PORT();
void move_Elevator(unsigned char move_data);

int  main(void)
{
     initialize_PORT();
     while(PINE==0) move_Elevator(DOWN_DATA);
     move_Elevator(STOP_DATA);
}

void initialize_PORT()
{
     DDRB=0xFF;  // DC motor control
     DDRA=0xFF;  // control signal output »Æ¿Œ
     DDRE=0x00;  // limit switch
}

void move_Elevator(unsigned char move_data)
{
     PORTB=PORTA=move_data;
}