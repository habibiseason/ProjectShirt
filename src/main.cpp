#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>

#define EAP_USERNAME "18129048@student.hhs.nl" // Username
#define EAP_PASSWORD "" // Password
#define WEB_URL "http://kinetic-data.dynu.net/values.php"
#define REGISTER_URL "http://kinetic-data.dynu.net/announce.php"

#define STRETCHBAND 23

const char* ssid = "eduroam"; // your ssid
const bool useWpa = true;
HTTPClient http;

const int numSamples = 20;
int buf[numSamples];
int bufferIndex = 0;

const int stretchbandValue = 5;

char macAddress[18] = {0};

void connect();
void disconnect();
int readStretch();

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println(ssid);

  //pinMode(STRETCHBAND, INPUT);
  uint8_t baseMac[8];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  connect();
}

void loop() {
  //int val = readStretch();
  
  // Send get request
  if(WiFi.status() == WL_CONNECTED) {
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    long currentTime = millis();
    
    // Stretchband
    String body = "type=stretchband&timestamp="+String(millis())+"&value=34";
    int code = http.POST(body);

    // IMU
    body = "type=IMU&imu_id=1&timestamp="+String(millis())+"&x=3&y=2&z=9";
    code = http.POST(body);
    body = "type=IMU&imu_id=2&timestamp="+String(millis())+"&x=3&y=2&z=9";
    code = http.POST(body);

    // Capacitive
    body = "type=capacitive&capacitive_id=1&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);
    body = "type=capacitive&capacitive_id=2&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);

    // Gsr
    body = "type=gsr&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);
    body = "type=gsr&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);

    Serial.println(code);
    if(code == 200) { // Request success
      String body = http.getString();
      Serial.println(body);
      http.end();
    }
    delay(500);
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
}