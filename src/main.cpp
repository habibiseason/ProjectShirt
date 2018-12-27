#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>
#include <Wire.h>

#define EAP_USERNAME "15057070@student.hhs.nl" // Username
#define WEB_URL "http://kinetic-data.dynu.net/values.php"
#define REGISTER_URL "http://kinetic-data.dynu.net/announce.php"

#define IMU1_ADD  0x68
#define IMU2_ADD  0x69

#define RESISTIVE_STRETCHBAND 39
#define GSR                   36

#define BLUE_LED  2
#define H_LED     23
#define MOTOR1    5
#define MOTOR2    18

#define THRESHOLD_MOTORS      3700

const char* ssid = "KineticAnalysis";
const char* password = "password";

const bool useWpa = false;
HTTPClient http;

const int numSamples = 20;
int buf[numSamples];
int bufferIndex = 0;

char macAddress[18] = {0};

WiFiServer server;
String request;
String requestLine;

String externalWifiId = "";
String externalWifiPassword = "";
IPAddress ip;


int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1,
        AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

int resisitveValue, gsrValue;

void connect();
void disconnect();

bool isConfigured();
void handleResponse(WiFiClient client);
void displayWebpage(WiFiClient client);
void handleRequest(WiFiClient client);
void blink();

String filterChar(String data);

void readSensors();
void sendAllSensorData();

void alertUser();
void stopAlertingUser();
void printIMU_data();
void wakeupIMU(byte address);
void readIMU1();
void readIMU2();
int readStretch();

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println(externalWifiId);

  //setup sensor/actuator ---->>
  pinMode(RESISTIVE_STRETCHBAND, INPUT);
  pinMode(GSR, INPUT);

  pinMode(BLUE_LED, OUTPUT);
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
  
  server.begin(80);

  // Setup AP
  WiFi.softAP(ssid, password);
  ip = WiFi.softAPIP();
  server.begin();

  Serial.print("Wifi server started with ip ");
  Serial.print(ip.toString());
}


void readSensors(){
    readIMU1();
    readIMU2();
    resisitveValue = readStretch();
    gsrValue = analogRead(GSR);
}

void sendAllSensorData(){
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

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
    String response = http.getString();
    Serial.println(response);

    http.end();
}

void loop() {
  if(!isConfigured()) {
    blink();

    WiFiClient client = server.available();
    if(client) {
      while(client.connected()) {
        if(client.available()) {
          char c = client.read();
          Serial.write(c);
          if(c=='\n') {
            if(requestLine.length() == 0) { 
              handleResponse(client);
              break;
            } else {
              requestLine = "";
            }
          } else if(c!='\r') {
            request += c;
            requestLine += c;
          }
        }
      }

      request = "";
      client.stop();

      if(isConfigured()) {
        WiFi.disconnect(true);
        server.stop();
        WiFi.softAPdisconnect();

        Serial.println(externalWifiId);
        Serial.println(externalWifiPassword);
      }
    }
  } else {
    // START SENSOR PROGRAM
    if(WiFi.status() == WL_CONNECTED) {
      //read and send all sensor data
      readSensors();
      sendAllSensorData();
      delay(2000);
    } else {
      connect();
    }
  }
}

void connect() {
  WiFi.disconnect(true);
  if(useWpa) {
    WiFi.mode(WIFI_STA);
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)externalWifiPassword.c_str(), strlen(externalWifiPassword.c_str()));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);
    WiFi.begin(externalWifiId.c_str());
  } else {
    WiFi.begin(externalWifiId.c_str(), externalWifiPassword.c_str());
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting..");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // // Register device
  // http.begin(REGISTER_URL);
  // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // String body = "mac="+String(macAddress);
  // int code = http.POST(body);
  // Serial.println(body);
  // String res = http.getString();
  // Serial.println(code);
  // Serial.println(res);
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

void handleResponse(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  handleRequest(client);

  displayWebpage(client);
}

void handleRequest(WiFiClient client) {
  if(request.indexOf("GET /save") >= 0) {
    String send_ssid;
    String send_password;

    for(int i=request.indexOf("ssid=")+5; i < request.length(); i++) {
      if(request[i] == '&') break;
      send_ssid += request[i];
    }
    for(int i=request.indexOf("password=")+9; i < request.length(); i++) {
      if(request[i] == ' ') break;
      send_password += request[i];
    }
    send_ssid.replace("%2C",",");
    send_ssid.replace("+", " ");
    externalWifiId = send_ssid;
    externalWifiPassword = send_password;
  }
}

bool isConfigured() {
  return !(externalWifiId == "" || externalWifiPassword == "");
}

void displayWebpage(WiFiClient client) {
  client.println("<html><head><title>Kinetic Analysis Config</title></head>");
  client.println("<body>");
  client.println("<h3>Wearable sensor shirt Configuration</h3>");
  client.println("<form action='/save' method='GET'>");
  client.println("<input type='text' name='ssid' placeholder='ssid' /> <br/>");
  client.println("<input type='password' name='password' placeholder='password' /> <br/>");
  client.println("<input type='submit' value='Save' />");
  client.println("</form>");
  client.println("</body></html>");
}

int readStretch() {
    int val = analogRead(RESISTIVE_STRETCHBAND);
    buf[bufferIndex] = val;
    if(++bufferIndex >= numSamples) bufferIndex = 0;

    float output = 0;
    for(int i=0; i < numSamples; i++) {
    output += buf[i];
    }
    output /= numSamples;

    return output;
}

void blink() {
  digitalWrite(BLUE_LED, HIGH);
  delay(400);
  digitalWrite(BLUE_LED, LOW);
  delay(400);
}