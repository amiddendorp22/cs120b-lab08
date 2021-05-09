/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}





void set_PWM(double frequency)
{
	static double current_frequency;
	if(frequency != current_frequency)
	{
		if(!frequency)
		{
			TCCR3B &= 0x08;
		}
		else
		{
			TCCR3B |= 0x03;
		}
	
		if(frequency < 0.954)
		{
			OCR3A = 0xFFFF;
		}
		else if(frequency > 31250)
		{
			OCR3A = 0x0000;
		}
		else
		{
			OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		}
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on()
{
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off()
{
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum Sound_States {Scale_1, Scale_2, Scale_3, Scale_4, Scale_5, Scale_6, Scale_7, Scale_8} SOUND_STATE;

void TickFct_Sound()
{
	unsigned char tmpA = ~PINA & 0x07;
	switch(SOUND_STATE)
	{
		case(Scale_1):
			if(tmpA == 0x02)
			{
				SOUND_STATE = Scale_2;
			}
			else if(tmpA == 0x04)
			{
				SOUND_STATE = Scale_1;
			}
			else
			{
				SOUND_STATE = Scale_1;
			}
			break;
		case(Scale_2):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_3;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_1;
                        }
                        else
                        {
                                SOUND_STATE = Scale_2;
                        }
                        break;
		case(Scale_3):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_4;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_2;
                        }
                        else
                        {
                                SOUND_STATE = Scale_3;
                        }
                        break;
		case(Scale_4):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_5;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_3;
                        }
                        else
                        {
                                SOUND_STATE = Scale_4;
                        }
                        break;
		case(Scale_5):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_6;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_4;
                        }
                        else
                        {
                                SOUND_STATE = Scale_5;
                        }
                        break;
		case(Scale_6):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_7;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_5;
                        }
                        else
                        {
                                SOUND_STATE = Scale_6;
                        }
                        break;
		case(Scale_7):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_8;
			}
			else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_6;
                        }
                        else
                        {
                                SOUND_STATE = Scale_7;
                        }
                        break;
		case(Scale_8):
                        if(tmpA == 0x02)
                        {
                                SOUND_STATE = Scale_8;
                        }
                        else if(tmpA == 0x04)
                        {
                                SOUND_STATE = Scale_7;
                        }
                        else
                        {
                                SOUND_STATE = Scale_8;
                        }
                        break;
		default:
			break;
	}

	switch(SOUND_STATE)
	{
		case(Scale_1):
			set_PWM(261.63);
			break;
		case(Scale_2):
                        set_PWM(293.66);
                        break;
		case(Scale_3):
                        set_PWM(329.63);
                        break;
		case(Scale_4):
                        set_PWM(349.23);
                        break;
		case(Scale_5):
                        set_PWM(392.00);
                        break;
		case(Scale_6):
                        set_PWM(440.00);
                        break;
		case(Scale_7):
                        set_PWM(493.88);
                        break;
		case(Scale_8):
                        set_PWM(523.25);
                        break;

	}
}

enum OFF_STATES {Off_State, On_State} OFF_STATE;

void TickFct_OnOff()
{
	unsigned char tmpA = ~PINA & 0x07;
	switch(OFF_STATE)
	{
		case(Off_State):
			if(tmpA == 0x01)
			{
				OFF_STATE = On_State;
			}
			else
			{
				OFF_STATE = Off_State;
			}
			break;
		case(On_State):
			if(tmpA == 0x01)
			{
				OFF_STATE = Off_State;
			}
			else
			{
				OFF_STATE = On_State;
			}
			break;
		default:
			break;
	}

	switch(OFF_STATE)
	{
		case(Off_State):
			set_PWM(0);
			break;
		case(On_State):
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; //PINA AS INPUT
	DDRB = 0x40; PORTB = 0x40; //PORTB AS SPEAKER OUTPUT
    /* Insert your solution below */
	PWM_on();
	OFF_STATE = Off_State;
	SOUND_STATE = Scale_1;
	TimerSet(200);
	TimerOn();
    while (1) {
	unsigned char tmpA = ~PINA & 0x07; // grabs PA0, PA1, PA2 (input buttons)
	TickFct_Sound();
	TickFct_OnOff();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
