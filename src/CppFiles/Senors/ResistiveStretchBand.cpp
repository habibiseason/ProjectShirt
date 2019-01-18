#include "HeaderFiles/Sensors/ResistiveStretchBand.h"



ResistiveStretchBand::ResistiveStretchBand(string n, int pin): BasicAnalogSensor(n, pin)
{
	initSensor();
}

int ResistiveStretchBand::getValue() {
	int value = analogRead(getAnalogPin());
	cout << "Resisitve stretch band data incomming from pin " << getAnalogPin() << ": "<< value << endl;
	return value;
}