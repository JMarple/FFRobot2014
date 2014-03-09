 /*
 * Stepper Motor Controller - For Trinity Firefighting Robot and Micromouse	
 *
 * @author Justin Marple, Eli Siskind
 * @version Feb 3rd 2014
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "FFRModel_StepperMotor.h"



StepperMotor leftMotor;
StepperMotor rightMotor;
extern long unsigned microseconds;

/* LEFT MOTOR CONTROL */
ISR(TIMER0_COMPA_vect)
{
	//Set to max initially to prevent the next interrupt cycle from overwriting this one
	OCR0A = 255;
	
	//If ready to update again
	if(leftMotor.delayCounter == 0)
	{
		pulseMotor(&PORTD, PD2);
		leftMotor.stepCount++;		
		
		leftMotor.delayCounter = leftMotor.currentStepDelay;
	}
	
	//8 Bit Fix
	if(leftMotor.delayCounter <= 255)
	{
		OCR0A = leftMotor.delayCounter;
		leftMotor.delayCounter = 0;
	}
	else
	{
		if((int)leftMotor.delayCounter % 255 < 20)
		{
			OCR0A = 20;
			leftMotor.delayCounter -= 20;
		}
		else
		{
			OCR0A = 255;
			leftMotor.delayCounter -= 255;
		}
	}
}

/* RIGHT MOTOR CONTROL */
ISR(TIMER2_COMPA_vect)
{
	//Set to max initially to prevent the next interrupt cycle from overwriting this one
	OCR2A = 255;

	//If ready to update again
	if(rightMotor.delayCounter == 0)
	{
		//Pulse and increment motor
		pulseMotor(&PORTD, PD4);
		rightMotor.stepCount++;
		
		rightMotor.delayCounter = rightMotor.currentStepDelay;
	}
	
	//8 Bit Fix
	if(rightMotor.delayCounter <= 255)
	{
		OCR2A = rightMotor.delayCounter;
		rightMotor.delayCounter = 0;
	}
	else
	{
		if((int)rightMotor.delayCounter % 255 < 20)
		{
			OCR2A = 20;
			rightMotor.delayCounter -= 20;
		}
		else
		{
			OCR2A = 255;
			rightMotor.delayCounter -= 255;
		}
	}
}

void setupStepperMotor()
{
	//Motor Pins are D2 and D4 on arduino nano
	DDRD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4);
	DDRB |= (1 << PORTB1) | (1 << PORTB0);
	
	//Set to CTC
	TCCR0A = (1 << WGM01);
	TCCR2A = (1 << WGM21);
	
	//Compare Ticks
	OCR0A = 1;
	OCR2A = 1;
	
	//Enable Interrupts
	TIMSK0 = (1 << OCIE0A);
	TIMSK2 = (1 << OCIE2A);
	
	//Set Prescalar to 0
	OCR0B = 0;
	OCR2B = 0;
	
	//Enable Interrupts
	sei(); 
	
	enableDrive(0, 0);	
}

void enableDrive(int left, int right)
{
	if(left == 1)	
		PORTD &= ~(1 << PD6);	
	else if(right == 0)
		PORTD |= (1 << PD6);
	
	if(right == 1)	
		PORTD &= ~(1 << PD7);	
	else if(right == 0)	
		PORTD |= (1 << PD7);	
}

/* Pulse port and pin for 10 microseconds */
void pulseMotor(volatile uint8_t *port, uint8_t pin)
{
	//Send Pulse to Motor
	(*port) |= (1 << pin);
	_delay_us(5);
	(*port) &= ~(1 << pin);
}

void setDirection(int left, int right)
{
	if(left)
		PORTB &= ~(1 << PB0);
	else
		PORTB |= (1 << PB0);
		
	if(right)
		PORTB |= (1 << PB1);
	else
		PORTB &= ~(1 << PB1);
}


int eightBitTimerFix(struct StepperMotor *motor)
{
	
	int returnNum;

	if(motor->delayCounter <= 255)
	{
		returnNum = motor->delayCounter;
		motor->delayCounter = 0;	
	}
	else
	{
		motor->delayCounter -= 255;
		returnNum = 255;
	}
	return returnNum;
}

void turnOnTimers(int one, int two)
{
	if(one)
		TCCR0B |= (1 << CS01) | (1 << CS00);
	else
		TCCR0B &= ~((1 << CS01) | (1 << CS00));
		
	if(two)
		TCCR2B |= (1 << CS22);
	else	
		TCCR2B &= ~(1 << CS22);
}

int getDelayFromVelocity(int stepsPerSecond)
{
	if(stepsPerSecond < 10)
		return 25000;
	else
		return TIMER_FREQUENCY / stepsPerSecond;
}
