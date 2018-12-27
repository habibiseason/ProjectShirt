#include "HeaderFiles/CapacitiveSensor.h"


CapacitiveSensor::CapacitiveSensor(string n, int pin1, int pin2) : Sensor(n), analogPin1(pin1), analogPin2(pin2)
{
	initSensor();
}

void CapacitiveSensor::initSensor() {
	pinMode(analogPin1, INPUT);
	pinMode(analogPin2, INPUT);
	cout << "Initialsing capacitive sensor (pin " << analogPin1 << ", " << analogPin2 << ") --> " << this->getName() << endl;
}

int CapacitiveSensor::getValue() {
	//insert capacitive measuring code
	cout << "Capacitive data incomming...\n";
	return 5;
}

