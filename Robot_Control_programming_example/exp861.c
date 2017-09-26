// *** exp861.c ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char duty_Ratio, direction;

//Timer0
#define PWM_MODE                    0x00 
#define FAST_PWM_MODE               0x48  
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 
#define PRESCALE_8                  0x02  

//Motor Control
#define CCW_ROTATION  0x80 
#define CW_ROTATION   0x40 
#define FREE_STOP     0x00 
#define DUTYRATIO_MAX       0xFF
#define DUTYRATIO_50        0x80
#define DUTYRATIO_MIN       0x00

//Analog Comparator
#define AC_ENABLE                0x00  // 0000 0000
#define AC_INTERRUPT_ENABLE      0x08  // 0000 1000
#define INTERRUPT_MODE_ON_TOGGLE 0x00  // 0000 0000
#define AC_OUTPUT                0x20  // 0010 0000

//Main Pulse Out
#define PF_7_HIGH 0x80
#define PF_7_LOW  0x00

void initialize_PORT();
void initialize_TIMER0_PWM();
void initialize_DC_motor_rotation();
void initialize_AC_Interrupt();
void rotate_DC_motor();

void delay_us(unsigned char time_us);
void delay_ms(unsigned int  time_ms);

int  main( )
{
     initialize_PORT();	 
     initialize_TIMER0_PWM();
     initialize_DC_motor_rotation();
     initialize_AC_Interrupt();

     sei();

     while(1){
        PORTF=PF_7_HIGH;  // 주기-약 31 ms, 잘 맞지 않는다
        delay_ms(14);     // compile시 optimization - Os로
        PORTF=PF_7_LOW;
        delay_ms(14);
    }
}
	
void initialize_PORT()
{
     DDRA=0xFF;    // LED
     DDRB=0xFF;    // DC motor control, OC0 output
     DDRF=0x80;    // PF.7=>output
}

void initialize_TIMER0_PWM()
{
     TCCR0 = PWM_MODE|FAST_PWM_MODE|OUTPUT_LOW_ON_COMPARE_MATCH|PRESCALE_8; 
     TCNT0 = 0x00;
}

void initialize_DC_motor_rotation()
{
     PORTB = FREE_STOP;
     OCR0  = DUTYRATIO_MIN;
     PORTA = OCR0;
}

void rotate_DC_motor()
{
     PORTB = direction;
     OCR0  = duty_Ratio;
     PORTA = duty_Ratio;
}

void initialize_AC_Interrupt()
{
     ACSR = AC_ENABLE | AC_INTERRUPT_ENABLE | INTERRUPT_MODE_ON_TOGGLE;
}

ISR(ANALOG_COMP_vect)
{
     if     ((ACSR & AC_OUTPUT)!=0x00){
              duty_Ratio=DUTYRATIO_MAX;
              direction=CCW_ROTATION;
              rotate_DC_motor();
     }
     else if((ACSR & AC_OUTPUT)==0x00){
              duty_Ratio=DUTYRATIO_50;
              direction=CCW_ROTATION;
              rotate_DC_motor();
     }
}

void delay_us(unsigned char time_us)	
{
     register unsigned char i;

     for(i=0; i<time_us; i++){	        
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
     }  
}

void delay_ms(unsigned int  time_ms)	
{
     register unsigned int i;

     for(i=0; i<time_ms; i++){ 
         delay_us(250);
         delay_us(250);
         delay_us(250);
         delay_us(250);
     }
}


