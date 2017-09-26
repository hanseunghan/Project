// **** exp703.c --- 16*2 LCD display ST7066U ---

#include <avr/io.h>

char number1[3], number2[5];

#define WRITE_LCD_IR *((unsigned char *)0x7000)
#define WRITE_LCD_DR *((unsigned char *)0x7001)

#define EXTERNAL_MEMORY_INTERFACE  0x80  

#define FUNCTION_SET            0x20
#define _8_BIT_BUS              0x10
#define _2_LINE_DISPLAY         0x08
#define _5_8_FONT               0x00

#define DISPLAY_ON_OFF_CONTROL 	0x08
#define DISPLAY_ON       	0x04
#define DISPLAY_OFF      	0x00
#define CURSOR_ON        	0x02
#define CURSOR_OFF       	0x00
#define BLINK_ON         	0x01
#define BLINK_OFF        	0x00

#define ENTRY_MODE_SET   	0x04
#define ENTIRE_SHIFT_OFF	0x00
#define CURSOR_SHIFT_LEFT	0x00
#define CURSOR_SHIFT_RIGHT   	0x02
#define ENTIRE_SHIFT_ON  	0x01
#define DISPLAY_SHIFT_LEFT	0x02
#define DISPLAY_SHIFT_RIGHT	0x00

#define DISPLAY_CLEAR      	0x01
#define CURSOR_RETURN_HOME 	0x02
#define SET_DDRAM_ADDRESS  	0x80 

#define CURSOR_DISPLAY_SHIFT 	0x10
#define DISPLAY_SHIFT    	0x08
#define CURSOR_SHIFT     	0x00
#define SHIFT_RIGHT      	0x04 
#define SHIFT_LEFT       	0x00

#define RIGHT   1
#define LEFT    2
#define NO      0
#define ON      1
#define OFF     2

void initialize_MCU();
void initialize_LCD();
void delay_us(unsigned char time_us);
void delay_ms(unsigned int  time_ms);

void clear_screen(void);
void cursor_home(void);
void write_1_character(unsigned char c);
void set_coordinate(unsigned char x,unsigned char y);
void write_string(unsigned char x,unsigned char y,char *str);
void hexainteger_2_ascii(unsigned char hexavalue);
void decimalinteger_4_ascii(unsigned int decimalvalue);

int  main( )
{
     unsigned char a,b,sum1;
     unsigned int c,d,sum2;
	 
     initialize_MCU();
     initialize_LCD();

     a=0xA3;
     b=0x2C;
     sum1=a+b;
     hexainteger_2_ascii(sum1);
     write_string(2,0,number1);
     
     c=1234;
     d=6789;
     sum2=c+d;
     decimalinteger_4_ascii(sum2);
     write_string(2,1,number2);

     while(1);   
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

void hexainteger_2_ascii(unsigned char hexavalue)
{
     unsigned char hexa_high, hexa_low, ascii_high, ascii_low;

     hexa_high = hexavalue>>4;
     if (hexa_high>=0x0A) ascii_high = hexa_high - 0x0A + 0x41;
     else                 ascii_high = hexa_high + 0x30;
     number1[0] = ascii_high;
//     *(number1+0) = ascii_high;

     hexa_low  = hexavalue & 0x0F;
     if (hexa_low>=0x0A)  ascii_low = hexa_low - 0x0A + 0x41;
     else                 ascii_low = hexa_low + 0x30;
     number1[1] = ascii_low;
//     *(number1+1) = ascii_low;
     number1[2] = 0;  // null character
//     *(number1+2) = 0; 
}

void decimalinteger_4_ascii(unsigned int decimalvalue)
{
     unsigned char vd;

     vd = decimalvalue / 1000;
     number2[0]   = vd + 0x30;
     decimalvalue = decimalvalue % 1000;

     vd = decimalvalue / 100;
     number2[1]   = vd + 0x30;
     decimalvalue = decimalvalue % 100;

     vd = decimalvalue / 10;
     number2[2]   = vd + 0x30;
     decimalvalue = decimalvalue % 10;

     number2[3]   = decimalvalue + 0x30; 
     number2[4]   = 0;
}
