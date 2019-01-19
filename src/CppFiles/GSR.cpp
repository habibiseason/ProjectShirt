#include "HeaderFiles/GSR.h"
#include "HeaderFiles/CommonInterface.h"

GSR::GSR(string n, int pin): BasicAnalogSensor(n, pin)
{
}

int GSR::getValue(){
	int value = analogRead(getAnalogPin());
	//cout << "GSR data incomming from pin " << getAnalogPin() << ": " << value << endl;
	gsr_struct strct = {
		.timestamp = millis(),
		.value = value,
	};
	CommonInterface::gsrQueue.push(strct);

	return value;
}
