#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "StepperMotor.h"
#include "StepperControl.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{	
	//Motor settings
	leftMotor.stepCount 	= rightMotor.stepCount	= 0;
	
	//Motor's max speed converted to a delay
	leftMotor.targetDelay		= rightMotor.targetDelay 	= TIMER_FREQUENCY / maxSpeed;
	
	//Current STep Delay
	leftMotor.currentStepDelay = rightMotor.currentStepDelay = calculateInitialDelay(accel);	

	leftMotor.stepAccel = 0;
	
	turnOnTimers(1, 1);	
	enableDrive(1, 1);
	
	while(leftMotor.stepAccel == 0)
	{}
	
	float decelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
	
	while(leftMotor.stepCount + decelSteps < stepTarget)
	{
	}
	
	leftMotor.targetDelay = rightMotor.targetDelay = 7500;
	leftMotor.stepAccel = rightMotor.stepAccel = 0;
	leftMotor.tempCount = rightMotor.tempCount = (float)maxSpeed * (float)maxSpeed / (2.0 * decel);
	
	while(leftMotor.stepAccel == 0)
	{}
	
	printlnNum((float)leftMotor.stepCount);
	turnOnTimers(0, 0);
}

float calculateInitialDelay(float accel)
{
	return 0.676 * TIMER_FREQUENCY * sqrt(2.0 / (float)accel);	
}

