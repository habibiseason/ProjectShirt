#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;
class Actuator
{
public:
	Actuator(string n);
	virtual ~Actuator() {}

	virtual string getName();
	virtual void initActuator() = 0;

private:
	string name;
};

#endif
