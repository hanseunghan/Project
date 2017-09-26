//*** exp601C.c  =>  exp601A에서 single mode를 free running mode로  *** 

#include <avr/io.h>

#define AREF_SELECT               0x00  
#define ADC_LEFT_ADJUSTED_RESULT  0x20  
#define ADC0                      0  

#define ADC_ENABLE	       0x80  
#define ADC_START_CONVERSION   0x40  
#define FREE_RUNNING_MODE      0x20  // ADC반복을 자동으로!
#define PRESCALE_128	       0x07  
#define ADC_COMPLETE 	       0x10  
#define RESET_ADC_COMPLETE     0x10 // 0001 0000

void initialize_PORT();
void initialize_ADC();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high;
	
     initialize_PORT();
     initialize_ADC();

     while(1){
        while((ADCSRA & ADC_COMPLETE)==0x00);  // 반복할 때, start 명령이 없음
//      ad_value_low =ADCL;                    // 8bit 변환값만 필요 할 때, ADCL은 무시하고 읽지않아도 된다
        ad_value_high=ADCH;     
        PORTA = PORTB= ad_value_high;  

        ADCSRA |= RESET_ADC_COMPLETE;
     }
}

void initialize_PORT()
{
     DDRA =DDRB = 0xFF;  // PORTA => 8-LED, PORTB => GAL+7-seg.LED 동시에 display
     PORTA=PORTB= 0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;
     ADCSRA= ADC_ENABLE|ADC_START_CONVERSION|FREE_RUNNING_MODE|PRESCALE_128; // start 명령을 한번만!	
}

