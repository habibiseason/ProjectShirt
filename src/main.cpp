#include <Arduino.h>
#include <Wire.h>

#define IMU1_ADD  0x68
#define IMU2_ADD  0x69

#define RESISTIVE_STRETCHBAND 39
#define GSR                   36

#define H_LED     23
#define MOTOR1    5
#define MOTOR2    18


#define THRESHOLD_MOTORS      3700


int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1,
        AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

int resisitveValue, gsrValue;


void setup() {
  Serial.begin(9600);
  
  pinMode(RESISTIVE_STRETCHBAND, INPUT);
  pinMode(GSR, INPUT);

  pinMode(H_LED, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  digitalWrite(H_LED, LOW);
  digitalWrite(MOTOR1, LOW);
  digitalWrite(MOTOR2, LOW);
  
  wakeupIMU(IMU1_ADD);
  wakeupIMU(IMU2_ADD);

  Serial.println("Starting program");
}

void loop() {
    readIMU1();
    readIMU2();
    
    resisitveValue = analogRead(RESISTIVE_STRETCHBAND);
    gsrValue = analogRead(GSR);
    
    printIMU_data();
    Serial.print("Resistive Stretchband: ");Serial.print(resisitveValue);
    Serial.print("\tGSR: ");Serial.println(gsrValue);
    if(resisitveValue >= THRESHOLD_MOTORS){
      alertUser();
    }else{
      stopAlertingUser();
    }
    delay(200);
}

void alertUser(){
  digitalWrite(MOTOR1, HIGH);
  digitalWrite(MOTOR2, HIGH);
  digitalWrite(H_LED, HIGH);
}

void stopAlertingUser(){
  digitalWrite(MOTOR1, LOW);
  digitalWrite(MOTOR2, LOW);
  digitalWrite(H_LED, LOW);
}

void printIMU_data(){
  Serial.print("IMU1: ");
  Serial.print("GyX = "); Serial.print(GyX1);
  Serial.print("\tGyY = "); Serial.print(GyY1);
  Serial.print("\tGyZ = "); Serial.println(GyZ1);

  Serial.print("IMU2: ");
  Serial.print("GyX = "); Serial.print(GyX2);
  Serial.print("\tGyY = "); Serial.print(GyY2);
  Serial.print("\tGyZ = "); Serial.println(GyZ2);
}

void wakeupIMU(byte address){
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.beginTransmission(address);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
}

void readIMU1(){
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
}

void readIMU2(){
  Wire.beginTransmission(IMU2_ADD);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  
  Wire.requestFrom(IMU2_ADD, 14, true); // request a total of 14 registers
    AcX2 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY2 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ2 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp2 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX2 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY2 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ2 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}