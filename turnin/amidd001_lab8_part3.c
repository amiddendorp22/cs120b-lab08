/*	Author: amidd001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *	Video Demo: https://youtu.be/xlnqXcIKPXE
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

enum Melody_States {Mel_Init, Mel_1, Mel_2, Mel_3, Mel_4, Mel_5, Mel_6, Mel_7, Mel_8, Mel_9, Mel_10} MELODY_STATES;

void TickFct_Melody()
{
	unsigned char tmpA = ~PINA & 0x01;
	switch(MELODY_STATES)
	{
		case(Mel_Init):
			if(tmpA == 0x01)
			{
				MELODY_STATES = Mel_1;
			}
			else
			{
				MELODY_STATES = Mel_Init;
			}
			break;
		case(Mel_1):
			MELODY_STATES = Mel_2;
			break;
		case(Mel_2):
                        MELODY_STATES = Mel_3;
                        break;
		case(Mel_3):
                        MELODY_STATES = Mel_4;
                        break;
		case(Mel_4):
                        MELODY_STATES = Mel_5;
                        break;
		case(Mel_5):
                        MELODY_STATES = Mel_6;
                        break;
		case(Mel_6):
                        MELODY_STATES = Mel_7;
                        break;
		case(Mel_7):
                        MELODY_STATES = Mel_8;
                        break;
		case(Mel_8):
                        MELODY_STATES = Mel_9;
                        break;
		case(Mel_9):
                        MELODY_STATES = Mel_10;
                        break;
		case(Mel_10):
                        MELODY_STATES = Mel_Init;
                        break;
		default:
			break;
	}

	switch(MELODY_STATES)
	{
		case(Mel_Init):
			set_PWM(0);
			break;
		case(Mel_1):
			set_PWM(261.63);
			break;
		case(Mel_2):
			set_PWM(349.23);
			break;
		case(Mel_3):
			set_PWM(293.66);
			break;
		case(Mel_4):
			set_PWM(329.63);
			break;
		case(Mel_5):
			set_PWM(392.00);
			break;
		case(Mel_6):
			set_PWM(523.25);
			break;
		case(Mel_7):
			set_PWM(440.00);
			break;
		case(Mel_8):
			set_PWM(493.88);
			break;
		case(Mel_9):
			set_PWM(261.63);
			break;
		case(Mel_10):
			set_PWM(523.25);
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; //PINA AS INPUT
	DDRB = 0x40; PORTB = 0x40; //PORTB AS SPEAKER OUTPUT
    /* Insert your solution below */
	PWM_on();
	MELODY_STATES = Mel_Init;
	TimerSet(500);
	TimerOn();
	unsigned char buttonHeld = 0;
    while (1) {
	unsigned char tmpA = ~PINA & 0x01;
	if(MELODY_STATES == Mel_Init)
	{
		if(buttonHeld == 0)
		{
			TickFct_Melody();
		}
	}
	else
	{
		TickFct_Melody();
	}
	if(tmpA == 0x01)
	{
		buttonHeld = 1;
	}
	if(tmpA == 0x00)
	{
		buttonHeld = 0;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
