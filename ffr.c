#include <avr/io.h>
#include "FFRModel/FFRModel.h"

int main(void)
{
	initRobot();
	
	moveForward();
	
	stopRobot();	
}