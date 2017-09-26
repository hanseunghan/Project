//*** exp384C.c -복합문 수정***

#include <avr/io.h>

#define LED0  0x01

void short_delay();
void delay();

int main(void)
{
    unsigned char now_position, aim_position;

    DDRA=0xFF;
    DDRD=0x00;

    now_position=LED0;
    PORTA=now_position;
	
    while(1){
       do{
          aim_position=PIND;
          short_delay();         // very short delay 추가
        } while(aim_position==0x00);
        
       while(1){
          if     (aim_position==now_position) break;
          else if(aim_position>now_position)  now_position=now_position<<1;
          else if(aim_position<now_position)  now_position=now_position>>1;
          PORTA=now_position;
          delay();
       }
    }
}

void short_delay()
{
     unsigned int tk;

     tk=8000; while(tk--);     
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