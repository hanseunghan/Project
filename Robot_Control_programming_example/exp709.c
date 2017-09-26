// **** exp709.c --- '빛이되는100년' 한글 display ---

#include <avr/io.h>

const unsigned char HG_font[40]={
      0x15,0x1D,0x15,0x1D,0x00,0x04,0x1F,0x0A,  // 빛
      0x01,0x09,0x15,0x15,0x09,0x01,0x01,0x00,  // 이
      0x01,0x1D,0x11,0x1D,0x09,0x1D,0x01,0x00,  // 되
      0x08,0x0E,0x00,0x1F,0x00,0x08,0x0E,0x00,  // 는
      0x17,0x11,0x17,0x11,0x1D,0x00,0x10,0x1F}; // 년

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
#define CURSOR_SHIFT_RIGHT  0x02
#define ENTIRE_SHIFT_ON  	0x01
#define DISPLAY_SHIFT_LEFT	0x02
#define DISPLAY_SHIFT_RIGHT	0x00

#define DISPLAY_CLEAR      	0x01
#define CURSOR_RETURN_HOME 	0x02
#define SET_DDRAM_ADDRESS  	0x80 

#define CURSOR_DISPLAY_SHIFT 0x10
#define DISPLAY_SHIFT    	0x08
#define CURSOR_SHIFT     	0x00
#define SHIFT_RIGHT      	0x04 
#define SHIFT_LEFT       	0x00

#define RIGHT   1
#define LEFT    2
#define NO      0
#define ON      1
#define OFF     2

#define SET_CGRAM_ADDRESS      0x40
#define START_CGRAM_ADDRESS    0x00
#define MAX_CHARACETER_CODE    0x04
#define INITIAL_X_COORDINATE   5
#define INITIAL_Y_COORDINATE   1

void initialize_MCU();
void initialize_LCD();
void delay_us(unsigned char time_us);
void delay_ms(unsigned int  time_ms);

void write_1_character(unsigned char c);
void set_coordinate(unsigned char x,unsigned char y);
void write_string(unsigned char x,unsigned char y,char *str);
void making_character_font();
void display_HG();
void clear_screen(void);
void shift_display(unsigned char direction);

int  main( )
{
     initialize_MCU();
     initialize_LCD();
     
     making_character_font();
     while(1) display_HG();     
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

void making_character_font()
{
     unsigned char character_CODE, font_line_number;

     for(character_CODE=0x00;character_CODE<=MAX_CHARACETER_CODE;character_CODE++){
         WRITE_LCD_IR = SET_CGRAM_ADDRESS | (START_CGRAM_ADDRESS + 8*character_CODE);
         delay_us(40); 
         for(font_line_number=0;font_line_number<8;font_line_number++){
             WRITE_LCD_DR = HG_font[8*character_CODE + font_line_number];
             delay_us(40); 
         }
     }
}

void display_HG()
{
     unsigned char x,y,character_CODE,i;

     x=INITIAL_X_COORDINATE; y=INITIAL_Y_COORDINATE;
     set_coordinate(x, y);                  // DDRAM address를 지정

     for(character_CODE=0x00;character_CODE<=MAX_CHARACETER_CODE-1;character_CODE++){
         write_1_character(character_CODE); // character-code를 지정
         delay_us(40);
     }

     write_string(INITIAL_X_COORDINATE+character_CODE,INITIAL_Y_COORDINATE, "100");

     write_1_character(character_CODE);     // character-code 0x04
     delay_us(40);
     delay_ms(500);

     for(i=1;i<=5;i++){
	 shift_display(LEFT);
	 delay_ms(500);
     }

     clear_screen();
     delay_ms(500);
}

void shift_display(unsigned char direction)
{
     if      (direction==RIGHT) 
              WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|DISPLAY_SHIFT|SHIFT_RIGHT;
     else if (direction==LEFT)  
              WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|DISPLAY_SHIFT|SHIFT_LEFT;
     delay_us(40);
}

void clear_screen(void)
{
     WRITE_LCD_IR = DISPLAY_CLEAR;
     delay_ms(2);    
} 
