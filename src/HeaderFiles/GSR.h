#ifndef GSR_H
#define GSR_H

#include "BasicAnalogSensor.h"

class GSR: public BasicAnalogSensor
{
public:
	GSR(string n, int pin);
	virtual ~GSR(){}

	virtual int getValue();

private:

};

#endif //GSR_H