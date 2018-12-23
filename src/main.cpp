#include <Arduino.h>
#include <WiFi.h>

#define LED 2

const char* ssid = "KineticAnalysis";
const char* password = "password";

WiFiServer server;
String request;
String requestLine;

String externalWifiId = "";
String externalWifiPassword = "";
IPAddress ip;

bool isConfigured();
void handleResponse(WiFiClient client);
void displayWebpage(WiFiClient client);
void handleRequest(WiFiClient client);
String filterChar(String data);
void blink();

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(LED, OUTPUT);
  
  server.begin(80);

  // Setup AP
  WiFi.softAP(ssid, password);
  ip = WiFi.softAPIP();
  server.begin();

  Serial.print("Wifi server started with ip ");
  Serial.print(ip.toString());
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
    Serial.println("Connecting");
  }
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

void blink() {
  digitalWrite(LED, HIGH);
  delay(400);
  digitalWrite(LED, LOW);
  delay(400);
}