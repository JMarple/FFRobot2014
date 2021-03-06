#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "FFRModel_sensors.h"

/******************************************
 * Setup ADC
 * 
 * Preset Registers for Analog to Digital
 * conversion
 ******************************************/
void setupADC()
{
	ADMUX = (1 << REFS0);// | (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
	ADCSRA = (1 << ADEN) | (ADPS0) | (1 << ADPS1) | (1 << ADPS2);	
	
	DDRD |= (1 << PORTD4) | (1 << PORTD1) | (1 << PORTD0);
}

/******************************************
 * Get IR Sensor Value
 * 
 * Turns on Emitter, waits, fetches an ADC 
 * value and returns a value
 ******************************************/
int getIRSensorValue(int analogChannel)
{	
	return ReadADC(analogChannel);	
}

//Copied from http://extremeelectronics.co.in/avr-tutorials/using-the-analog-to-digital-converter/
uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX= (1 << REFS0) | ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);
	
   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   ADCSRA|=(1<<ADIF);

   return(ADC);
}
//End Copy
