#include "HeaderFiles/I2C_sensor.h"



I2C_sensor::I2C_sensor(string n, uint8_t add) : Sensor(n), I2C_Add(add)
{
	initSensor();
}

void I2C_sensor::initSensor() {
	//put I2C init code here
	cout << "Initialising I2C for IMU with addres: " << I2C_Add << " --> " << this->getName() << endl;
}

