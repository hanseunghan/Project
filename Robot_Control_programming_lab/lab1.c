

#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED0  0x01

volatile unsigned char reservation_flag = 0x00;
volatile unsigned char up_down_flag = 0;	// 정지 : 0, up : 1, down : 2
volatile unsigned long timer0, count;

volatile unsigned char now_position = 0x01;

#define PRESCALE		256L
#define PULSE_PER_OVERFLOW	256L
#define MS_OVERFLOW_CYCLE	((double)(PULSE_PER_OVERFLOW) * PRESCALE / (double)((double)F_CPU/1000.0))


void check_reservation_flag();
void up_down_flag_func();
void count_func();

ISR(TIMER0_OVF_vect)
{
	// timer/counter0 overflow interrupt는 분주비를 256으로 했으므로, 16MHz / 256*256
	// 256 : 8bit 범위
	// 256 : freescale 범위
	timer0++;

	// 여기에 스위치 입력을 받아 reservation_flag 값을 바꿔주는 함수를 넣어준다.
	check_reservation_flag();
	
	// 여기에 up_down_flag를 조정하는 함수를 넣어준다.
	up_down_flag_func();

	// 여기에 now_position 값과 reservation_flag의 특정비트와 같은지 아닌지 비교하여 같으면 count를 3까지 세게 하고, 다르면 count를 1만 세게 한다.
	count_func();
	

}

void check_reservation_flag()
{
	int i;

	for(i=0; i<=7; i++)
	{
		if(PIND & (0x01>>i))
		{
			reservation_flag = reservation_flag | (0x01 << i);
		}
	}
}

void up_down_flag_func()
{
	int i, cnt, num1, num2;
	int now_position_bit;

	cnt = 0;
	now_position_bit = 0;
	num1 = 0;
	num2 = 0;

	
	for(i=0; i<=7; i++)
	{
		if(reservation_flag & (0x01 >> i))
		{
			cnt++;
		}
	}

	if(cnt == 1)
	{
		if(now_position > reservation_flag)
		{
			up_down_flag = 2;
		}
		else if(now_position < reservation_flag)
		{
			up_down_flag = 1;
		}
		else if(now_position == reservation_flag)
		{
			up_down_flag = 0;
		}
		else
		{
			up_down_flag = 0;
		}
	}
	else if(cnt >=2)
	{
		if(up_down_flag == 2)
		{
	 		// 해당 층 검색.
			for(i=0; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					now_position_bit = i;
				}
			}

			// 해당 층보다 아래에  reservation_flag bit가 존재하는지 확인
			
			for(i=0; i<now_position_bit; i++)
			{
				if(now_position & (0x1 << i))
				{
					num1 = i;
				}
			}

			// 해당 층보다 위에  reservation_flag bit가 존재하는지 확인
			
			for(i=now_position_bit+1; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					num2 = i;
				}
			}

			if(num1 != 0)
			{
				up_down_flag = 2;
			}
			else if(num2 != 0)
			{
				up_down_flag = 1; 
			}
			else
			{
				up_down_flag = 0;
			}
		}

		else if(up_down_flag == 1)
		{
			// 해당 층 검색.
			for(i=0; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					now_position_bit = i;
				}
			}

			// 해당 층보다 아래에  reservation_flag bit가 존재하는지 확인
			
			for(i=0; i<now_position_bit; i++)
			{
				if(now_position & (0x1 << i))
				{
					num1 = i;
				}
			}

			// 해당 층보다 위에  reservation_flag bit가 존재하는지 확인
			
			for(i=now_position_bit+1; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					num2 = i;
				}
			}

			if(num1 != 0)
			{
				up_down_flag = 1;
			}
			else if(num2 != 0)
			{
				up_down_flag = 2; 
			}
			else
			{
				up_down_flag = 0;
			}

		}

	}
	else	// 예약이 없음 => up_down_flag = 0;
	{
		up_down_flag = 0;
	}


	// 알고리즘 추가 필요
	
	
	
}

void count_func()
{

	if(reservation_flag != 0x00 && timer0 / 244 == 0)
	{
		count++;
	}	    
}

int main(void)
{
    unsigned char i, now_position_bit;

    DDRA=0xFF;
    DDRD=0x00;

    TIMSK |= 1<<TOIE0;			// Timer/Counter0 interrupt 활성화
    TCCR0 |= 1<<CS02 | 1<<CS01;		// freescaler (1,1,0) 256 분주

    PORTA=now_position;			// LED0 ON 초기값

    timer0 = 0;
    now_position_bit = 0;
    sei();

    while(1)
    {
	    if(reservation_flag != 0x00)	// reservation_flag가 있을시
	    {	    
				if((now_position & reservation_flag) == now_position)
				{	
// count값과 up_down_flag를 비교하여 LED의 점등점멸을 결과로 출력하게끔 한다.
// 해당 층에 3초 멈춰있은 후, 이동하게 되는 순간에 reservation_flag를 0로 변경해 주어야 한다.		    	
				    	if(count == 3 && up_down_flag == 1)	
				   	 {
						 // 해당 층 검색.
						for(i=0; i<=7; i++)
						{
							if(now_position & (0x1 << i))
							{
								now_position_bit = i;
							}
						}
						// 해당층을 지나갈 것이므로, flag 비트를 0로 clear
						reservation_flag &= ~(0x1 << now_position_bit);
						now_position = now_position << 1;
						PORTA = now_position;
						count = 0;
				    	}

				    	else if(count == 3 && up_down_flag == 2)
				    	{
						 // 해당 층 검색.
						for(i=0; i<=7; i++)
						{
							if(now_position & (0x1 << i))
							{
								now_position_bit = i;
							}
						}
						// 해당층을 지나갈 것이므로, flag 비트를 0로 clear
						reservation_flag &= ~(0x1 << now_position_bit);
						now_position = now_position >> 1;
						PORTA = now_position;
						count = 0;
				    	}

				    	else
				    	{
						PORTA = now_position;
				    	}
				}
				
				else if ((now_position & reservation_flag) == 0x00)
				{	
					// 현재 position에 flag가 없다.
// count값과 up_down_flag를 비교하여 LED의 점등점멸을 결과로 출력하게끔 한다.				
				
					if(count == 1 && up_down_flag == 1)	
					{
						now_position = now_position << 1;
						PORTA = now_position;
						count = 0;
					}
					else if(count == 1 && up_down_flag == 2)
					{
						now_position = now_position >> 1;
						PORTA = now_position;
						count = 0;
					}
					else
					{
						PORTA = now_position;
					}
				}
				
				else 
				{
					PORTA = now_position;
				}	 
	    }
	    else if(reservation_flag == 0x00)	// reservation_flag가 없을시
	    {
		    PORTA = now_position;

	    }
    }
    return 0;
}
