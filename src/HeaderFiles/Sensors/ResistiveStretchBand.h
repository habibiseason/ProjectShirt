#ifndef RESISTIVE_STRETCH_BAND
#define RESISTIVE_STRETCH_BAND

#include "BasicAnalogSensor.h"

class ResistiveStretchBand: public BasicAnalogSensor
{
public:
	ResistiveStretchBand(string n, int pin, int mPin, int mValue);
	virtual ~ResistiveStretchBand() {}

	virtual int getValue();

private:
int muxPin, muxValue;

};

#endif // !RESISTIVE_STRETCH_BAND
