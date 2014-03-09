#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "FFRModel/FFRModel_StepperMotor.h"
#include "FFRModel/FFRModel_StepperControl.h"

volatile unsigned long milliseconds;

void moveForward()
{
	//Go Forward
	setDirection(0, 0);
	
	//Move Robot
	straight(200, 0, 1000, 0, 1000, 1000);	
}

void rotateDegrees(float degrees)
{
	//Rotate
	if(degrees < 0)
	{
		setDirection(1, 0);
		degrees = -degrees;
	}
	else
	{
		setDirection(0, 0);
	}
	
	//Move Robot
	straight(degrees*40, 0, 1000, 0, 1000, 1000);
}

void initRobot()
{
	setupStepperMotor();
	startTimer();
	USART_init();

	turnOnTimers(1,1);
	setDirection(1, 1);
	enableDrive(1, 1);
}

void stopRobot()
{
	enableDrive(0, 0);
	turnOnTimers(0, 0);
	while(1==1)
	{
		
	}	
}


void startTimer()
{
	//Refresh Loop Timer1  
	TCCR1B = (1 << WGM12) | (1 << CS10);//Set CTC & prescalar to 1
	OCR1A = 16000;//Compare Ticks 
    TIMSK1 = (1 << OCIE1A);//Enable Timer Interrupts
	milliseconds = 0;
}

ISR(TIMER1_COMPA_vect)
{
	milliseconds++;	
}
