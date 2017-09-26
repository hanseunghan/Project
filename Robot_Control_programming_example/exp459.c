// *** exp459.c ***

#include <avr/io.h>

unsigned char initial_position, now_position;

#define UP_DATA     0xA0
#define DOWN_DATA   0x60
#define STOP_DATA   0x20
#define SW2         0x04
#define SW1         0x02

#define START_FLOOR   0x01  // 0類
#define UPPER_FLOOR   0x80  // 7類
#define LOWER_FLOOR   0x01  // 0類

void initialize_PORT();
void move_Initial_Floor();
void move_Elevator(unsigned char move_data);
void delay();
void display_Floor();
void move_up();
void move_down();

int  main(void)
{
     initialize_PORT();
     move_Initial_Floor();

     while(1){
        if     ((PIND & SW2)!=0x00) move_up();
        else if((PIND & SW1)!=0x00) move_down();
        move_Elevator(STOP_DATA);
     }
}

void initialize_PORT()
{
     DDRB=0xFF;  // DC motor control signal
     DDRA=0xFF;  // DC motor control signal �挫�
     DDRC=0xFF;  // 7-seg. LED display  
     DDRD=0x00;  // push button switch
     DDRE=0x00;  // limit switch
}


void move_Initial_Floor()
{
     initial_position=START_FLOOR; 

     if(PINE==0x00) move_Elevator(DOWN_DATA);
     delay();
     while(PINE==0x00) move_Elevator(UP_DATA);
     move_Elevator(STOP_DATA);

     while(1) {
        while((now_position=PINE)==0x00);
        display_Floor();
        if      (now_position>initial_position) move_Elevator(DOWN_DATA);
        else if (now_position<initial_position) move_Elevator(UP_DATA);
        else    break;
//      while(PINE!=0x00);
     }
     move_Elevator(STOP_DATA);
}

void move_Elevator(unsigned char move_data)
{
     PORTB=PORTA=move_data;;
}

void delay()
{
     unsigned int ta,tb,i,j;

     ta=20;
     tb=50000; 
     for (i=0; i<ta; i++) {
     for (j=0; j<tb; j++) if(PINE!=0x00) return; 
     }
}

void display_Floor()
{
     unsigned char temp, count_floor; 

     temp=now_position;
     count_floor=0;
     while(1){
         temp=temp>>1;
         if (temp==0) break;
         else         count_floor++;
     }
     PORTC=count_floor; 
}      

void move_up()
{
//   if(now_position==UPPER_FLOOR) return; //7類縑 氈朝等 SW2蒂 傳 援腦朝 唳辦
     while(now_position != UPPER_FLOOR){
           move_Elevator(UP_DATA);
           while(PINE!=0);
           while((now_position=PINE)==0);
           display_Floor(); 
     }
}

void move_down()
{
//   if(now_position==LOWER_FLOOR) return; //0類縑 氈朝等 SW1擊 傳 援腦朝 唳辦
     while(now_position != LOWER_FLOOR){
           move_Elevator(DOWN_DATA);
           while(PINE!=0);
           while((now_position=PINE)==0);
           display_Floor(); 
     }    
}
