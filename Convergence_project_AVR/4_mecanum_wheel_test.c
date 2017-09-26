
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PRESSED 1
#define RELEASED 0
#define DEBOUNCE_MS 100

/*
#define Right_Front_Wheel_plus PB0 			// #define PB0
#define Right_Front_Wheel_minus PB1 			// #define PB1
#define Left_Front_Wheel_plus PB2 			// #define PB2
#define Left_Front_Wheel_minus PB3 			// #define PB3


#define Right_Back_Wheel_plus PB4 			// #define PB4
#define Right_Back_Wheel_minus PB5 			// #define PB5
#define Left_Back_Wheel_plus PB6 			// #define PB6
#define Left_Back_Wheel_minus PB7 			// #define PB7
*/

#define MOTOR_PORT PORTB 	// PORTB의 digital 출력값을 써주기 위한 매크로
#define MOTOR_DDR DDRB 		// PORTB의 입출력 설정을 위한 매크로
#define CLEAR (MOTOR_PORT & 0x00) // PORTB의 PIN4, PIN5, PIN6, PIN7을 무조건 0으로 만든다. 
/*
#define MOTOR_STOP (1<<Left_Back_Wheel_plus | 1<<Left_Back_Wheel_minus | 1<<Right_Back_Wheel_plus | 1<<Right_Back_Wheel_minus)	// 1111_0000
#define MOTOR_FORE (1<<Left_Back_Wheel_minus | 1<<Right_Back_Wheel_minus) 		// 1010_0000
#define MOTOR_BACK (1<<Left_Back_Wheel_plus | 1<<Right_Back_Wheel_plus) 		// 0101_0000
*/

#define MOTOR_STOP 0xFF
#define MOTOR_FORE 0x69
#define MOTOR_BACK 0x96
#define MOTOR_LEFT 0x55
#define MOTOR_RIGHT 0xAA
#define MOTOR_FORE_LEFT 0x7D
#define MOTOR_BACK_LEFT 0xD7
#define MOTOR_FORE_RIGHT 0xEB
#define MOTOR_BACK_RIGHT 0xBE


unsigned char digit[] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xB8, 0x80, 0xB0}; 		
														// 7segment display 값

void display_7segled(unsigned char led[], unsigned int number){

	PORTC = led[number];

}

int main(void)
{
	int number;
	int before;

	DDRC = 0xFF; 									
	DDRD = 0xFE; 			
	PORTD = PORTD | 1<<PD0;
	
	MOTOR_DDR |= 0xFF;				
	MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_STOP; 
	number = 0;
	before = RELEASED; 		// before = 0;

	while(1)
	{
		display_7segled(digit, number%10);
		
		if((before == RELEASED) && !(PIND & 1<<PD0)){ 
			number++;
			_delay_ms(DEBOUNCE_MS);
			before = PRESSED;
		}
		else if((before == PRESSED) && (PIND & 1<<PD0)){
			_delay_ms(DEBOUNCE_MS);
			before = RELEASED;
		}

		if(number % 10 == 1)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_FORE;
		else if(number % 10 == 2)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_BACK;
		else if(number % 10 == 3)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_LEFT;
		else if(number % 10 == 4)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_RIGHT;
		else if(number % 10 == 5)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_FORE_LEFT;
		else if(number % 10 == 6)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_BACK_LEFT;
		else if(number % 10 == 7)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_FORE_RIGHT;
		else if(number % 10 == 8)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_BACK_RIGHT;
		else if(number % 10 == 9)
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_FORE;
			
		else
			MOTOR_PORT = (MOTOR_PORT & CLEAR) | MOTOR_STOP;
	}
	return 0;
}
 
 
 





