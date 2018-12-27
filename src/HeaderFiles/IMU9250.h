#ifndef IMU9250_H
#define IMU9250_H

#include "I2C_sensor.h"
#include <Wire.h>


class IMU9250: public I2C_sensor
{
public:
	IMU9250(string n, uint8_t add);
	virtual ~IMU9250() {}

	virtual void initSensor();
	virtual int getValue();	

private:
	uint8_t i2cAdd;

};

#endif	//IMU9250_H