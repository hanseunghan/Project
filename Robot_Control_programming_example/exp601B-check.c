//*** exp601B-check.c => single mode,right adjusted,불편함; 10bit 변환시에만 사용함***

#include <avr/io.h>

#define AREF_SELECT               0x00  
#define ADC_RIGHT_ADJUSTED_RESULT 0x00  // 0000 0000 ; 우측정렬 
#define ADC0                      0  

#define ADC_ENABLE             0x80  
#define ADC_START_CONVERSION   0x40  
#define SINGLE_MODE            0x00  
#define PRESCALE_128	       0x07  
#define ADC_COMPLETE 	       0x10 
#define RESET_ADC_COMPLETE     0x10 

#define SW0  0x01
#define SW1  0x02
#define SW2  0x04
#define SW3  0x08
#define SW4  0x10

void initialize_PORT();
void initialize_ADC();
void delay();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high;  
     unsigned int  ad_value_total;
	
     initialize_PORT();
     initialize_ADC();

     while(1){
        ADCSRA |= ADC_START_CONVERSION;
        PORTA=ADCSRA;               // ?

        while((PIND & SW0)==0x00);

        while((ADCSRA & ADC_COMPLETE)==0x00); 
        PORTA=ADCSRA;               // ?

        while((PIND & SW1)==0x00);
        ad_value_low=ADCL;          // 3V 입력시 어떻게 나타날까?  
        PORTA=ADCSRA;               // ? 
        PORTB=ad_value_low;         // 0x66 ~ 오차 많음; AREF=5.0 volt ?

        while((PIND & SW2)==0x00);

        ad_value_high=ADCH & 0x03;
        PORTA=ADCSRA;               // ?
        PORTB=ad_value_high;        // 0x02

        while((PIND & SW3)==0x00);

        ad_value_total=(ad_value_high<<8) + ad_value_low;  // <<8 또는 *256

        PORTB =ad_value_high;       // 0x02
        delay();

        PORTB =ad_value_low;        // 0x66
        delay();

        PORTB = ad_value_total>>2;  // 0x99
        delay();

        ADCSRA |= RESET_ADC_COMPLETE;
        PORTA=ADCSRA;               // ?

        while((PIND & SW4)==0x00);
     }
}

void initialize_PORT()
{
     DDRA=0xFF;  // register 확인   -> 8 LEDD
     DDRB=0xFF;  // ADC result 확인 -> GAL+7-seg.LED
     DDRD=0x00;  // pause switch in process
	
     PORTA=0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_RIGHT_ADJUSTED_RESULT|ADC0;
     ADCSRA= ADC_ENABLE|SINGLE_MODE|PRESCALE_128;	
}

void delay()
{     
     unsigned char i,ti;
     unsigned int  j,tj;

     ti=20; tj=50000;   
     for(i=0;i<ti;i++)
         for(j=0;j<tj;j++); 
}

