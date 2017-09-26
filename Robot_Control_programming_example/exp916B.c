//*** exp916B.c --- CTC mode
//              --- input capture interrupt, output compare match interrupt 이용 ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char edge_number, ms_number;
unsigned int one_period_time_ms;
unsigned long _100_us_number, one_period_time_100us;  // 10초=10000ms=100000*0.1ms

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

//Timer1
#define CTC_MODE_HIGH          0x08  // Mode-4
#define CTC_MODE_LOW           0x00
#define NORMAL_PORT_OPERATION  0x00
#define INPUT_CAPTURE_ON_RISING_EDGE    0x40  // 0100 0000
#define PRESCALE_64                     0x03  // 0000 0011 ; 250 kHz(4us)

#define TIMER1_INPUT_CAPTURE_INT_ENABLE           0x20  // 0010 0000
#define TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE  0x10  // 0001 0000

#define FIRST_RISING   1
#define SECOND_RISING  2
#define _100_US_CLOCK  25   // 4us * 25 = 100us=0.1ms, 4us * 250 = 1ms

//Main Pulse Out
#define PF_7_HIGH  0x80
#define PF_7_LOW   0x00	

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
void display_Period();

void initialize_PORT();
void initialize_TIMER1();
void initialize_INTERRUPT();

int main(void )
{
    initialize_MCU();
    initialize_LCD();
    initialize_Display();

    initialize_PORT();
    initialize_TIMER1();
    initialize_INTERRUPT();

    sei();
		
    while(1){
       PORTF = PF_7_HIGH;   // 주기 약 20ms
       delay_ms(7); 
       PORTF = PF_7_LOW;
       delay_ms(6);
    }
}

//LCD
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

void delay_ms(unsigned int  time_ms)	
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
     write_string(0,0,"period=");
     write_string(12,0,"s"); 
}

void display_Period()
{
     unsigned char number0, number3, number4 ;
     unsigned int number2;
     char display_number[6];

     one_period_time_ms= one_period_time_100us/10; // 0.1ms로 측정하여 1ms로 display

     number0    = one_period_time_ms/1000;  // 소수3째자리 10진수의 네숫자를 ascii code로 변환
     display_number[0] = number0 + 0x30;  
     display_number[1] = '.';
     number2    = one_period_time_ms%1000;
     display_number[2] = number2/100 + 0x30;
     number3    = number2%100;
     display_number[3] = number3/10 + 0x30;
     number4    = number3%10;
     display_number[4] = number4 + 0x30;
     display_number[5] = 0;

     write_string(7,0,display_number);
}

//Timer1
void initialize_PORT()
{
//	 DDRD = 0x00;  // input pulse ICP1(PD4) 
     DDRF = 0xFF;  // pulse 출력
     PORTF= 0x00;
}

void initialize_TIMER1()
{
     TCCR1A = CTC_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = CTC_MODE_HIGH| INPUT_CAPTURE_ON_RISING_EDGE| PRESCALE_64;

     OCR1A = _100_US_CLOCK-1;    // 25-1; 1 clock before 
                                 // 일정한 시간을 계산 설정한 후, interrupt를 반복 발생시키는 방법!!
     edge_number= FIRST_RISING;  // interrupt후 TCNT1->0000으로 clear!!
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER1_INPUT_CAPTURE_INT_ENABLE| TIMER1_OUTPUT_COMPARE_A_MATCH_INT_ENABLE;  
}

ISR(TIMER1_COMPA_vect)
{
    _100_us_number++;  // 오차를 고려하여 0.1ms=100us 시계로 측정. interrupt 발생때마다 countup.
} 

ISR(TIMER1_CAPT_vect)
{
    if     (edge_number==FIRST_RISING){
//          TCNT1=0x0000;    // 다른 동작에 영향이 있을때는 clear하면 안됨!
            _100_us_number=0;
            edge_number=SECOND_RISING;
    }
    else if(edge_number==SECOND_RISING){
            one_period_time_100us=_100_us_number;
            TCNT1=0x0000;    // 다른 동작에 영향이 있을때는 clear하면 안됨!          
            _100_us_number=0;
            display_Period();
   }
}


