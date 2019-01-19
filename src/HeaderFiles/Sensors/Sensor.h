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

	void setDelay(int value) { updateDelay = value; }
	int getDelay() { return updateDelay; }
protected:
	int counter = 0;
	int updateDelay = 1000;
private:
	int value = 0;
	string name;
};

#endif // !SENSOR_H