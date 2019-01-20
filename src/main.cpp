#include <Arduino.h>
#include "FreeRTOS.h"
#include "esp_system.h"
#include "HeaderFiles/CommonInterfaceDefines.h"

#define RSB_THRESHOLD 3800
#define DATA_UPLOAD_DELAY 0


CommonInterface CI;

//setting up wifi connections
WebConfig *webConf = new WebConfig("Kinetic Analysis", "password");


void sendTask(void* pvParameters);



void setup() {
  Serial.begin(9600);

//adding all sensors
  CI.addSensor(new GSR("GSR", GSR_PIN));
  CI.addSensor(new ResistiveStretchBand("RSB1", RSB_PIN, MUX_PIN, 1));
  CI.addSensor(new ResistiveStretchBand("RSB2", RSB_PIN, MUX_PIN, 0));
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

  //*Start web configuration and store acquired connection in Common Interface
  WifiConnection *wifiConn = webConf->startAP();
  CI.addConnection(wifiConn);

  Serial.print("Connected to ");Serial.println(CI.getConnection()->getSsid());
  CI.getConnection()->connect();

  //create and start FreeRTOS task for sending the data
  xTaskCreate(sendTask, "wifi_task", CONFIG_MAIN_TASK_STACK_SIZE, NULL, 0, NULL);
  //  */
}

void sendTask(void* pvParameters) {
  while(true) {
    if(WiFi.status() == WL_CONNECTED) {
      Serial.println(CI.stretchQueue.size());
      CI.getConnection()->sendAllSensorData("stretch");
      CI.getConnection()->sendAllSensorData("capacitive");
      CI.getConnection()->sendAllSensorData("gsr");
      CI.getConnection()->sendAllSensorData("imu");
    }
  }
}

void loop() {

  //* reading and writing sensordata to the queue
  Serial.print("RSB1: ");Serial.println(CI.getSensor("RSB1")->getValue());
  Serial.print("RSB2: ");Serial.println(CI.getSensor("RSB2")->getValue());
  Serial.print("IMU1: ");Serial.println(CI.getSensor("IMU1")->getValue());
  Serial.print("IMU2: ");Serial.println(CI.getSensor("IMU2")->getValue());
  Serial.print("CSB1: ");Serial.println(CI.getSensor("CSB1")->getValue());
  Serial.print("CSB2: ");Serial.println(CI.getSensor("CSB2")->getValue());
  Serial.print("GSR: ");Serial.println(CI.getSensor("GSR")->getValue());
  delay(100);
  //  */
}
