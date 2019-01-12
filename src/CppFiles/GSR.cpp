#include "HeaderFiles/GSR.h"


GSR::GSR(string n, int pin): BasicAnalogSensor(n, pin)
{
}

int GSR::getValue(){
	int value = analogRead(getAnalogPin());
	cout << "GSR data incomming from pin " << getAnalogPin() << ": " << value << endl;
	return value;
}
