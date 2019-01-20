#ifndef CAPACITIVE_SENSOR_H
#define CAPACITIVE_SENSOR_H

#include "HeaderFiles/Sensors/Sensor.h"

#define CAPACITANCE_MULTIPLIER  100	//multiplier needed because change in capacitance is very small


class CapacitiveSensor: public Sensor
{
public:
	CapacitiveSensor(string n, int pin1, int pin2);
	virtual ~CapacitiveSensor() {}

	virtual void initSensor();
	virtual int getValue();

private:
	int analogPin1, analogPin2;
};

#endif