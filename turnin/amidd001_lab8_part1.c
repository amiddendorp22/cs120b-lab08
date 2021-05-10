/*	Author: amidd001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *	Demo Video: https://youtu.be/-DLqn8PmcOo
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


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

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; //PINA AS INPUT
	DDRB = 0x40; PORTB = 0x40; //PORTB AS SPEAKER OUTPUT
    /* Insert your solution below */
	PWM_on();
    while (1) {
	unsigned char tmpA = ~PINA & 0x07; // grabs PA0, PA1, PA2 (input buttons)
	if(tmpA == 0x01) // outputs C_4
	{
		set_PWM(261.63);
	}
	else if(tmpA == 0x02) //outputs D_4
	{
		set_PWM(293.66);
	}
	else if(tmpA == 0x04) // outputs E_4
	{
		set_PWM(329.63);
	}
	else //if no buttons or more than one button is pressed
	{
		set_PWM(0);
	}
    }
    return 1;
}
