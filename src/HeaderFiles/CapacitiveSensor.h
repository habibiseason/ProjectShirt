#ifndef CAPACITIVE_SENSOR_H
#define CAPACITIVE_SENSOR_H

#include "Sensor.h"


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