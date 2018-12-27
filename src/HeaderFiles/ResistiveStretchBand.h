#ifndef RESISTIVE_STRETCH_BAND
#define RESISTIVE_STRETCH_BAND

#include "BasicAnalogSensor.h"

class ResistiveStretchBand: public BasicAnalogSensor
{
public:
	ResistiveStretchBand(string n, int pin);
	virtual ~ResistiveStretchBand() {}

	virtual int getValue();

private:

};

#endif // !RESISTIVE_STRETCH_BAND
