#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>

#define EAP_USERNAME "" // Username
#define EAP_PASSWORD "" // Password
#define WEB_URL "http://145.52.157.191/wearableShirt_WebAPI/rest.php"

const char* ssid = "eduroam"; // your ssid
const bool useWpa = true;
HTTPClient http;

void connect();
void disconnect();

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println(ssid);
  
  connect();
}

void loop() {
  // Send get request
  if(WiFi.status() == WL_CONNECTED) {
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    long currentTime = millis();
    int code = http.POST("sensor_data=hey");
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