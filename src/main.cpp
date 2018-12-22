#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "KineticAnalysis";
const char* password = "password";

WiFiServer server;
String header;

String externalWifiId = "";
String externalWifiPassword = "";
IPAddress ip;

void setup() {
  Serial.begin(9600);
  delay(100);
  
  server.begin(80);

  // Setup AP
  WiFi.softAP(ssid, password);
  ip = WiFi.softAPIP();
  server.begin();

  Serial.print("Wifi server started with ip ");
  Serial.print(ip.toString());
}

void loop() {
  delay(1000);

  Serial.println(ip.toString());
}