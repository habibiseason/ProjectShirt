#ifndef SENSOR_DEFINES_H
#define SENSOR_DEFINES_H


#include "CommonInterface.h"

#include "BasicAnalogSensor.h"
#include "CapacitiveSensor.h"
#include "ResistiveStretchBand.h"
#include "IMU9250.h"
#include "GSR.h"

#include "DigitalControlActuator.h"

//pin defines
#define GSR_PIN   36
#define RSB_PIN   39

#define CSB1_PINA 32
#define CSB1_PINB 33

#define CSB2_PINA 34
#define CSB2_PINB 35

#define IMU1_ADD  0x68
#define IMU2_ADD  0x69

#define MOTOR1_pin  18
#define MOTOR2_pin  19

#define HB_LED_PIN  23


#endif // !SENSOR_DEFINES_H

