//*** exp604.c ***

#include <avr/io.h>

#define AREF_SELECT               0x00  
#define ADC_LEFT_ADJUSTED_RESULT  0x20   
#define ADC0                      0 
 
#define ADC_ENABLE             0x80  
#define ADC_START_CONVERSION   0x40  
#define SINGLE_MODE            0x00  
#define PRESCALE_32            0x05  
#define ADC_COMPLETE           0x10  
#define RESET_ADC_COMPLETE     0x10 

#define SW0         0x01
#define SW1         0x02
#define PE_7_HIGH   0x80
#define PE_7_LOW    0x00

void initialize_PORT();
void initialize_ADC();

int  main(void)
{
     unsigned char advalue;
	
     initialize_PORT();
     initialize_ADC();

     while(1){
        PORTA=PORTB=0x00;
        while((PIND & SW0) == 0x00);        //start

        PORTA=PORTB=0xFF;                    
        PORTA=PORTB=0x00;                   //약 120ns

        ADCSRA|=ADC_START_CONVERSION;       //ADC 시작; register는 OR연산가능?
        while((ADCSRA & ADC_COMPLETE)==0x00); 	
        advalue=ADCH;	                    //ADC 완료; 약 53us
        PORTA=PORTB=advalue;

        PORTE=PE_7_HIGH;                    //1 line 실행시간은  약 250ns
        PORTE=PE_7_LOW;

        ADCSRA |= RESET_ADC_COMPLETE;
        while((PIND & SW1) == 0x00);        // pause-> again!
    }
}

void initialize_PORT()
{
     DDRA=0xFF;
     DDRB=0xFF;  // PORTA와 동시출력하면 측정가능
     DDRE=0xFF;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;
     ADCSRA= ADC_ENABLE|SINGLE_MODE|PRESCALE_32;	
}
