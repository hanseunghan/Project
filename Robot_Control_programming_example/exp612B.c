// *** exp612B.c - 전체 전압범위에서 작성 ***

#include <avr/io.h>

unsigned char advalue, duty_Ratio, direction;

//switch
#define SW0    0x01
#define SW7    0x80

//Timer0
#define PWM_MODE                    0x00 
#define FAST_PWM_MODE               0x48  
#define OUTPUT_LOW_ON_COMPARE_MATCH 0x20 
#define PRESCALE_8                  0x02

//DC motor 
#define CCW_ROTATION 	0x80 
#define CW_ROTATION  	0x40 
#define FREE_STOP   	0x00 
#define DUTYRATIO_MAX   0xFF
#define DUTYRATIO_MIN   0x20
#define DUTYRATIO_MID   0xC0

//ADC
#define AREF_SELECT              0x00  
#define ADC_LEFT_ADJUSTED_RESULT 0x20   
#define ADC5                     5  

#define ADC_ENABLE             0x80  
#define ADC_START_CONVERSION   0x40  
#define PRESCALE_32            0x05  
#define SINGLE_MODE            0x00 
#define ADC_COMPLETE           0x10  
#define RESET_ADC_COMPLETE     0x10 

#define AD0VOLT  0x00
#define AD1VOLT  0x33
#define AD2VOLT  0x66
#define AD3VOLT  0x99
#define AD4VOLT  0xCC
#define AD5VOLT  0xFF

void initialize_PORT();
void initialize_ADC();
void initialize_TIMER0_PWM();
void initialize_DC_motor();
void ad_Conversion();
void calculate_DutyRatio();
void rotate_DC_motor();

int  main(void)
{
     initialize_PORT();
     initialize_ADC();
     initialize_TIMER0_PWM();

     while(1){
        initialize_DC_motor();
        while((PIND & SW0)==0x00);
        while(1){
           if((PIND & SW7)!=0x00) break;
           ad_Conversion();
           calculate_DutyRatio(); 
           rotate_DC_motor();
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
     OCR0 =0x00;
     PORTA=OCR0;
}

void ad_Conversion()
{
     ADCSRA |= ADC_START_CONVERSION;
     while((ADCSRA & ADC_COMPLETE)==0x00);
     advalue=ADCH;
     ADCSRA |= RESET_ADC_COMPLETE;
}

void calculate_DutyRatio()
{
     if (AD0VOLT<=advalue && advalue<=AD1VOLT){
              duty_Ratio=+(DUTYRATIO_MAX-DUTYRATIO_MIN)*(advalue-AD0VOLT)/(AD1VOLT-AD0VOLT) + DUTYRATIO_MIN; 
                     //+0xDF*advalue/0x33 + 0x20
              direction=CCW_ROTATION;
     }
     else if (AD1VOLT<advalue && advalue<=AD2VOLT){
              duty_Ratio=+(DUTYRATIO_MIN-DUTYRATIO_MAX)*(advalue-AD1VOLT)/(AD2VOLT-AD1VOLT) + DUTYRATIO_MAX;
                     //-0xDF*advalue/0x33 + (0xDF+0xFF)
              direction=CCW_ROTATION;
     }
     else if (AD2VOLT<advalue && advalue<=AD3VOLT){
              duty_Ratio=(DUTYRATIO_MAX-DUTYRATIO_MIN)*(advalue-AD2VOLT)/(AD3VOLT-AD2VOLT) + DUTYRATIO_MIN; 
                     //+0xDF*advalue/0x33 + (-2*0xDF+0x20)
              direction=CW_ROTATION;
     }
     else if (AD3VOLT<advalue && advalue<=AD4VOLT){
              duty_Ratio=+(DUTYRATIO_MIN-DUTYRATIO_MAX)*(advalue-AD3VOLT)/(AD4VOLT-AD3VOLT) + DUTYRATIO_MAX; 
                     //-0xDF*advalue/0x33 + (+3*0xDF+0x20)
              direction=CW_ROTATION;
     }
     else if (AD4VOLT<advalue && advalue<=AD5VOLT){
              duty_Ratio=+DUTYRATIO_MID;       //0xC0
              direction=CW_ROTATION;
     }

}

void rotate_DC_motor()
{
     PORTB=direction;
     OCR0=duty_Ratio;
     PORTA=duty_Ratio;
}