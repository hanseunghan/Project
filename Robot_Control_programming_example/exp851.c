// *** exp851.c ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char advalue;
char          number0[3], number1[4];

//LCD
#define WRITE_LCD_IR *((unsigned char *)0x7000)
#define WRITE_LCD_DR *((unsigned char *)0x7001)
#define EXTERNAL_MEMORY_INTERFACE  0x80  

#define FUNCTION_SET     0x20
#define _8_BIT_BUS       0x10
#define _2_LINE_DISPLAY  0x08
#define _5_8_FONT        0x00

#define DISPLAY_ON_OFF_CONTROL 0x08
#define DISPLAY_ON             0x04
#define DISPLAY_OFF            0x00
#define CURSOR_ON              0x02
#define CURSOR_OFF             0x00
#define BLINK_ON               0x01
#define BLINK_OFF              0x00

#define ENTRY_MODE_SET      0x04
#define ENTIRE_SHIFT_OFF    0x00
#define CURSOR_SHIFT_LEFT   0x00
#define CURSOR_SHIFT_RIGHT  0x02
#define ENTIRE_SHIFT_ON     0x01
#define DISPLAY_SHIFT_LEFT  0x02
#define DISPLAY_SHIFT_RIGHT 0x00

#define DISPLAY_CLEAR       0x01
#define CURSOR_RETURN_HOME  0x02
#define SET_DDRAM_ADDRESS   0x80 

#define CURSOR_DISPLAY_SHIFT  0x10
#define DISPLAY_SHIFT         0x08
#define CURSOR_SHIFT          0x00
#define SHIFT_RIGHT           0x04 
#define SHIFT_LEFT            0x00

//AD Conversion
#define AREF_SELECT             0x00 // reference voltage-external AREF
#define ADC_LEFT_ADJUST_RESULT  0x20 // left adjust
#define ADC0                    0    // #0 channel
#define ADC_ENABLE              0x80
#define ADC_START_CONVERSION    0x40
#define ADC_COMPLETE_INT_ENABLE 0x08
#define SINGLE_MODE             0x00
#define PRESCALE_128            0x07

//Main Pulse Out
#define PF_7_HIGH 0x80
#define PF_7_LOW  0x00

void initialize_MCU();
void initialize_LCD();
void initialize_Display();
void delay_us(unsigned char time_us);
void delay_ms(unsigned int  time_ms);
void clear_screen(void);
void cursor_home(void);
void write_1_character(unsigned char c);
void set_coordinate(unsigned char x,unsigned char y);
void write_string(unsigned char x,unsigned char y,char *str);

void initialize_PORT();
void initialize_ADC_Interrupt();
void start_ADC();
void advalue_2_ascii();
void advalue_2_analog_voltage();

int  main( )
{
     initialize_MCU();
     initialize_LCD();
     initialize_Display();

     initialize_PORT();	      
     initialize_ADC_Interrupt();
     sei();

     while(1){
        PORTF=PF_7_HIGH;  // 주기는 대략 17ms - delay를 이용하여 주기를 맞출수 없다.
        delay_ms(1);      // PORTF 상위4bit를 사용하려면?  
        PORTF=PF_7_LOW;   // => Fuse Bit 설정시 JTAAG interface Enabled 선택하지 말아야!
        delay_ms(1);
    }
}

void initialize_MCU()
{
     MCUCR = EXTERNAL_MEMORY_INTERFACE;
}

void initialize_LCD()
{
     delay_ms(50);
     WRITE_LCD_IR = FUNCTION_SET|_8_BIT_BUS|_2_LINE_DISPLAY|_5_8_FONT;
     delay_us(40);
     WRITE_LCD_IR = FUNCTION_SET|_8_BIT_BUS|_2_LINE_DISPLAY|_5_8_FONT;
     delay_us(40);
     WRITE_LCD_IR = DISPLAY_ON_OFF_CONTROL|DISPLAY_ON|CURSOR_ON|BLINK_ON;
     delay_us(40);
     WRITE_LCD_IR = DISPLAY_CLEAR;
     delay_ms(2);
     WRITE_LCD_IR = ENTRY_MODE_SET|CURSOR_SHIFT_RIGHT|ENTIRE_SHIFT_OFF;
     delay_us(40);
}

void delay_us(unsigned char time_us)	
{
     register unsigned char i;

     for(i=0; i<time_us; i++){	        
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
         asm volatile(" PUSH  R0 ");	
         asm volatile(" POP   R0 ");	
     }  
}

void delay_ms(unsigned int time_ms)	
{
     register unsigned int i;

     for(i=0; i<time_ms; i++){ 
         delay_us(250);
         delay_us(250);
         delay_us(250);
         delay_us(250);
     }
}

void clear_screen(void)
{
     WRITE_LCD_IR = DISPLAY_CLEAR;
     delay_ms(2);    
} 

void cursor_home(void)
{
     WRITE_LCD_IR = CURSOR_RETURN_HOME;  
     delay_ms(2);     
}

void set_coordinate(unsigned char x, unsigned char y)
{
     if     (y==0) WRITE_LCD_IR = SET_DDRAM_ADDRESS | (0x00 + x);
     else if(y==1) WRITE_LCD_IR = SET_DDRAM_ADDRESS | (0x40 + x);
     delay_us(40);     
}

void write_1_character(unsigned char ch)
{
     WRITE_LCD_DR = ch;
     delay_us(40);    
}

void write_string(unsigned char x, unsigned char y, char *str)
{
     set_coordinate(x, y);
     while(*str) write_1_character(*str++);
}

void initialize_Display()
{
     write_string(0,0,"ADCH=");  // write_string(0,0,"ADCH=  H");깜박임,이중글자 현상
     write_string(7,0,"H"); 

     write_string(0,1,"ADC0=");
     write_string(8,1,"V");  
}

void initialize_PORT()
{
//   DDRA=0xFF;    // PORTA와 LCD 충돌
     DDRF=0x80;    // PF.7=>output
}

void initialize_ADC_Interrupt()
{
     ADMUX = AREF_SELECT|ADC_LEFT_ADJUST_RESULT|ADC0;     
     ADCSRA= ADC_ENABLE|SINGLE_MODE|ADC_COMPLETE_INT_ENABLE|PRESCALE_128;
     start_ADC();
}

void start_ADC()
{    
     ADCSRA |= ADC_START_CONVERSION;
}

ISR(ADC_vect)
{                         // ADC Interrupt Flag는 자동으로 clear됨!
     advalue=ADCH;
     advalue_2_ascii();
     write_string(5,0,number0);

     advalue_2_analog_voltage();
     write_string(5,1,number1);

     start_ADC();
}

void advalue_2_ascii()
{
     unsigned char hexa_high, hexa_low, ascii_high, ascii_low;

     hexa_high = advalue>>4;
     if (hexa_high>=0x0A) ascii_high = hexa_high - 0x0A + 0x41;
     else                 ascii_high = hexa_high + 0x30;
     number0[0] = ascii_high;

     hexa_low  = advalue & 0x0F;
     if (hexa_low>=0x0A)  ascii_low = hexa_low - 0x0A + 0x41;
     else                 ascii_low = hexa_low + 0x30;
     number0[1] = ascii_low;
     number0[2] = 0;  // null character
}

void advalue_2_analog_voltage()
{
     unsigned char advalue10;

     advalue10  = advalue*5*10/0xFF;    // *10 => 소수첫째자리까지 정수로
     number1[0] = advalue10/10 + 0x30;  // 10진수를 ascii code로
     number1[1] = '.';
     number1[2] = advalue10%10 + 0x30;
     number1[3] = 0;
}


