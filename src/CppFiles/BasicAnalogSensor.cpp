#include "HeaderFiles/BasicAnalogSensor.h"



BasicAnalogSensor::BasicAnalogSensor(string n, int pin): Sensor(n), analogPin(pin)
{
	initSensor();
}

void BasicAnalogSensor::initSensor() {
	pinMode(analogPin, INPUT);
	cout << "Initialzing basic analog sensor (pin " << analogPin << ") --> " << this->getName() << endl;
}

int BasicAnalogSensor::getValue() {
	int value = analogRead(analogPin);
	cout << "Basic analog sensor data incomming...\n";
	return value;
}

int BasicAnalogSensor::getAnalogPin() {
	return analogPin;
}