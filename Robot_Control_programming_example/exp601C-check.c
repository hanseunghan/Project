//*** exp601C-check.c ; exp601A에서 single mode -> free running mode로*** 

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

#define SW0  0x01
#define SW1  0x02
#define SW2  0x04
#define SW3  0x08

void initialize_PORT();
void initialize_ADC();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high;
	
     initialize_PORT();
     initialize_ADC();

     while(1){

        PORTA=ADCSRA;            // ?

        while((PIND & SW0)==0x00);

        while((ADCSRA & ADC_COMPLETE)==0x00);
        PORTA=ADCSRA;            // ?

        while((PIND & SW1)==0x00);

        ad_value_high=ADCH;
        PORTA=ADCSRA;            // ?
        PORTB= ad_value_high;    // 3 volt 입력시?

        while((PIND & SW2)==0x00);
     
        ADCSRA |= RESET_ADC_COMPLETE;
        PORTA=ADCSRA;            // ?
        PORTB= ad_value_high;    // ?  

        while((PIND & SW3)==0x00);
     }
}

void initialize_PORT()
{
     DDRA =DDRB = 0xFF;   
     PORTA=PORTB= 0x00;
}

void initialize_ADC()
{
     ADMUX  = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;
     ADCSRA = ADC_ENABLE|ADC_START_CONVERSION|FREE_RUNNING_MODE|PRESCALE_128;	
}

