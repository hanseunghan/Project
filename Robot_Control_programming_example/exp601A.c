//*** exp601A.c => single mode,왼쪽 정렬(left adjusted) ; 8bit 변환시 편리하다 ***

#include <avr/io.h>

#define AREF_SELECT              0x00 // 0000 0000; ref.voltage는 external AREF입력전압
#define ADC_LEFT_ADJUSTED_RESULT 0x20 // 0010 0000; ADC 결과값을 좌측정렬하여 저장 
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
         ADCSRA=ADCSRA | ADC_START_CONVERSION; // 반복동작할 때마다 start 명령을 주어야 함
         while((ADCSRA & ADC_COMPLETE)==0x00); // delay를 쓰지말고 polling 방식으로! 
         ad_value_low =ADCL& 0xC0; // AD변환 결과값은 반드시 하위 byte를 먼저 읽어 저장하고,
         ad_value_high=ADCH ;      //                        상위 byte를 나중에 읽어 저장해야 함 
                                   // ADCH만 읽어내어도 ADCL과 ADCH가 다음 변환때 update가능함
                                   // ADCL과 ADCH값은 다음 변환직전까지 그대로 유지하다가,
                                   // 다음 변환후에 update됨
         ad_value_total=(ad_value_high<<2) + (ad_value_low>>6); // 16 bit로 연산해야함.                                                     // 16bit <- 16bit + 8bit
         PORTA = ad_value_total>>2; // 하위 4bit를 버린다면? (>>2 또는 /4)
//       PORTA = ad_value_high;     // 3V 입력시 어떻게 나타날까? =>8 bit 변환값으로

         ADCSRA = ADCSRA | RESET_ADC_COMPLETE;// 1을 overwrite -> flag bit를 clear(0)해야함. 
                                              // AND operation? OR operation?
                                              // 생략하면 마치 반복동작하는 것처럼 보일 뿐!
     }
}
 
void initialize_PORT()
{
     DDRA=0xFF;   // AD변환 결과값 확인 -> 8 LED
//   DDRF=0x00;   // AD변환이 미리 설정되므로 필요없음!
	
     PORTA=0x00;
}

void initialize_ADC()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUSTED_RESULT|ADC0;	
     ADCSRA= ADC_ENABLE|SINGLE_MODE|PRESCALE_128;  // start 명령만 빼고 미리 설정
}
