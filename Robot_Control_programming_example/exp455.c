// *** exp455.c --- elevator control ***

#include <avr/io.h>

#define UP_DATA   0xA0
#define DOWN_DATA 0x60
#define STOP_DATA 0x20

void initialize_PORT();
void move_Elevator(unsigned char move_data);

int  main(void)
{
     unsigned char aim_position, now_position;

     initialize_PORT();
     move_Elevator(STOP_DATA);
     now_position=PINE;  // ������ġ �ľ� 

     while(1){
        while((aim_position=PIND)==0x00);  
        while (aim_position!=now_position) {
           if     (aim_position>now_position) move_Elevator(UP_DATA);
           else if(aim_position<now_position) move_Elevator(DOWN_DATA);
           while(PINE!=0x00);                //�������� L/S���� ������ ������
           while((now_position=PINE)==0x00); //���������̸� �̵�,�������� ������ ������ 
        }   
        move_Elevator(STOP_DATA);
     }
}

void initialize_PORT()
{
     DDRB=0xFF; // DC motor control signal
     DDRA=0xFF; // DC motor control signal Ȯ��
     DDRD=0x00; // push button switch
     DDRE=0x00; // limit switch
}

void move_Elevator(unsigned char move_data)
{
     PORTB=PORTA=move_data;
}