#ifndef BASIC_ANALOG_SENSOR_H
#define BASIC_ANALOG_SENSOR_H

#include "HeaderFiles/Sensors/Sensor.h"

class BasicAnalogSensor: public Sensor
{
public:
	BasicAnalogSensor(string n, int pin);
	virtual ~BasicAnalogSensor() {}

	virtual void initSensor();
	virtual int getValue();
	virtual int getAnalogPin();

private:
	int analogPin;
};

#endif // !BASIC_ANALOG_SENSOR_H
	
