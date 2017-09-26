//***exp701.c *** --- 16*2 LCD controller ST7066U --- 
#include <avr/io.h>

#define WRITE_LCD_IR (*(unsigned char *)0x7000)
#define WRITE_LCD_DR (*(unsigned char *)0x7001)

#define EXTERNAL_MEMORY_INTERFACE 	    0x80  // 1000 0000

/*
#define WAIT_STATE_SECTOR_LIMIT         0x00  // Lower=N/A,Upper=0x1100~0xFFFF 

#define LOWER_SECTOR_0_WAIT_STATE       0x00  // 0000 0000   SRL1,SRL0
#define LOWER_SECTOR_1_WAIT_STATE       0x04  // 0000 0100
#define LOWER_SECTOR_2_WAIT_STATE       0x08  // 0000 1000
#define LOWER_SECTOR_2_WAIT_STATE_1     0x0C  // 0000 1100

#define UPPER_SECTOR_0_WAIT_STATE_SRW11 0x00  // 0000 0000
#define UPPER_SECTOR_0_WAIT_STATE_SRW10 0x00  // 0000 0000

#define UPPER_SECTOR_1_WAIT_STATE_SRW11 0x00  // 0000 0000
#define UPPER_SECTOR_1_WAIT_STATE_SRW10 0x40  // 0100 0000

#define UPPER_SECTOR_2_WAIT_STATE_SRW11 0x02  // 0000 0010
#define UPPER_SECTOR_2_WAIT_STATE_SRW10 0x00  // 0000 0000

#define UPPER_SECTOR_2_WAIT_STATE_1_SRW11 0x02  // 0000 0010
#define UPPER_SECTOR_2_WAIT_STATE_1_SRW10 0x40  // 0100 0000

#define EXTERNAL_MEMORY_BUS_KEEPER_ENABLE  0x80
#define NONE_RELEASED_PORT_PINS            0x00
*/

#define FUNCTION_SET     0x20   // 0010 0000
#define _8_BIT_BUS       0x10   // 0001 0000 
#define _4_BIT_BUS       0x00   // 0000 0000
#define _2_LINE_DISPLAY  0x08   // 0000 1000
#define _1_LINE_DISPLAY  0x00   // 0000 0000
#define _5_11_FONT       0x04   // 0000 0100
#define _5_8_FONT        0x00   // 0010 0000

#define DISPLAY_ON_OFF_CONTROL 0x08
#define DISPLAY_ON             0x04
#define DISPLAY_OFF            0x00
#define CURSOR_ON              0x02
#define CURSOR_OFF             0x00
#define BLINK_ON               0x01
#define BLINK_OFF              0x00

#define DISPLAY_CLEAR    0x01

#define ENTRY_MODE_SET      0x04
#define CURSOR_SHIFT_RIGHT  0x02
#define CURSOR_SHIFT_LEFT   0x00
#define ENTIRE_SHIFT_ON     0x01
#define ENTIRE_SHIFT_OFF    0x00  

#define RIGHT	1
#define LEFT	2
#define NO	0
#define ON	1
#define OFF	2

void initialize_MCU();
void initialize_LCD();
void delay_us(unsigned char time_us);
void delay_ms(unsigned int  time_ms);

int  main(void)
{
     initialize_MCU();
     initialize_LCD();
                   
     while(1);
}

void initialize_MCU()
{
     MCUCR = EXTERNAL_MEMORY_INTERFACE;
}

void initialize_LCD()
{
     delay_ms(50);

     WRITE_LCD_IR = FUNCTION_SET|_8_BIT_BUS|_2_LINE_DISPLAY|_5_8_FONT;    // 0011 1000
     delay_us(40);
     WRITE_LCD_IR = FUNCTION_SET|_8_BIT_BUS|_2_LINE_DISPLAY|_5_8_FONT;
     delay_us(40);

     WRITE_LCD_IR = DISPLAY_ON_OFF_CONTROL|DISPLAY_ON|CURSOR_ON|BLINK_ON; // 0000 1111
     delay_us(40);
     WRITE_LCD_IR = DISPLAY_CLEAR;                                        // 0000 0001
     delay_ms(2);
     WRITE_LCD_IR = ENTRY_MODE_SET|CURSOR_SHIFT_RIGHT|ENTIRE_SHIFT_OFF;   // 0000 0110
     delay_us(40);  
}

void delay_us(unsigned char time_us)	// time delay for us 
{
     register unsigned char i;

     for(i=0; i<time_us; i++){	        // 4 cycle +
         asm volatile(" PUSH  R0 ");	// 2 cycle +
         asm volatile(" POP   R0 ");	// 2 cycle +
         asm volatile(" PUSH  R0 ");	// 2 cycle +
         asm volatile(" POP   R0 ");	// 2 cycle +
         asm volatile(" PUSH  R0 ");	// 2 cycle +
         asm volatile(" POP   R0 ");	// 2 cycle = 16 cycle = 1 us for 16MHz
     }  
}

void delay_ms(unsigned int time_ms)	// time delay for ms
{
     register unsigned int i;

     for(i=0; i<time_ms; i++){ 
         delay_us(250);
         delay_us(250);
         delay_us(250);
         delay_us(250);
     }
}

