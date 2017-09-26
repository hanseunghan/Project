//*** exp401.c ***

#include <avr/io.h>

#define SW0          0x01
#define CCW_ROTATION 0xA0
#define CW_ROTATION  0x60
#define QUICK_STOP   0x20
#define FREE_STOP    0x00

void initialize_PORT();
void rotate_DC_Motor(unsigned char rotation_data);
void delay(unsigned char ta);

int  main(void)
{
     initialize_PORT();
     rotate_DC_Motor(QUICK_STOP);

     while(1){
	  while((PIND & SW0)!=0x00){
		rotate_DC_Motor(CCW_ROTATION);
		delay(20);
		rotate_DC_Motor(QUICK_STOP);
		delay(10);
		rotate_DC_Motor(CW_ROTATION);
		delay(20);
		rotate_DC_Motor(QUICK_STOP);
		delay(10);
	  }
       rotate_DC_Motor(FREE_STOP);
     }
}

void initialize_PORT()
{
    DDRB=0xFF;  // control signal output => dc motor controller 연결
    DDRA=0xFF;  // motor control 신호 확인
    DDRD=0x00;
}

void rotate_DC_Motor(unsigned char rotation_data)
{
     PORTB=PORTA=rotation_data;
}

void delay(unsigned char ta)
{
     unsigned int tb,i,j;  // volatile unsigned int tb,i,j;

     tb=35000;
		
     for(i=0; i<ta; i++){
        for(j=0; j<tb; j++){
            if((PIND & SW0)==0x00){
                rotate_DC_Motor(FREE_STOP);
                return;
            }
        }
     }
}