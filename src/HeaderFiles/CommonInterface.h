#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

#include "Sensor.h"
#include "Actuator.h"
#include "WifiConnection.h"
#include <list>

class CommonInterface
{
public:
	CommonInterface();
	virtual ~CommonInterface() {}

	virtual void addSensor(Sensor* s);
	virtual void removeSensor(string sensorName);
	virtual Sensor* getSensor(string sensorName);
	virtual void listAllSensors();
	
	virtual void addActuator(Actuator* a);
	virtual void removeActuator(string actuatorName);
	virtual Actuator* getActuator(string actuatorName);
	virtual void listAllActuators();

	virtual void addConnection(WifiConnection* conn);
	virtual void removeConnection();
	virtual WifiConnection* getConnection();

private:
	list<Sensor*> sensors;
	list<Sensor*>::iterator it_sens;

	list<Actuator*> actuators;
	list<Actuator*>::iterator it_act;

	WifiConnection* connection;
};

#endif // !COMMON_INTERFACE_H