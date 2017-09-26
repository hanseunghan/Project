//*** exp384A.c ***

#include <avr/io.h>

#define LED0  0x01

void delay();

int main(void)
{
    unsigned char now_position,aim_position;

    DDRA=0xFF;
    DDRD=0x00;

    now_position=LED0;
    PORTA=now_position;
	
    while(1){
       while(PIND==0x00);
       aim_position=PIND;	// switch charttering 또는 timing으로 인한 오동작 발생!
       while(1){
          if     (aim_position==now_position) break;
          else if(aim_position>now_position)  now_position=now_position<<1;
          else	              		      now_position=now_position>>1;
          PORTA=now_position;
          delay();
       }
    }
}

void delay()
{
     unsigned int ta,tb,i,j;

     ta=10;
     tb=35000;
	
     for(i=0; i<ta; i++){
         for(j=0; j<tb; j++);
     }
}
	