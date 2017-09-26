// *** exp1061A.c --- STEP motor control ***
// ***            --- External int & Timer3 output compare match int 이용 ***
// ***            --- switch가 OFF되어도 int는 반복하나 pulse는 발생안함  ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int  int_count;
unsigned char pulse_generation;

#define PF_7_HIGH	 0x80
#define PF_7_LOW	 0x00	

//step motor
#define CCW_ROTATION     0x40
#define CW_ROTATION      0x00
#define ROTATION_ENABLE  0x20
#define ROTATION_DISABLE 0x00

#define PULSE_NO 200     
#define YES      1
#define NO       0

//external interupt
#define INT0_RISING_EDGE_TRIGGER   0x03  
#define INT1_RISING_EDGE_TRIGGER   0x0C
#define EXT_INT0_ENABLE	   0x01
#define EXT_INT1_ENABLE	   0x02

//Timer3
#define CTC_MODE_HIGH	 0x08
#define CTC_MODE_LOW	 0x00
#define PRESCALE_8         0x02
#define OUTPUT_NOCHANGE_ON_COMPARE_MATCH  0x00
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH    0x40
#define TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10

void initialize_PORT();
void initialize_STEP_MOTOR();
void initialize_TIMER3_PULSE_GENERATION();
void initialize_INTERRUPT();
void generate_START_PULSE();
void delay();

int  main(void)
{  
     initialize_PORT();
     initialize_STEP_MOTOR();    
     initialize_TIMER3_PULSE_GENERATION();
     initialize_INTERRUPT();
   
     sei();
   
     while(1){
        PORTF = PF_7_HIGH;
        delay();
        PORTF = PF_7_LOW;
        delay();
     }        
}

void initialize_PORT()
{
     DDRC=0xFF;  // step motor control output
     DDRA=0xFF;  // step motor control signal 확인
//   DDRD=0x00;  // switch input(INT0,INT1)
     DDRB=0xFF;  // pulse count 출력
     DDRE=0xFF;  // PE3(OC3A) output

     PORTC=PORTA=0x00;
}

void initialize_STEP_MOTOR()
{
     PORTC = PORTA = ROTATION_DISABLE;
}


void initialize_TIMER3_PULSE_GENERATION()
{
     TCCR3A = CTC_MODE_LOW  | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
     TCCR3B = CTC_MODE_HIGH | PRESCALE_8;

     OCR3A = 5000-1;
               // timer3 toggle 주기=2.5ms,duty ratio=50% (<- CTC mode에서 toggle 반복)
//   TCNT3 = 0x0000;
     pulse_generation = NO;
}

void initialize_INTERRUPT()
{
     EICRA = INT0_RISING_EDGE_TRIGGER | INT1_RISING_EDGE_TRIGGER;   
     EIMSK = EXT_INT0_ENABLE | EXT_INT1_ENABLE;
     ETIMSK = TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;
}

ISR(INT0_vect)
{
     PORTC = PORTA = CCW_ROTATION|ROTATION_ENABLE;
     generate_START_PULSE();
}

void generate_START_PULSE()
{
     int_count = 0;
     pulse_generation = YES;
     TCCR3A = CTC_MODE_LOW | OUTPUT_TOGGLE_ON_COMPARE_MATCH; 
}

ISR(INT1_vect)
{
     PORTC = PORTA = CW_ROTATION |ROTATION_ENABLE;
     generate_START_PULSE();
}

ISR(TIMER3_COMPA_vect)
{
     if(pulse_generation == NO) return;//int는 반복하나,ext int 입력후부터 count시작함.

     int_count++;
     if(int_count == 2*PULSE_NO) {    // 400번->200pulse
        TCCR3A = CTC_MODE_LOW | OUTPUT_NOCHANGE_ON_COMPARE_MATCH;
        pulse_generation = NO;
        PORTC = PORTA = ROTATION_DISABLE;
     }
}

void delay()
{
     unsigned int td=11500;
     while(td--);
}
