// *** exp612A.c - 우선 0～1V 범위에서만 작성 ***

#include <avr/io.h>

unsigned char advalue, duty_Ratio;

//switch
#define SW0     0x01
#define SW7     0x80

//Timer0_PWM
#define PWM_MODE                    0x00 
#define FAST_PWM_MODE               0x48  
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 
#define PRESCALE_8                  0x02

//DC motor 
#define CCW_ROTATION 0x80 
#define CW_ROTATION  0x40 
#define FREE_STOP    0x00 
#define DUTYRATIO_MAX      0xFF
#define DUTYRATIO_MIN      0x20

//ADC
#define AREF_SELECT              0x00  
#define ADC_LEFT_ADJUSTED_RESULT 0x20   
#define ADC5                     5      // AD_chanel#5 enable? -> fuse bit check !
 
#define ADC_ENABLE             0x80  
#define ADC_START_CONVERSION   0x40  
#define SINGLE_MODE            0x00  
#define PRESCALE_32            0x05  
#define ADC_COMPLETE           0x10  
#define RESET_ADC_COMPLETE     0x10 

#define AD0VOLT  0x00
#define AD1VOLT  0x33

void initialize_PORT();
     void initialize_ADC();
void initialize_TIMER0_PWM();
void initialize_DC_motor();
void ad_Conversion();

int  main(void)
{
     initialize_PORT();
     initialize_TIMER0_PWM();
     initialize_ADC();

     while(1){
        initialize_DC_motor();
        while((PIND & SW0)==0x00);
        while(1){
           if((PIND & SW7)!=0x00) break;
           ad_Conversion();
           if (AD0VOLT<=advalue && advalue<=AD1VOLT)
               duty_Ratio=+(DUTYRATIO_MAX-DUTYRATIO_MIN)*(advalue-AD0VOLT)/(AD1VOLT-AD0VOLT)+DUTYRATIO_MIN; 
                    //+0xDF*advalue/0x33 + 0x20
           PORTB=CCW_ROTATION;
           OCR0=duty_Ratio;
           PORTA=duty_Ratio;
        }
     }
}

void initialize_PORT()
{
     DDRA=0xFF;  // duty ratio out
     DDRB=0xFF;  // dc motor control 
     DDRD=0x00;  // start switch
     PORTA=0x00;
     PORTB=0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC5;	
     ADCSRA= ADC_ENABLE|SINGLE_MODE|PRESCALE_32;
}

void initialize_TIMER0_PWM()
{
     TCCR0 = PWM_MODE|FAST_PWM_MODE|OUTPUT_LOW_ON_COMPARE_MATCH|PRESCALE_8; 
     TCNT0 = 0x00; 
}

void initialize_DC_motor()
{
     PORTB=FREE_STOP;
     OCR0=0x00;
     PORTA=OCR0;
}

void ad_Conversion()
{
     ADCSRA |= ADC_START_CONVERSION;
     while((ADCSRA & ADC_COMPLETE)==0x00);
     advalue=ADCH;
     ADCSRA |= RESET_ADC_COMPLETE;
}

