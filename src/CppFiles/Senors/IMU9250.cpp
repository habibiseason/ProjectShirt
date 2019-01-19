#include "HeaderFiles/Sensors/IMU9250.h"
#include "HeaderFiles/CommonInterface.h"

IMU9250::IMU9250(string n, uint8_t add): I2C_sensor(n, add), i2cAdd(add)
{
  cout<<"Initializing IMU with address: " << (int)i2cAdd << endl;
	initSensor();
}

void IMU9250::initSensor(){
	Wire.begin(); // join i2c bus (address optional for master)
  	Wire.beginTransmission(i2cAdd);
  	Wire.write(0x6B);  // PWR_MGMT_1 register
  	Wire.write(0);     // set to zero (wakes up the MPU-6050)
  	Wire.endTransmission();
}

int IMU9250::getValue() {
  int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1;
  uint8_t i = 104;
  Wire.beginTransmission(i2cAdd);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission();
  
  Wire.requestFrom(i2cAdd, 14, true); // request a total of 14 registers
    AcX1 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY1 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ1 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp1 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX1 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY1 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ1 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //cout<<getName()<< ":"<<endl;
  //Serial.print("GyX = "); Serial.print(GyX1);
  //Serial.print("\tGyY = "); Serial.print(GyY1);
  //Serial.print("\tGyZ = "); Serial.println(GyZ1);
  
  if((millis() - counter) > getDelay()) {
      counter = millis();
      struct imu_struct strct = {
        .sensorId = (getName() == "IMU1") ? 1 : 2,
        .timestamp = millis(),
        .x = GyX1,
        .y = GyY1,
        .z = GyZ1,
      };

      CommonInterface::imuQueue.push(strct);
  }
	
	return 5;
}

