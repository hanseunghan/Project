//*** exp601A.c => single mode,���� ����(left adjusted) ; 8bit ��ȯ�� ���ϴ� ***

#include <avr/io.h>

#define AREF_SELECT              0x00 // 0000 0000; ref.voltage�� external AREF�Է�����
#define ADC_LEFT_ADJUSTED_RESULT 0x20 // 0010 0000; ADC ������� ���������Ͽ� ���� 
#define ADC0                     0    // 0000 0000; #0 channel

#define ADC_ENABLE             0x80 // 1000 0000
#define ADC_START_CONVERSION   0x40 // 0100 0000
#define SINGLE_MODE            0x00 // 0000 0000
#define PRESCALE_128           0x07 // 0000 0111
#define ADC_COMPLETE           0x10 // 0001 0000
#define RESET_ADC_COMPLETE     0x10 // 0001 0000

void initialize_PORT();
void initialize_ADC();

int  main(void)
{
     unsigned char ad_value_low, ad_value_high;  // unsigned int ? -> OK!
     unsigned int  ad_value_total;

     initialize_PORT();
     initialize_ADC();
  
     while(1){
         ADCSRA=ADCSRA | ADC_START_CONVERSION; // �ݺ������� ������ start ����� �־�� ��
         while((ADCSRA & ADC_COMPLETE)==0x00); // delay�� �������� polling �������! 
         ad_value_low =ADCL& 0xC0; // AD��ȯ ������� �ݵ�� ���� byte�� ���� �о� �����ϰ�,
         ad_value_high=ADCH ;      //                        ���� byte�� ���߿� �о� �����ؾ� �� 
                                   // ADCH�� �о� ADCL�� ADCH�� ���� ��ȯ�� update������
                                   // ADCL�� ADCH���� ���� ��ȯ�������� �״�� �����ϴٰ�,
                                   // ���� ��ȯ�Ŀ� update��
         ad_value_total=(ad_value_high<<2) + (ad_value_low>>6); // 16 bit�� �����ؾ���.                                                     // 16bit <- 16bit + 8bit
         PORTA = ad_value_total>>2; // ���� 4bit�� �����ٸ�? (>>2 �Ǵ� /4)
//       PORTA = ad_value_high;     // 3V �Է½� ��� ��Ÿ����? =>8 bit ��ȯ������

         ADCSRA = ADCSRA | RESET_ADC_COMPLETE;// 1�� overwrite -> flag bit�� clear(0)�ؾ���. 
                                              // AND operation? OR operation?
                                              // �����ϸ� ��ġ �ݺ������ϴ� ��ó�� ���� ��!
     }
}
 
void initialize_PORT()
{
     DDRA=0xFF;   // AD��ȯ ����� Ȯ�� -> 8 LED
//   DDRF=0x00;   // AD��ȯ�� �̸� �����ǹǷ� �ʿ����!
	
     PORTA=0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;	
     ADCSRA= ADC_ENABLE|SINGLE_MODE|PRESCALE_128;  // start ��ɸ� ���� �̸� ����
}
