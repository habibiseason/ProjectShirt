#include "HeaderFiles/CommonInterface.h"

queue<stretchband_struct> CommonInterface::stretchQueue;
queue<capacitive_struct> CommonInterface::capacitiveQueue;
queue<gsr_struct> CommonInterface::gsrQueue;
queue<imu_struct> CommonInterface::imuQueue;

CommonInterface::CommonInterface()
{
}

//------------------------Sensors-------------------------->>
void CommonInterface::addSensor(Sensor* s) {
	sensors.push_back(s);
}

void CommonInterface::removeSensor(string sensorName) {

	for (it_sens = sensors.begin(); it_sens != sensors.end(); it_sens++) {
		if ((*it_sens)->getName() == sensorName) {
			sensors.remove((*it_sens));
			return;
		}
	}
	cout << "Couldn't delete sensor '" << sensorName << "' (sensor not found)\n";
	
}

Sensor* CommonInterface::getSensor(string sensorName) {
	for (it_sens = sensors.begin(); it_sens != sensors.end(); it_sens++) {
		if ((*it_sens)->getName() == sensorName) {
			return (*it_sens);
		}
	}

	cout << "Couldn't find sensor '" << sensorName << endl;
	return NULL;
}
void CommonInterface::listAllSensors() {
	cout<< "Listing all sensors:" << endl;
	for (it_sens = sensors.begin(); it_sens != sensors.end(); it_sens++) {
		cout<<(*it_sens)->getName()<<endl;
	}
}

//------------------------Actuators-------------------------->>

void CommonInterface::addActuator(Actuator* a) {
	actuators.push_back(a);
}

void CommonInterface::removeActuator(string actuatorName) {
	for (it_act = actuators.begin(); it_act != actuators.end(); it_act++) {
		if ((*it_act)->getName() == actuatorName) {
			actuators.remove((*it_act));
			return;
		}
	}
	cout << "Couldn't delete actuator named '" << actuatorName << "' (actuator not found)\n";
}

Actuator* CommonInterface::getActuator(string actuatorName) {
	for (it_act = actuators.begin(); it_act != actuators.end(); it_act++) {
		if ((*it_act)->getName() == actuatorName) {
			return (*it_act);
		}
	}
	cout << "Couldn't find actuator named '" << actuatorName << endl;
	return NULL;
}

void CommonInterface::listAllActuators() {
	cout<< "Listing all actuators:" << endl;
	for (it_act = actuators.begin(); it_act != actuators.end(); it_act++) {
		cout << (*it_act)->getName() << endl;
	}
}


//------------------------Wifi connection-------------------->>

void CommonInterface::addConnection(WifiConnection* conn){
	connection = conn;
}

void CommonInterface::removeConnection(){
	connection = NULL;
}

WifiConnection* CommonInterface::getConnection(){
	return connection;
}