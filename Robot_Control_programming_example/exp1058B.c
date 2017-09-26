// *** exp1058B.c --- STEP motor control ***
// ***            --- Timer1 CTC mode 와 external int0(SW0),INT1(SW1),INT2(SW2) 이용  ***
// ***            --- switch가 OFF되어도 timer동작과 int는 반복하나, pulse만 발생안함 ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char pulse_generation;

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
#define EXT_INT0_ENABLE   0x01
#define EXT_INT1_ENABLE   0x02
#define EXT_INT2_ENABLE   0x04

#define YES   1
#define NO    0

//Timer1
#define CTC_MODE_HIGH   0x08
#define CTC_MODE_LOW    0x00
#define PRESCALE_8      0x02
#define OUTPUT_NOCHANGE_ON_COMPARE_MATCH  0x00
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH    0x40
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10

void initialize_PORT();
void initialize_STEP_MOTOR();
void initialize_TIMER1_PULSE_GENERATION();
void initialize_INTERRUPT();
void generate_START_PULSE();
void delay();

int  main(void)
{  
     initialize_PORT();
     initialize_STEP_MOTOR();    
     initialize_TIMER1_PULSE_GENERATION();
     initialize_INTERRUPT();
   
     sei();                  // int를 사용하므로 main 동작에 제약이 없음!!
   
     while(1){               // motor를 구동하기 위한 switch check 과정이 없음!
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
     DDRA=0xFF;  // step motor control signal 확인
     DDRD=0x00;  // push button switch input
     DDRB=0xFF;  // PB5(OC1A) output
 
     PORTC=PORTA=0x00;
}

void initialize_STEP_MOTOR()
{
     PORTC = PORTA = ROTATION_DISABLE; // 처음에 정지상태에서 대기함
}

void initialize_TIMER1_PULSE_GENERATION()
{
     TCCR1A = CTC_MODE_LOW  | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
     TCCR1B = CTC_MODE_HIGH | PRESCALE_8;


     OCR1A = 5000-1;         // timer1 반복주기=2.5ms(pulse주기=5ms),duty ratio=50% 
     pulse_generation = NO;  // int는 반복하나 pulse만 발생하지 않음.
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER| INT1_RISING_EDGE_TRIGGER| INT2_RISING_EDGE_TRIGGER;  
     EIMSK = EXT_INT0_ENABLE | EXT_INT1_ENABLE | EXT_INT2_ENABLE;
     TIMSK = TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE; 
// 처음에 int enable만 해놓는다. 이후에 switch를 누르면 pulse 발생하도록 한다.
}

ISR(INT0_vect)
{
     PORTC = PORTA = CCW_ROTATION|ROTATION_ENABLE;
     generate_START_PULSE();     // ext int switch ON해야 pulse발생
}

void generate_START_PULSE()
{
     pulse_generation = YES;
     TCCR1A = CTC_MODE_LOW | OUTPUT_TOGGLE_ON_COMPARE_MATCH;
}

ISR(INT1_vect)
{
     PORTC = PORTA = CW_ROTATION|ROTATION_ENABLE;
     generate_START_PULSE(); 
}

ISR(INT2_vect)
{
     PORTC = PORTA = ROTATION_DISABLE;
     TCCR1A = CTC_MODE_LOW | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
     cli();
}

ISR(TIMER1_COMPA_vect)
{
     if(pulse_generation == NO) return;
                  // 처음부터 int반복하며,pulse의 반주기마다 switch OFF 상태만을 체크한다.
                  // switch OFF이후부터 int는 반복하나 pulse발생안함. 
     if((PIND & SW0)== 0x00 && (PIND & SW1)== 0x00) { 
         pulse_generation = NO;
         TCCR1A = CTC_MODE_LOW  | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
         PORTC = PORTA = ROTATION_DISABLE;
     }
}

void delay()
{
     unsigned int td=11500;
     while(td--);
}