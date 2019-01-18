#ifndef I2C_SENSOR_H
#define I2C_SENSOR_H


#include "Sensor.h"

class I2C_sensor: public Sensor
{
public:
	I2C_sensor(string n, uint8_t add);
	virtual ~I2C_sensor() {}

	virtual void initSensor();	//is the same for every I2C device, so this could be one generic code
	virtual int getValue() = 0;	//this however differs for every I2C device and thus must be implemented for every derived class


private:
		uint8_t I2C_Add;
};

#endif // !I2C_SENSOR_H

