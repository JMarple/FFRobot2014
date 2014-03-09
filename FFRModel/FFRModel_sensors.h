#ifndef SENSORS_H
#define SENSORS_H

//Function Prototypes
void setupADC(void);
uint16_t ReadADC(uint8_t ch);
int getIRSensorValue(int analogChannel);

#endif
