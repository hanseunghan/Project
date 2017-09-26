//*** exp404.c ***

#include <avr/io.h>

unsigned char THMAX, THMIN, TH, movement;

#define SW0  0x01
#define SW1  0x02

#define CCW_ROTATION 0xA0
#define CW_ROTATION  0x60
#define QUICK_STOP   0x20
#define FREE_STOP    0x00

#define INCREASE     1
#define DECREASE     0

void initialize_PORT();
void initialize_period_data();
void rotate_DC_Motor(unsigned char rotation_data);
void dc_motor_continous();
void delay(unsigned char ta);

int  main(void)
{
     initialize_PORT();
     initialize_period_data();
     rotate_DC_Motor(QUICK_STOP);
		
     while(1){
        if((PIND & SW0)!=0x00){
            while((PIND & SW1)==0x00){
                   dc_motor_continous();
                   switch(movement){
                          case INCREASE:TH++;
                                        if(TH==THMAX) movement=DECREASE;
                                        break;
                          case DECREASE:TH--;
                                        if(TH==THMIN) movement=INCREASE;
                   }
            }
        }
        else if((PIND & SW1)!=0x00) break;
     }
     rotate_DC_Motor(QUICK_STOP);
}

void initialize_PORT()
{
     DDRB=0xFF;
     DDRA=0xFF;
     DDRD=0x00;
}

void initialize_period_data()
{
     THMAX=100;
     THMIN=0;
     TH=THMIN;
     movement=INCREASE;
}

void rotate_DC_Motor(unsigned char rotation_data)
{
     PORTB=PORTA=rotation_data;
}

void dc_motor_continous()
{
     rotate_DC_Motor(CCW_ROTATION);
     delay(TH);
     rotate_DC_Motor(QUICK_STOP);
     delay(THMAX-TH);
}

void delay(unsigned char ta)
{
     unsigned int tb,i,j; // volatile unsigned int tb,i,j;

     tb=100;
		
     for(i=0; i<ta; i++){
         for(j=0; j<tb; j++){
             if((PIND & SW1)!=0x00) return;
         }
     }
}
