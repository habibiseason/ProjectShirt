#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>
#include <Wire.h>

#define EAP_USERNAME "15057070@student.hhs.nl" // Username
#define EAP_PASSWORD "zrySIhGi" // Password
#define WEB_URL "http://kinetic-data.dynu.net/values.php"
#define REGISTER_URL "http://kinetic-data.dynu.net/announce.php"

#define IMU1_ADD  0x68
#define IMU2_ADD  0x69

#define RESISTIVE_STRETCHBAND 39
#define GSR                   36

#define H_LED     23
#define MOTOR1    5
#define MOTOR2    18

#define THRESHOLD_MOTORS      3700



const char* ssid = "eduroam"; // your ssid
const bool useWpa = true;
HTTPClient http;

const int numSamples = 20;
int buf[numSamples];
int bufferIndex = 0;

const int stretchbandValue = 5;

char macAddress[18] = {0};



int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1,
        AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

int resisitveValue, gsrValue;

void connect();
void disconnect();

void readSensors();
void sendAllSensorData();

void alertUser();
void stopAlertingUser();
void printIMU_data();
void wakeupIMU(byte address);
void readIMU1();
void readIMU2();



void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println(ssid);

  //setup sensor/actuator ---->>
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
  //setup sensor/actuator ----<<

  uint8_t baseMac[8];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  connect();
}


void readSensors(){
    readIMU1();
    readIMU2();
    resisitveValue = analogRead(RESISTIVE_STRETCHBAND);
    gsrValue = analogRead(GSR);
}

void sendAllSensorData(){
    // Stretchband
    
    String body = "type=stretchband&timestamp="+String(millis())+"&value=" + String(resisitveValue);
    int code = http.POST(body);

    // IMU
    body = "type=IMU&imu_id=1&timestamp="+String(millis())+"&x=" + String(GyX1)+ "&y=" +String(GyY1)+ "&z=" + String(GyZ1);
    code = http.POST(body);
    body = "type=IMU&imu_id=2&timestamp="+String(millis())+"&x=" + String(GyX2)+ "&y=" +String(GyY2)+ "&z=" + String(GyZ2);
    code = http.POST(body);

    // Capacitive (with dummy values)
    body = "type=capacitive&capacitive_id=1&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);
    body = "type=capacitive&capacitive_id=2&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);

    // Gsr
    body = "type=gsr&timestamp="+String(millis())+"&value=" + String(gsrValue);
    code = http.POST(body);

    Serial.println(code);
    if(code == 200) { // Request success
      String body = http.getString();
      Serial.println(body);
      http.end();
    }
}

void loop() {



  
  // Send get request
  if(WiFi.status() == WL_CONNECTED) {
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    long currentTime = millis();
    
    //read and send all sensor data
    readSensors();
    sendAllSensorData();
    delay(2000);

  } else {
    connect();
  }
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

  // Register device
  http.begin(REGISTER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String body = "mac="+String(macAddress);
  int code = http.POST(body);
  Serial.println(body);
  String res = http.getString();
  Serial.println(code);
  Serial.println(res);
}

void disconnect() {
  WiFi.disconnect(true);
  while(WiFi.status() == WL_CONNECTED) {}
  Serial.println("Disconnected");
  delay(1000);
}

//-----------Sensor/actuator function------------>>
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