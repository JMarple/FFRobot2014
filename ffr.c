#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "StepperMotor.h"
#include "StepperControl.h"
#include "USART.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;
//unsigned long milliseconds;
//void startTimer(void);

int main(void)
{
	setupStepperMotor();
	USART_init();
	
	setDirection(0, 0);
	straight(5000, 900, 3000, 100, 3000, 2000);	
	
	enableDrive(0, 0);
	turnOnTimers(0, 0);
	
	while(1==1)
	{
	}	
}
