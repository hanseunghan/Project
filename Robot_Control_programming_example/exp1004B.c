//*** exp1004B.c --- CTC Mode와 interrupt 사용; 
//               --- isr 수행으로 인한 delay 누적오차 발생
//               --- pulse발생기능만으로는 비효율, isr에서 타동작과 같이 사용***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int  ocr1_index,ocr3_index;
const unsigned int ocr1_table[]={5999,13999};         // 6000,14000
const unsigned int ocr3_table[]={7999, 6999, 12999};  // 8000, 7000,13000

#define PF_7_HIGH	 0x80
#define PF_7_LOW	 0x00	

#define CTC_MODE_HIGH  	                   0x08  // Mode-4	
#define CTC_MODE_LOW   	                   0x00
#define OUTPUT_TOGGLE_ON_COMPARE_MATCH	   0x40
#define PRESCALE_8	                           0x02   
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE 0x10
#define TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE 0x10
 
void initialize_PORT();
void initialize_TIMER13_PULSE_GENERATION();
void initialize_INTERRUPT();
void delay();

int  main(void )
{  
     initialize_PORT();
     initialize_TIMER13_PULSE_GENERATION();
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
     DDRB = DDRE = 0xFF;    // 생략안됨 - PB.5(OC1A)와 PE.3(OC3A) 출력
     DDRF = 0x80;
     PORTF = 0x00;
}

void initialize_TIMER13_PULSE_GENERATION()
{
     TCCR1A = CTC_MODE_LOW  | OUTPUT_TOGGLE_ON_COMPARE_MATCH ;
     TCCR1B = CTC_MODE_HIGH | PRESCALE_8 ;
     TCCR3A = CTC_MODE_LOW  | OUTPUT_TOGGLE_ON_COMPARE_MATCH ;
     TCCR3B = CTC_MODE_HIGH | PRESCALE_8 ;

     ocr1_index=0; OCR1A=ocr1_table[ocr1_index]; 
     ocr3_index=0; OCR3A=ocr3_table[ocr3_index]; 
}

void initialize_INTERRUPT()
{
     TIMSK  = TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;
     ETIMSK = TIMER3_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;
}

ISR(TIMER1_COMPA_vect)
{  
     ocr1_index++; 
     if(ocr1_index==2) ocr1_index=0; //0->1->0->1->...
     OCR1A=ocr1_table[ocr1_index];
}


ISR(TIMER3_COMPA_vect)
{  
     ocr3_index++; 
     if(ocr3_index==3) ocr3_index=1; //0->1->2->1->2->...
     OCR3A=ocr3_table[ocr3_index];
}

void delay()
{
     unsigned int td=11500;

     while(td--);
}
