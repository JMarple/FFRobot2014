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
	//Set Acceleration
	setAccel(&leftMotor, inSpeed, maxSpeed, accel);
	setAccel(&rightMotor, inSpeed, maxSpeed, accel);
	
	turnOnTimers(1, 1);	
	enableDrive(1, 1);
	
	while(motorStillRunning(&leftMotor));
	
	//Calculate when to start decelerating
	float decelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
	
	while(leftMotor.stepCount + decelSteps < stepTarget);
	
	//Set Deceleration
	setDecel(&leftMotor, maxSpeed, exitSpeed, decel);
	setDecel(&rightMotor, maxSpeed, exitSpeed, decel);
	
	while(motorStillRunning(&leftMotor));
	turnOnTimers(0, 0);
		
}

void setAccel(struct StepperMotor *motor, int inSpeed, int maxSpeed, int accel)
{
	motor->targetDelay = TIMER_FREQUENCY / maxSpeed;	
	motor->tempCount = 0;
	motor->currentStepDelay = 0.676 * TIMER_FREQUENCY * sqrt(2.0 / (float)accel);

	for(int i = 0; i < (float)inSpeed * (float)inSpeed / (2.0 * (float)accel); i++)
	{
		motor->tempCount++;
		motor->currentStepDelay -= (float)(2.0 * motor->currentStepDelay) / (float)(4.0 * 	motor->tempCount + 1);
	}	
	
}

void setDecel(struct StepperMotor *motor, int maxSpeed, int exitSpeed, int decel)
{
	if(exitSpeed < 10)
		motor->targetDelay = 7500;
	else
		motor->targetDelay = TIMER_FREQUENCY / exitSpeed;
		
	motor->stepAccel = 0;
	motor->tempCount = (float)maxSpeed * (float)maxSpeed / (2.0 * decel);
}

int motorStillRunning(struct StepperMotor *motor)
{	
	if(motor->stepAccel == 0)
		return 1;
	else
		return 0;
}

