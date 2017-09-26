//***exp702.c *** --- 16*2 LCD controller ST7066U ---

#include <avr/io.h>

#define WRITE_LCD_IR (*(unsigned char *)0x7000)
#define WRITE_LCD_DR (*(unsigned char *)0x7001)

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
void shift_display(unsigned char direction);
void shift_cursor(unsigned char direction);
void entry_shift(unsigned char direction);
void display_ON_OFF(unsigned char display,unsigned char cursor,unsigned char blink);

int  main(void)
{
     unsigned int N;
     char ch;

     initialize_MCU()
     initialize_LCD();
     delay_ms(200);

     write_string(0,0,"SEOUL");
     write_string(0,1,"NATIONAL");
     delay_ms(1000);   
     
     write_string(0,0,"UNIVERSITY");
     write_string(0,1,"OF SCIENCE AND TECHNOLOGY");     
     delay_ms(1000);  

     clear_screen();
     delay_ms(1000);  
     
     write_string(0,0,"Dept of MSD");
     write_string(0,1,"ATMEL ATmega128");     
     delay_ms(1000); 
     display_ON_OFF(OFF,ON,ON);
     delay_ms(1000); 

     display_ON_OFF(ON,ON,ON);
     delay_ms(1000);          
					
     set_coordinate(0,0);
     delay_ms(1000);     
     for(N=1;N<=8;N++){
         shift_cursor(RIGHT);
         delay_ms(1000);
     }
     display_ON_OFF(ON,ON,OFF);
     for(N=1;N<=8;N++){
         shift_cursor(LEFT);
         delay_ms(1000);
     }

     cursor_home();
     display_ON_OFF(ON,ON,ON);
     for(N=1;N<=15;N++){				
         if(N<=5) shift_display(RIGHT);
         else     shift_display(LEFT);
         delay_ms(1000);
     }						

     clear_screen();
     delay_ms(1000);     
     ch ='A';
     for(N=1;N<=45;N++){
         entry_shift(LEFT);
         write_1_character(ch++);
         if(ch>'Z') ch ='A';
         delay_ms(1000);      
     }
     delay_ms(1000);
     ch ='A';
     for(N=1;N<=3;N++){
         entry_shift(RIGHT);
         write_1_character(ch++);
         delay_ms(1000);      
     }
     delay_ms(1000);

     entry_shift(NO);
     clear_screen();
     write_string(1,0,"*END*"); 
     write_string(1,1,"*Bye*");         
                      
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

void write_string(unsigned char x, unsigned char y, char *str)
{
     set_coordinate(x, y);
     while(*str) write_1_character(*str++);
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

void shift_display(unsigned char direction)
{
     if      (direction==RIGHT) 
            WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|DISPLAY_SHIFT|SHIFT_RIGHT;
     else if (direction==LEFT)  
            WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|DISPLAY_SHIFT|SHIFT_LEFT;
     delay_us(40);
}

void shift_cursor(unsigned char direction)
{
     if      (direction==RIGHT) 
            WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|CURSOR_SHIFT|SHIFT_RIGHT;
     else if (direction==LEFT)  
            WRITE_LCD_IR = CURSOR_DISPLAY_SHIFT|CURSOR_SHIFT|SHIFT_LEFT;
     delay_us(40);
}

void entry_shift(unsigned char direction)
{
     if      (direction==RIGHT)
            WRITE_LCD_IR = ENTRY_MODE_SET|CURSOR_SHIFT_LEFT|ENTIRE_SHIFT_ON;
     else if (direction==LEFT) 
            WRITE_LCD_IR = ENTRY_MODE_SET|CURSOR_SHIFT_RIGHT|ENTIRE_SHIFT_ON;
     else if (direction==NO)   
            WRITE_LCD_IR = ENTRY_MODE_SET|CURSOR_SHIFT_RIGHT|ENTIRE_SHIFT_OFF;
     delay_us(40);
}

void display_ON_OFF(unsigned char display, unsigned char cursor, unsigned char blink)
{
     if      (display==ON)  display = DISPLAY_ON;
     else if (display==OFF) display = DISPLAY_OFF;
     if      (cursor==ON)   cursor  = CURSOR_ON;
     else if (cursor==OFF)  cursor  = CURSOR_OFF;
     if      (blink ==ON)   blink   = BLINK_ON;
     else if (blink ==OFF)  blink   = BLINK_OFF;

     WRITE_LCD_IR = DISPLAY_ON_OFF_CONTROL|display|cursor|blink;
     delay_us(40);
}