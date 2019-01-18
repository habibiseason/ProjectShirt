#include <Arduino.h>
#include "HeaderFiles/CommonInterfaceDefines.h"


//int RSB_value = 0;

CommonInterface CI;

//setting up wifi connections
WebConfig *webConf = new WebConfig("KineticAnalysis", "password");


void setup() {
  Serial.begin(9600);

//adding all sensors
  CI.addSensor(new GSR("GSR", GSR_PIN));
  CI.addSensor(new ResistiveStretchBand("RSB", RSB_PIN));
  CI.addSensor(new CapacitiveSensor("CSB1", CSB1_PINA, CSB1_PINB));
  CI.addSensor(new CapacitiveSensor("CSB2", CSB2_PINA, CSB2_PINB));
  CI.addSensor(new IMU9250("IMU1", IMU1_ADD));
  CI.addSensor(new IMU9250("IMU2", IMU2_ADD));

//adding all actuators
  CI.addActuator(new DigitalControlActuator("VibMotor1", MOTOR1_pin));
	CI.addActuator(new DigitalControlActuator("VibMotor2", MOTOR2_pin));
  CI.addActuator(new DigitalControlActuator("HearthBeatLED", HB_LED_PIN));

  CI.listAllSensors();
  CI.listAllActuators();
  
  Serial.println("\nStarting program\n");

//Start web configuration and store acquired connection in Common Interface
  CI.addConnection(webConf->startAP());
  Serial.print("Linked to ");Serial.println(CI.getConnection()->getSsid());
  CI.getConnection()->connect();

}

void loop() {


  //CI.getConnection()->sendAllSensorData();
  delay(1000);
  /*
  CI.getSensor("CSB1")->getValue();
  CI.getSensor("IMU1")->getValue();
  CI.getSensor("IMU2")->getValue();
  CI.getSensor("GSR")->getValue();
  RSB_value = CI.getSensor("RSB")->getValue();

  if (RSB_value >= RSB_THRESHOLD){
    ((DigitalControlActuator*)CI.getActuator("HearthBeatLED"))->power(ON);
    ((DigitalControlActuator*)CI.getActuator("VibMotor2"))->power(ON);
  }else{
    ((DigitalControlActuator*)CI.getActuator("HearthBeatLED"))->power(OFF);
    ((DigitalControlActuator*)CI.getActuator("VibMotor2"))->power(OFF);
  }*/
  
}
