// *** exp1058C.c --- STEP motor control ***
// ***            --- by Timer1 CTC mode �� external int0(SW0),INT1(SW1),INT2(SW2) �̿�  ***
// ***            --- switch�� OFF�Ǹ� timer���۰� int�� �ݺ����� ������, pulse�� �߻�����.
// ***            --- program������ ������� �ʰ� register���������� ���۰���=>���100%Ȱ��!!

#include <avr/io.h>
#include <avr/interrupt.h>

#define SW0   0x01
#define SW1   0x02
#define SW2   0x04

#define PF_7_HIGH  0x80
#define PF_7_LOW   0x00	

//step motor
#define CCW_ROTATION     0x40
#define CW_ROTATION      0x00
#define ROTATION_ENABLE  0x20
#define ROTATION_DISABLE 0x00

//external interupt
#define INT0_RISING_EDGE_TRIGGER   0x03  
#define INT1_RISING_EDGE_TRIGGER   0x0C
#define INT2_RISING_EDGE_TRIGGER   0x30
#define EXT_INT0_ENABLE	   0x01
#define EXT_INT1_ENABLE	   0x02
#define EXT_INT2_ENABLE	   0x04

#define YES   1
#define NO    0

//Timer1
#define CTC_MODE_HIGH	 0x08
#define CTC_MODE_LOW	 0x00
#define PRESCALE_8       0x02
#define OUTPUT_NOCHANGE_ON_COMPARE_MATCH  0x00
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH    0x40
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10
#define CLEAR            0x00

void initialize_PORT();
void initialize_STEP_MOTOR();
void initialize_INTERRUPT();

void set_TIMER1_PULSE_GENERATION();
void clear_TIMER1_PULSE_GENERATION();
void delay();

int  main(void)
{  
     initialize_PORT();
     initialize_STEP_MOTOR();    
     initialize_INTERRUPT();
    
     sei();                  // main ������ �����ο�!!
    
     while(1){               // switch�� ������ ������, �ƿ� int�� pulse�� �߻���Ű�� ����!
        PORTF = PF_7_HIGH;
        delay();
        PORTF = PF_7_LOW;
        delay();
        if ((PIND & SW2)!= 0x00) break;
     } 
}   

void initialize_PORT()
{
     DDRC=0xFF;  // step motor control output
     DDRA=0xFF;  // step motor control signal Ȯ��
     DDRD=0x00;  // push button switch input
     DDRB=0xFF;  // PB5(OC1A) output
 
     PORTC=PORTA=0x00;
}

void initialize_STEP_MOTOR()
{
     PORTC = PORTA = ROTATION_DISABLE;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER | INT1_RISING_EDGE_TRIGGER | INT2_RISING_EDGE_TRIGGER;
     EIMSK = EXT_INT0_ENABLE | EXT_INT1_ENABLE | EXT_INT2_ENABLE;
}

ISR(INT0_vect)
{
     PORTC = PORTA = CCW_ROTATION|ROTATION_ENABLE;
     set_TIMER1_PULSE_GENERATION();  // ext int0 switch ON�ؾ� pulse�߻�
}

ISR(INT1_vect)
{
     PORTC = PORTA = CW_ROTATION|ROTATION_ENABLE;
     set_TIMER1_PULSE_GENERATION();  // ext int1 switch ON�ؾ� pulse�߻�
}

ISR(INT2_vect)
{
     PORTC = PORTA = ROTATION_DISABLE;
     clear_TIMER1_PULSE_GENERATION();
     cli();                          // global interrupt disable
}

void set_TIMER1_PULSE_GENERATION()
{
     TCCR1A = CTC_MODE_LOW  | OUTPUT_TOGGLE_ON_COMPARE_MATCH;//switch ON���Ŀ��� register����
     TCCR1B = CTC_MODE_HIGH | PRESCALE_8;
     TIMSK  = TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;      //timer int enable�� ���⿡��!
 
     TCNT1 = 0x0000;  // �ʱ�ȭ ����! �ٸ� ���ۿ� ������ �ִ��� üũ
     OCR1A = 5000-1;  // timer1 toggle �ݺ��ֱ�=2.5ms,duty ratio=50% 
}

void clear_TIMER1_PULSE_GENERATION()
{
     TCCR1A = CLEAR;    // switch OFF���Ŀ��� register ���� => pulse �߻�����.
     TCCR1B = CLEAR;
     TIMSK  = CLEAR;    // timer int�� �ݺ����� => �ٸ� ���ۿ� ����!!
}

ISR(TIMER1_COMPA_vect)
{
//   switch ON  ���ĺ��͸� int �ݺ��ϸ�, pulse�� ���ֱ⸶�� switch OFF���¸��� üũ�Ѵ�
//   switch OFF ���ĺ��ʹ� pulse�߻����� �ʰ�, int�� �ݺ����� ����.

     if ((PIND & SW0)== 0x00 && (PIND & SW1)== 0x00) { 
          clear_TIMER1_PULSE_GENERATION();
          PORTC = PORTA = ROTATION_DISABLE;
     }
}

void delay()
{
     unsigned int td=11500;

     while(td--);
}