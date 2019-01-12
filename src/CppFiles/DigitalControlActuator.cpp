#include "HeaderFiles/DigitalControlActuator.h"



DigitalControlActuator::DigitalControlActuator(string n, int p): Actuator(n), pin(p)
{
	initActuator();
}

void DigitalControlActuator::initActuator() {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	cout << "Initializing digital control actuator (" << pin << ") --> " << this->getName() << endl;
}

void DigitalControlActuator::power(powerOptions p) {
	switch (p) {
	case ON:
		digitalWrite(pin, HIGH);
		cout << "Powering "<< this->getName() <<" on\n";
		break;

	case OFF:
		digitalWrite(pin, LOW);
		cout << "Powering "<<this->getName() <<" off\n";
		break;

	case TOGGLE:
		digitalWrite(pin, !digitalRead(pin));
		cout << "Toggling "<< this->getName() <<" power\n";
		break;
	}
}

int DigitalControlActuator::getStatus() {
	int status = digitalRead(pin);
	return status;
}