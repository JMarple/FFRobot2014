#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "StepperMotor.h"
#include "StepperControl.h"
#include "USART.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;
volatile unsigned long milliseconds;
void startTimer(void);

int main(void)
{
	setupStepperMotor();
	startTimer();
	USART_init();
	
	turnOnTimers(1,1);
	setDirection(1, 1);
	enableDrive(1, 1);
	
	straight(1000, 0, 1000, 0, 1000, 3000);
	
	for(int i = 0; i < 250; i++)
	{
		
		_delay_ms(1);
	}
	
	PORTD |= (1 << PD3);
	enableDrive(0, 0);
	
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
