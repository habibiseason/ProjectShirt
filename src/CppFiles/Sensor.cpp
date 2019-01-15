#include "HeaderFiles/Sensor.h"



Sensor::Sensor(string n) : name(n)
{
}


string Sensor::getName() {
	return name;
}