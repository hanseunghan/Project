//*** exp601C.c  =>  exp601A���� single mode�� free running mode��  *** 

#include <avr/io.h>

#define AREF_SELECT               0x00  
#define ADC_LEFT_ADJUSTED_RESULT  0x20  
#define ADC0                      0  

#define ADC_ENABLE	       0x80  
#define ADC_START_CONVERSION   0x40  
#define FREE_RUNNING_MODE      0x20  // ADC�ݺ��� �ڵ�����!
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
        while((ADCSRA & ADC_COMPLETE)==0x00);  // �ݺ��� ��, start ����� ����
//      ad_value_low =ADCL;                    // 8bit ��ȯ���� �ʿ� �� ��, ADCL�� �����ϰ� �����ʾƵ� �ȴ�
        ad_value_high=ADCH;     
        PORTA = PORTB= ad_value_high;  

        ADCSRA |= RESET_ADC_COMPLETE;
     }
}

void initialize_PORT()
{
     DDRA =DDRB = 0xFF;  // PORTA => 8-LED, PORTB => GAL+7-seg.LED ���ÿ� display
     PORTA=PORTB= 0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;
     ADCSRA= ADC_ENABLE|ADC_START_CONVERSION|FREE_RUNNING_MODE|PRESCALE_128; // start ����� �ѹ���!	
}

