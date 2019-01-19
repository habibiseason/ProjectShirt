#include "HeaderFiles/Sensors/ResistiveStretchBand.h"
#include "HeaderFiles/CommonInterface.h"


ResistiveStretchBand::ResistiveStretchBand(string n, int pin): BasicAnalogSensor(n, pin)
{
	initSensor();
}

int ResistiveStretchBand::getValue() {
	int value = analogRead(getAnalogPin());
	//cout << "Resisitve stretch band data incomming from pin " << getAnalogPin() << ": "<< value << endl;
	if((millis() - counter) > getDelay()) {
      counter = millis();
	  struct stretchband_struct strct = {
			.sensorId = (getName() == "RSB1") ? 1:2,
			.timestamp = millis(),
			.value = value,
	  };
	  CommonInterface::stretchQueue.push(strct);
	}

	return value;
}