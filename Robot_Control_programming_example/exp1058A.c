// *** exp1058A.c --- STEP motor control ***
// ***            --- CTC mode�� ����ص�, �Ϲ� switch�� ����ϸ� ��ȿ������! ***

#include <avr/io.h>
#include <avr/interrupt.h>

#define SW0   0x01
#define SW1   0x02
#define SW2   0x04

#define PF_7_HIGH	 0x80
#define PF_7_LOW	 0x00	

//step motor
#define CCW_ROTATION     0x40
#define CW_ROTATION      0x00
#define ROTATION_ENABLE  0x20
#define ROTATION_DISABLE 0x00

//Timer1
#define CTC_MODE_HIGH	 0x08  // Mode-4 (=> exp1002 ����)
#define CTC_MODE_LOW	 0x00
#define PRESCALE_8       0x02  // 2MHz(0.5��s)
#define OUTPUT_NOCHANGE_ON_COMPARE_MATCH  0x00 // 0000 0000
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH    0x40 // 0100 0000

void initialize_PORT();
void initialize_TIMER3_PULSE_GENERATION();

int  main(void)
{
     unsigned char inswitch;

     initialize_PORT();
     initialize_TIMER1_PULSE_GENERATION();

     while(1){
        while((inswitch=PIND)==0x00); // switch check������ �����Ƿ� ��ȿ������!!
        if(inswitch==SW0 || inswitch==SW1) {      
             switch(inswitch){
                case SW0: PORTC=PORTA=CCW_ROTATION|ROTATION_ENABLE;
                          break; 
                case SW1: PORTC=PORTA=CW_ROTATION|ROTATION_ENABLE;
            }
// switch�� ������ �ִ� ���ȿ��� pulse �߻�
            while(PIND != 0x00)  TCCR1A = CTC_MODE_LOW | OUTPUT_TOGGLE_ON_COMPARE_MATCH;
// switch�� ����, pulse �߻�����
            TCCR1A = CTC_MODE_LOW | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;  
        }
        else if(PIND==0x00){
                PORTC=PORTA=ROTATION_DISABLE; 
//              while(PIND == 0x00);               
        }       
        else if((PIND & SW2)!=0x00){
                PORTC=PORTA=ROTATION_DISABLE; 
                cli();
                break;
        }
     }
}

void initialize_PORT()
{
     DDRC=0xFF;  // step motor control output
     DDRA=0xFF;  // step motor control signal Ȯ��
     DDRD=0x00;  // switch input
     DDRB=0xFF;  // PB5(OC1A) output

     PORTC=PORTA=0x00;
}

void initialize_TIMER1_PULSE_GENERATION()
{
     TCCR1A = CTC_MODE_LOW  | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
     TCCR1B = CTC_MODE_HIGH | PRESCALE_8;

     OCR1A = 5000-1;  // timer1 toggle �ݺ��ֱ�=2.5ms(pulse�ֱ�=5ms),duty ratio=50% 
}