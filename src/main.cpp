#include <Arduino.h>
#include "HeaderFiles/SensorsDefines.h"

#define GSR_PIN   21
#define RSB_PIN   22

#define CSB1_PINA 32 
#define CSB1_PINB 33

#define CSB2_PINA 34
#define CSB2_PINB 35

#define IMU1_ADD  0x68
#define IMU2_ADD  0x69

#define MOTOR1_pin  18
#define MOTOR2_pin  19

CommonInterface CI;


void readIMU1(){
  int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1;

  Wire.beginTransmission(IMU1_ADD);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  
  Wire.requestFrom(IMU1_ADD, 14, true); // request a total of 14 registers
    AcX1 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY1 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ1 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp1 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX1 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY1 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ1 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Serial.print("IMU1: ");
  Serial.print("GyX = "); Serial.print(GyX1);
  Serial.print("\tGyY = "); Serial.print(GyY1);
  Serial.print("\tGyZ = "); Serial.println(GyZ1);
}

void wakeupIMU(byte address){
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.beginTransmission(address);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);

//adding all sensors
  CI.addSensor(new GSR("GSR", GSR_PIN));
  CI.addSensor(new ResistiveStretchBand("RSB", RSB_PIN));
  CI.addSensor(new CapacitiveSensor("CSB1", CSB1_PINA, CSB1_PINB));
  CI.addSensor(new CapacitiveSensor("CSB2", CSB2_PINA, CSB2_PINB));
  CI.addSensor(new IMU9250("IMU1", 104));
  CI.addSensor(new IMU9250("IMU2", IMU2_ADD));

  CI.addActuator(new DigitalControlActuator("VibMotor1", MOTOR1_pin));
	CI.addActuator(new DigitalControlActuator("VibMotor2", MOTOR2_pin));

  CI.listAllSensors();
  CI.listAllActuators();

	((DigitalControlActuator*)CI.getActuator("VibMotor1"))->power(ON);
	((DigitalControlActuator*)CI.getActuator("VibMotor2"))->power(OFF);
	((DigitalControlActuator*)CI.getActuator("VibMotor2"))->power(TOGGLE);
  
  //wakeupIMU(IMU1_ADD);
  Serial.println("Starting program\n");
}

void loop() {
  //readIMU1();
  //printIMU_data();
  CI.getSensor("IMU2")->getValue();
  delay(1000);
}