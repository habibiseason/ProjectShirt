#include "HeaderFiles/Actuators/Actuator.h"

Actuator::Actuator(string n): name(n)
{
}

string Actuator::getName() {
	return name;
}
