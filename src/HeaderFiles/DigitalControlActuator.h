#ifndef DIGITAL_CONTROL_ACTUATOR_H
#define DIGITAL_CONTROL_ACTUATOR_H

#include "Actuator.h"

enum powerOptions {
	ON, OFF, TOGGLE
};

class DigitalControlActuator:public Actuator
{

public:
	DigitalControlActuator(string n, int p);
	virtual ~DigitalControlActuator(){}

	virtual void initActuator();


	virtual void power(powerOptions p);
	virtual int getStatus();

private:
	int pin;
};

#endif // !DIGITAL_CONTROL_ACTUATOR_H

