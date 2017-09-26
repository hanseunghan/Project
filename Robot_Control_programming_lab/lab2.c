#define F_CPU		16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define CCW_ROTATION	0xA0
#define CW_ROTATION	0x60
#define QUICK_STOP	0x20
#define	FREE_STOP	0x00

#define PWM_MODE			0x00 
#define FAST_PWM_MODE			0x48
#define OUTPUT_LOW_ON_COMPARE_MATCH	0x20
#define PRESCALE_1024			0x07 // prescaler : 1024, freq : 15.625kHz, T : 64us
#define DUTYRATIO_MIN			0x10
#define DUTYRATIO_MAX			0xFF

#define PRESSED		1
#define RELEASED	0

void initialize_PORT();
void initialize_TIMER0_PWM();
void pulse_out(unsigned char duty_Ratio);
void quick_stop(unsigned char quick_stop, unsigned char duty_Ratio);

int main()
{
	unsigned char duty_Ratio = DUTYRATIO_MIN;
	
	unsigned char MAX_MIN = DUTYRATIO_MAX - DUTYRATIO_MIN;

	int number = 0;
	int before = RELEASED;

	

	initialize_PORT();
	initialize_TIMER0_PWM();

	PORTB = CCW_ROTATION;

	while(1)
	{
		// QUICK STOP MODE
		
		if(PIND & (1<<PD7))		//If PD7 SW is pushed  => QUICK_STOP
		{
			number = 0;
			_delay_ms(100);		// SW debouncing delay 0.1seconds
			PORTB = QUICK_STOP;
			duty_Ratio = DUTYRATIO_MAX;
			OCR0 = duty_Ratio;
			_delay_ms(100);		// Delay to provide time to run QUICK_STOP
		}
		
		
		// SW push -> Remove one's hand


		if(before == RELEASED && (PIND & 1<<PD0))	// If SW mode is Released and PD0 pin is HIGH
		{
			_delay_ms(100);							// SW debouncing delay 0.1seconds
			before = PRESSED;						// SW mode == PRESSED
		}
		else if(before == PRESSED && !(PIND & 1<<PD0)) // If SW mode is Pressed and PD0 pin is LOW
		{
			number++;								// number++
			_delay_ms(100);
			before = RELEASED;						// SW mode == RELEASED
		}
	

		// MOTOR_DRIVEN through number


		if(number % 8 == 1 || number % 8 == 2 || number % 8 == 3 || number % 8 == 4)
		{
			duty_Ratio = (MAX_MIN) * number / 4 + DUTYRATIO_MIN;
			pulse_out(duty_Ratio);
		}
		else if(number % 8 == 5 || number % 8 == 6 || number % 8 == 7)
		{
			duty_Ratio = DUTYRATIO_MAX + MAX_MIN - ((MAX_MIN) * number / 4);
			pulse_out(duty_Ratio);
		}
		else if(number % 8 == 0)
		{
			number = 0;
			duty_Ratio = DUTYRATIO_MIN;
			pulse_out(duty_Ratio);
		}
	


	}
	
	return 0;
}


void initialize_PORT()
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	
	DDRD = 0x7E; // 0111_1110, only input PD7, PD0 

	PORTA = 0x00;
	PORTB = 0x00;
}

void initialize_TIMER0_PWM()
{
	TCCR0 = PWM_MODE | FAST_PWM_MODE | OUTPUT_LOW_ON_COMPARE_MATCH | PRESCALE_1024;
	TCNT0 = 0x00;
}

void pulse_out(unsigned char duty_Ratio)
{
	PORTB = CCW_ROTATION;		// After PD7 SW is pressed, Operation by SW0 should be output as CCW_Rotation.
	OCR0 = duty_Ratio;
	PORTA = duty_Ratio;
}
