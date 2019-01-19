#include <Arduino.h>
#include "FreeRTOS.h"
#include "esp_system.h"
#include "HeaderFiles/CommonInterfaceDefines.h"

#define RSB_THRESHOLD 3800
#define DATA_UPLOAD_DELAY 0


//int RSB_value = 0;

CommonInterface CI;

//setting up wifi connections
WebConfig *webConf = new WebConfig("Shirt", "password");

void sendTask(void* pvParameters);

void setup() {
  Serial.begin(9600);

//adding all sensors
  CI.addSensor(new GSR("GSR", GSR_PIN));
  CI.addSensor(new ResistiveStretchBand("RSB1", RSB_PIN));
  CI.addSensor(new ResistiveStretchBand("RSB2", RSB_PIN));
  CI.addSensor(new CapacitiveSensor("CSB1", CSB1_PINA, CSB1_PINB));
  CI.addSensor(new CapacitiveSensor("CSB2", CSB2_PINA, CSB2_PINB));
  CI.addSensor(new IMU9250("IMU1", IMU1_ADD));
  CI.addSensor(new IMU9250("IMU2", IMU2_ADD));

  CI.getSensor("RSB1")->setDelay(100);
  CI.getSensor("RSB2")->setDelay(100);
  CI.getSensor("IMU1")->setDelay(200);
  CI.getSensor("IMU2")->setDelay(200);
  CI.getSensor("CSB1")->setDelay(200);
  CI.getSensor("CSB2")->setDelay(200);
  CI.getSensor("GSR")->setDelay(1000);

//adding all actuators
  CI.addActuator(new DigitalControlActuator("VibMotor1", MOTOR1_pin));
	CI.addActuator(new DigitalControlActuator("VibMotor2", MOTOR2_pin));
  CI.addActuator(new DigitalControlActuator("HearthBeatLED", HB_LED_PIN));

  CI.listAllSensors();
  CI.listAllActuators();
  
  Serial.println("\nStarting program\n");

  //Start web configuration and store acquired connection in Common Interface
  WifiConnection *conn = new WifiConnection("devolo-30d32d585e37", "PNESQLXOPRMOKHVJ");
  //WifiConnection *wifiConn = webConf->startAP();
  CI.addConnection(conn);

  Serial.print("Connected to ");Serial.println(CI.getConnection()->getSsid());
  CI.getConnection()->connect();

  xTaskCreate(sendTask, "wifi_task", CONFIG_MAIN_TASK_STACK_SIZE, NULL, 0, NULL);
}

void sendTask(void* pvParameters) {
  while(true) {
    Serial.println(CI.stretchQueue.size());
    CI.getConnection()->sendAllSensorData("stretch");
    CI.getConnection()->sendAllSensorData("capacitive");
    CI.getConnection()->sendAllSensorData("gsr");
    CI.getConnection()->sendAllSensorData("imu");
    vTaskDelay(DATA_UPLOAD_DELAY/portTICK_RATE_MS);
  }
}

void loop() {

  CI.getSensor("RSB1")->getValue();
  CI.getSensor("RSB2")->getValue();
  CI.getSensor("IMU1")->getValue();
  CI.getSensor("IMU2")->getValue();
  CI.getSensor("CSB1")->getValue();
  CI.getSensor("CSB2")->getValue();
  CI.getSensor("GSR")->getValue();

  delay(10);
  /*
  CI.getSensor("CSB1")->getValue();
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
