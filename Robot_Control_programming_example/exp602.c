//*** exp602.c  =>  differential vlotage  *** 

#include <avr/io.h>

#define AREF_SELECT            0x00  
#define ADC_LEFT_ADJUST_RESULT 0x20  
#define ADC0_1                 0x10  // diffrential vlotage ADC0 - ADC1

#define ADC_ENABLE	       0x80  
#define ADC_START_CONVERSION   0x40  
#define FREE_RUNNING_MODE      0x20   
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
        while((ADCSRA & ADC_COMPLETE)==0x00);
        ad_value_high=ADCH;     
        PORTA = PORTB= ad_value_high;
        ADCSRA |= RESET_ADC_COMPLETE;
     }
}

void initialize_PORT()
{
     DDRA =DDRB = 0xFF;   
     PORTA=PORTB= 0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUST_RESULT|ADC0_1;
     ADCSRA= ADC_ENABLE|ADC_START_CONVERSION|FREE_RUNNING_MODE|PRESCALE_128;	
}

