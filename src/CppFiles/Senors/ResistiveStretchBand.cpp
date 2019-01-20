#include "HeaderFiles/Sensors/ResistiveStretchBand.h"
#include "HeaderFiles/CommonInterface.h"


ResistiveStretchBand::ResistiveStretchBand(string n, int pin, int mPin, int mValue): BasicAnalogSensor(n, pin), muxPin(mPin), muxValue(mValue)
{
	pinMode(muxPin, OUTPUT);
	initSensor();
}

int ResistiveStretchBand::getValue() {
	digitalWrite(muxPin, muxValue);
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