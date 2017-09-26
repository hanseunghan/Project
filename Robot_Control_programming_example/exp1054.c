// *** exp1054.c --- STEP motor control ***

#include <avr/io.h>

unsigned char  rotation_data;

#define SW0              0x01
#define SW1              0x02

#define PC_7_HIGH        0x80
#define PC_7_LOW         0x00
#define CCW_ROTATION     0x40
#define CW_ROTATION      0x00
#define ROTATION_ENABLE  0x20
#define ROTATION_DISABLE 0x00
#define PULSE_NO	 200

void initialize_PORT();
void generate_One_Pulse();
void delay();

int  main(void)
{	 
     unsigned char inswitch,i;
 
     initialize_PORT();
     PORTC=PORTA= ROTATION_DISABLE; 

     while(1){    	  	
        while((inswitch=PIND)==0x00);
        switch(inswitch){
               case SW0:rotation_data=CCW_ROTATION|ROTATION_ENABLE;
                        for(i=1;i<=PULSE_NO;i++) generate_One_Pulse();
                        break; 
               case SW1:rotation_data=CW_ROTATION|ROTATION_ENABLE;
                        for(i=1;i<=PULSE_NO;i++) generate_One_Pulse();
        }    
     }
}

void initialize_PORT()
{
     DDRC=0xFF;  // step motor control output
     DDRA=0xFF;  // step motor control signal È®ÀÎ
     DDRD=0x00;  // switch input
}

void generate_One_Pulse()
{
     PORTA=PORTC= PC_7_HIGH|rotation_data;
     delay();
     PORTA=PORTC= PC_7_LOW |rotation_data;
     delay();
}

void delay()
{
     unsigned int td=2900;

     while(td--);
}
