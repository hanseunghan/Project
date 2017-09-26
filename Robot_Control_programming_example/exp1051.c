// *** exp1051.c --- STEP motor control ***

#include <avr/io.h>

unsigned char rotation_data;

#define SW0              0x01
#define SW1              0x02
#define SW2              0x04

#define PC_7_HIGH        0x80
#define PC_7_LOW         0x00
#define CCW_ROTATION     0x40
#define CW_ROTATION      0x00
#define ROTATION_ENABLE  0x20
#define ROTATION_DISABLE 0x00

void initialize_PORT();
void generate_One_Pulse();
void delay();

int  main(void)
{	 
     unsigned char  inswitch;

     initialize_PORT();
     PORTC=PORTA= ROTATION_DISABLE;

     while(1) {
        while((inswitch=PIND)==0x00);  
        switch (inswitch) {
           case SW0:rotation_data=CCW_ROTATION | ROTATION_ENABLE;
                    generate_One_Pulse();				
                    break; 
           case SW1:rotation_data=CW_ROTATION | ROTATION_ENABLE;
                    generate_One_Pulse();
                    break;
           case SW2:break;
        }
        if((PIND & SW2)!=0) break; 
     }
}

void initialize_PORT()
{
     DDRC=0xFF;  // step motor control output
     DDRA=0xFF;  // step motor control signal 확인
     DDRD=0x00;  // switch input
}

void generate_One_Pulse(void)
{
     PORTC=PORTA= PC_7_HIGH | rotation_data;
     delay();
     PORTC=PORTA= PC_7_LOW  | rotation_data;
     delay();
}

void delay(void)
{
     unsigned int td=2100;  // 대략

     while(td--);
}


