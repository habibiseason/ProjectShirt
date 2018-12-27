#include <Arduino.h>
<<<<<<< Updated upstream
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>

#define EAP_USERNAME "18129048@student.hhs.nl" // Username
#define EAP_PASSWORD "" // Password
#define WEB_URL "http://kinetic-data.dynu.net/values.php"

#define STRETCHBAND 32


const char* ssid = "eduroam"; // your ssid
const bool useWpa = true;
HTTPClient http;

const int numSamples = 20;
int buf[numSamples];
int bufferIndex = 0;

void connect();
void disconnect();
int readStretch();

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println(ssid);

  pinMode(STRETCHBAND, INPUT);
  
  connect();
}

void loop() {
  int val = readStretch();
  
  // Send get request
  if(WiFi.status() == WL_CONNECTED) {
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    long currentTime = millis();
    String body = "value="+String(val);
    int code = http.POST(body);
    if(code == 200) { // Request success
      String body = http.getString();
      Serial.println(body);
      Serial.println(millis() - currentTime);
      http.end();
      //disconnect();
    }
    delay(2000);
  } else {
    connect();
  }

  delay(500);
}

void connect() {
  WiFi.disconnect(true);
  if(useWpa) {
    WiFi.mode(WIFI_STA);
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);
    WiFi.begin(ssid);
  } else {
    WiFi.begin(ssid, EAP_PASSWORD);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting..");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void disconnect() {
  WiFi.disconnect(true);
  while(WiFi.status() == WL_CONNECTED) {}
  Serial.println("Disconnected");
  delay(1000);
}

int readStretch() {
    int val = analogRead(STRETCHBAND);
    buf[bufferIndex] = val;
    if(++bufferIndex >= numSamples) bufferIndex = 0;

    float output = 0;
    for(int i=0; i < numSamples; i++) {
    output += buf[i];
    }
    output /= numSamples;

    return output;
=======
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
>>>>>>> Stashed changes
}