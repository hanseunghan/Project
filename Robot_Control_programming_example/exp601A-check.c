//*** exp601A-check.c => single mode,왼쪽 정렬(left adjusted)***

#include <avr/io.h>

#define AREF_SELECT              0x00  // 0000 0000; 
#define ADC_LEFT_ADJUSTED_RESULT 0x20  // 0010 0000; 
#define ADC0                     0     // 0000 0000; 

#define ADC_ENABLE            0x80 // 1000 0000
#define ADC_START_CONVERSION  0x40 // 0100 0000
#define SINGLE_MODE           0x00 // 0000 0000
#define PRESCALE_128          0x07 // 0000 0111
#define ADC_COMPLETE          0x10 // 0001 0000
#define RESET_ADC_COMPLETE    0x10 // 0001 0000

#define SW0  0x01
#define SW1  0x02
#define SW2  0x04
#define SW3  0x08
#define SW4  0x10

void initialize_PORT();
void initialize_ADC();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high; 
     unsigned int  ad_value_total;
  
     initialize_PORT();
     initialize_ADC();
  
     while(1){
         ADCSRA= ADCSRA | ADC_START_CONVERSION;
         PORTA=ADCSRA;                 // ?

         while((PIND & SW0)==0x00);

         while((ADCSRA & ADC_COMPLETE)==0x00);  
         PORTA=ADCSRA;                 // ?

         while((PIND & SW1)==0x00);

         ad_value_low =ADCL & 0xC0 ;    // 3V 입력시 어떻게 나타날까? 0x99
         ad_value_high=ADCH ;                 // & 1100 0000  ->     0x80 ~ 약간오차
         ad_value_total=(ad_value_high<<2) + (ad_value_low>>6); 
         PORTB = ad_value_total>>2;   // 0x99

         while((PIND & SW2)==0x00);
         
         PORTB = ad_value_high;       // 0x99  => 8 bit 변환값으로
         PORTA = ADCSRA;              // ?
         
         while((PIND & SW3)==0x00);

         ADCSRA = ADCSRA | RESET_ADC_COMPLETE; 
         PORTA  = ADCSRA;             // ?	  

         while((PIND & SW4)==0x00);
     }
}
 
void initialize_PORT()
{
     DDRA=0xFF;   // register 확인   -> 8 LED
     DDRB=0xFF;   // ADC result 확인 -> GAL+7-seg.LED
     DDRD=0x00;   // pause switch in process
	
     PORTB=0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT | ADC_LEFT_ADJUSTED_RESULT | ADC0;
     ADCSRA= ADC_ENABLE | SINGLE_MODE | PRESCALE_128;	
}

