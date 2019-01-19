#include "HeaderFiles/Sensors/GSR.h"
#include "HeaderFiles/CommonInterface.h"

GSR::GSR(string n, int pin): BasicAnalogSensor(n, pin)
{
}

int GSR::getValue(){
	int value = analogRead(getAnalogPin());
	//cout << "GSR data incomming from pin " << getAnalogPin() << ": " << value << endl;

	if((millis() - counter) > getDelay()) {
      counter = millis();

	  gsr_struct strct = {
		.timestamp = millis(),
		.value = value,
	  };
	  CommonInterface::gsrQueue.push(strct);
	}

	return value;
}
