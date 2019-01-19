#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

#include "HeaderFiles/Sensors/Sensor.h"
#include "HeaderFiles/Actuators/Actuator.h"
#include "HeaderFiles/WiFi/WifiConnection.h"
#include <queue>
#include <list>

struct capacitive_struct {
	int sensorId;
	int timestamp;
	int value;
};

struct stretchband_struct {
	int sensorId;
	int timestamp;
	int value;
};

struct imu_struct {
	int sensorId;
	int timestamp;
	int x, y, z;
};

struct gsr_struct {
	int timestamp;
	int value;
};

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

	static queue<stretchband_struct> stretchQueue;
	static queue<capacitive_struct> capacitiveQueue;
	static queue<gsr_struct> gsrQueue;
	static queue<imu_struct> imuQueue;

private:
	list<Sensor*> sensors;
	list<Sensor*>::iterator it_sens;

	list<Actuator*> actuators;
	list<Actuator*>::iterator it_act;

	WifiConnection* connection;
};

#endif // !COMMON_INTERFACE_H