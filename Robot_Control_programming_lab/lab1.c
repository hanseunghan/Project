

#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED0  0x01

volatile unsigned char reservation_flag = 0x00;
volatile unsigned char up_down_flag = 0;	// ���� : 0, up : 1, down : 2
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
	// timer/counter0 overflow interrupt�� ���ֺ� 256���� �����Ƿ�, 16MHz / 256*256
	// 256 : 8bit ����
	// 256 : freescale ����
	timer0++;

	// ���⿡ ����ġ �Է��� �޾� reservation_flag ���� �ٲ��ִ� �Լ��� �־��ش�.
	check_reservation_flag();
	
	// ���⿡ up_down_flag�� �����ϴ� �Լ��� �־��ش�.
	up_down_flag_func();

	// ���⿡ now_position ���� reservation_flag�� Ư����Ʈ�� ������ �ƴ��� ���Ͽ� ������ count�� 3���� ���� �ϰ�, �ٸ��� count�� 1�� ���� �Ѵ�.
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
	 		// �ش� �� �˻�.
			for(i=0; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					now_position_bit = i;
				}
			}

			// �ش� ������ �Ʒ���  reservation_flag bit�� �����ϴ��� Ȯ��
			
			for(i=0; i<now_position_bit; i++)
			{
				if(now_position & (0x1 << i))
				{
					num1 = i;
				}
			}

			// �ش� ������ ����  reservation_flag bit�� �����ϴ��� Ȯ��
			
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
			// �ش� �� �˻�.
			for(i=0; i<=7; i++)
			{
				if(now_position & (0x1 << i))
				{
					now_position_bit = i;
				}
			}

			// �ش� ������ �Ʒ���  reservation_flag bit�� �����ϴ��� Ȯ��
			
			for(i=0; i<now_position_bit; i++)
			{
				if(now_position & (0x1 << i))
				{
					num1 = i;
				}
			}

			// �ش� ������ ����  reservation_flag bit�� �����ϴ��� Ȯ��
			
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
	else	// ������ ���� => up_down_flag = 0;
	{
		up_down_flag = 0;
	}


	// �˰��� �߰� �ʿ�
	
	
	
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

    TIMSK |= 1<<TOIE0;			// Timer/Counter0 interrupt Ȱ��ȭ
    TCCR0 |= 1<<CS02 | 1<<CS01;		// freescaler (1,1,0) 256 ����

    PORTA=now_position;			// LED0 ON �ʱⰪ

    timer0 = 0;
    now_position_bit = 0;
    sei();

    while(1)
    {
	    if(reservation_flag != 0x00)	// reservation_flag�� ������
	    {	    
				if((now_position & reservation_flag) == now_position)
				{	
// count���� up_down_flag�� ���Ͽ� LED�� ���������� ����� ����ϰԲ� �Ѵ�.
// �ش� ���� 3�� �������� ��, �̵��ϰ� �Ǵ� ������ reservation_flag�� 0�� ������ �־�� �Ѵ�.		    	
				    	if(count == 3 && up_down_flag == 1)	
				   	 {
						 // �ش� �� �˻�.
						for(i=0; i<=7; i++)
						{
							if(now_position & (0x1 << i))
							{
								now_position_bit = i;
							}
						}
						// �ش����� ������ ���̹Ƿ�, flag ��Ʈ�� 0�� clear
						reservation_flag &= ~(0x1 << now_position_bit);
						now_position = now_position << 1;
						PORTA = now_position;
						count = 0;
				    	}

				    	else if(count == 3 && up_down_flag == 2)
				    	{
						 // �ش� �� �˻�.
						for(i=0; i<=7; i++)
						{
							if(now_position & (0x1 << i))
							{
								now_position_bit = i;
							}
						}
						// �ش����� ������ ���̹Ƿ�, flag ��Ʈ�� 0�� clear
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
					// ���� position�� flag�� ����.
// count���� up_down_flag�� ���Ͽ� LED�� ���������� ����� ����ϰԲ� �Ѵ�.				
				
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
	    else if(reservation_flag == 0x00)	// reservation_flag�� ������
	    {
		    PORTA = now_position;

	    }
    }
    return 0;
}
