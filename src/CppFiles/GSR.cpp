#include "HeaderFiles/GSR.h"


GSR::GSR(string n, int pin): BasicAnalogSensor(n, pin)
{
}

int GSR::getValue(){
	//analogRead(getAnalogPin());
	cout << "GSR data incomming from pin " << getAnalogPin() << endl;
	return 5;
}
