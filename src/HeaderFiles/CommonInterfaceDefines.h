#ifndef COMMON_INTERFACE_DEFINES_H
#define COMMON_INTERFACE_DEFINES_H


#include "HeaderFiles/CommonInterface.h"

#include "HeaderFiles/Sensors/BasicAnalogSensor.h"
#include "HeaderFiles/Sensors/CapacitiveSensor.h"
#include "HeaderFiles/Sensors/ResistiveStretchBand.h"
#include "HeaderFiles/Sensors/IMU9250.h"
#include "HeaderFiles/Sensors/GSR.h"

#include "HeaderFiles/Actuators/DigitalControlActuator.h"

#include "HeaderFiles/WiFi/WifiConnection.h"
#include "HeaderFiles/WiFi/WebConfig.h"
#include "HeaderFiles/WiFi/WPAconnection.h"

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


#endif // !COMMON_INTERFACE_DEFINES_H

