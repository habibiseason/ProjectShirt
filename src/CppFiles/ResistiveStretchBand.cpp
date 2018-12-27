#include "HeaderFiles/ResistiveStretchBand.h"



ResistiveStretchBand::ResistiveStretchBand(string n, int pin): BasicAnalogSensor(n, pin)
{
	initSensor();
}

int ResistiveStretchBand::getValue() {
	//analogRead(getAnalogPin());
	cout << "Resisitve stretch band data incomming from pin " << getAnalogPin() << endl;
	return 5;
}