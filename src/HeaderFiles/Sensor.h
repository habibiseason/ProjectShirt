#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;


class Sensor
{
public:
	Sensor(string n);
	virtual ~Sensor() {}

	virtual string getName();
	virtual void initSensor() = 0;
	virtual int getValue() = 0;

private:
	int value;
	string name;
};

#endif // !SENSOR_H