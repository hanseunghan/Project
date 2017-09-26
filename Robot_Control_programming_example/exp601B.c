//*** exp601B.c => single mode,������ ����(right adjusted); 10bit ��ȯ���� �����. 8 bit ��ȯ���� ����! ***

#include <avr/io.h>

#define AREF_SELECT               0x00  
#define ADC_RIGHT_ADJUSTED_RESULT 0x00  // 0000 0000; �������� 
#define ADC0                      0  

#define ADC_ENABLE	       0x80  
#define ADC_START_CONVERSION   0x40  
#define SINGLE_MODE            0x00  
#define PRESCALE_128	       0x07  
#define ADC_COMPLETE 	       0x10 
#define RESET_ADC_COMPLETE     0x10 

void initialize_PORT();
void initialize_ADC();
void delay();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high;  // unsigned int ?  -> OK!
     unsigned int  ad_value_total;
	
     initialize_PORT();
     initialize_ADC();

     while(1){
        ADCSRA|=ADC_START_CONVERSION;
        while((ADCSRA & ADC_COMPLETE)==0x00);

        ad_value_low =ADCL;
        ad_value_high=ADCH & 0x03;

        ad_value_total=(ad_value_high<<8)+ad_value_low; // 16bit ���갡��->10bit adc ���������.
        PORTB=ad_value_total;                           // ad_value_high<<8 (�Ǵ�*256)

        PORTA =ad_value_high;       // 3V �Է½� ��� ��Ÿ����?  0x02
        delay();

        PORTA =ad_value_low;        // 0x66 ~ ���� ����; AREF=5.0 volt ?
        delay();

        PORTA = ad_value_total>>2;  // 0x99 ~ ���� �ణ; 8 bit ��ȯ
        delay();

        ADCSRA |= RESET_ADC_COMPLETE;
    }
}

void initialize_PORT()
{
     DDRA=0xFF;  // ADC result Ȯ�� -> 8 LED
     DDRB=0xFF;  // ADC result Ȯ�� -> GAL+7-seg.LED
     DDRD=0x00;
	
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

