//*** exp916A.c --- normal mode
//              --- input capture interrupt, overflow interupt 이용 ***

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char  edge_number, timer1_overflow_number, one_period_overflow_number;
unsigned int first_edge_time, second_edge_time, one_period_time_ms;
unsigned long total_period_clock;

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
#define NORMAL_MODE_HIGH       0x00  // 0000 0000 ; MODE-0
#define NORMAL_MODE_LOW        0x00  // 0000 0000
#define NORMAL_PORT_OPERATION  0x00  // 0000 0000

#define INPUT_CAPTURE_ON_RISING_EDGE    0x40  // 0100 0000
#define PRESCALE_64                     0x03  // 0000 0011 ;250 kHz(4us)
#define TIMER1_INPUT_CAPTURE_INT_ENABLE 0x20  // 0010 0000
#define TIMER1_OVERFLOW_INT_ENABLE      0x04  // 0000 0100	

#define FIRST_RISING  1
#define SECOND_RISING 2
#define ONE_MS_CLOCK  250   // 4us * 250 = 1ms

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
void calculate_Period();

int  main(void )
{
     initialize_MCU();
     initialize_LCD();
     initialize_Display();

     initialize_PORT();
     initialize_TIMER1();
     initialize_INTERRUPT();

     sei();
		
     while(1){
        PORTF = PF_7_HIGH;  // 주기 약 20ms
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


//Timer1
void initialize_PORT()
{
//   DDRD = 0x00;  // input pulse ICP1(PD4) 
     DDRF = 0xFF;  // pulse 출력
     PORTF= 0x00;
}

void initialize_TIMER1()
{
     TCCR1A = NORMAL_MODE_LOW | NORMAL_PORT_OPERATION;
     TCCR1B = NORMAL_MODE_HIGH| INPUT_CAPTURE_ON_RISING_EDGE| PRESCALE_64;

     edge_number=FIRST_RISING;
}

void initialize_INTERRUPT()
{
     TIMSK = TIMER1_INPUT_CAPTURE_INT_ENABLE|TIMER1_OVERFLOW_INT_ENABLE;  
}

ISR(TIMER1_CAPT_vect)
{
    if     (edge_number==FIRST_RISING) {
            timer1_overflow_number=0;
            first_edge_time=ICR1;
            edge_number=SECOND_RISING; 
    }
    else if(edge_number==SECOND_RISING){
            second_edge_time=ICR1;
            one_period_overflow_number=timer1_overflow_number;
            timer1_overflow_number=0;          

            calculate_Period();
            first_edge_time=second_edge_time; 
            display_Period();
    }
}

ISR(TIMER1_OVF_vect)
{
    timer1_overflow_number++; // overflow 1번마다 4us*65536=262144us=262.144ms 소요
}

void calculate_Period()
{
     total_period_clock = (65536-first_edge_time)+(one_period_overflow_number-1)*65536
                         + second_edge_time;
     one_period_time_ms = total_period_clock/ONE_MS_CLOCK; // <=9999; 4us로 측정하여 ms로 환산
}

void display_Period()
{
     unsigned char number0, number3, number4 ;
     unsigned int  number2;
     char          display_number[6];

     number0 = one_period_time_ms/1000;   // 소수3째자리 10진수의 네숫자를 ascii code로 변환
     display_number[0] = number0 + 0x30;  
     display_number[1] = '.';
     number2 = one_period_time_ms%1000;
     display_number[2] = number2/100 + 0x30;
     number3 = number2%100;
     display_number[3] = number3/10 + 0x30;
     number4 = number3%10;
     display_number[4] = number4 + 0x30;
     display_number[5] = 0;

     write_string(7,0,display_number);
}

