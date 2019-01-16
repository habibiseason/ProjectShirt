#include "HeaderFiles/WebConfig.h"

WebConfig::WebConfig(char* id, char* pass): ssid(id), password(pass){
    
    int passLength = static_cast<int>(strlen(password));
    if(passLength < 8){
        password = DEFAULT_PASSWORD;
        cout<< "Password was too short (<8), password set to default" << endl;
    }
    
    externalWifiId ="";
    externalWifiPassword = "";
}

WifiConnection* WebConfig::startAP(){
    //set ssid and password (password must be at least 8 characters)
    WiFi.softAP(ssid, password);

    //set ip address & mask
    IPAddress Ip(AP_IP);
    IPAddress NMask(AP_MASK);
    WiFi.softAPConfig(Ip, Ip, NMask);

    ip = WiFi.softAPIP();
    server.begin();

    Serial.print("Starting AP on ip: ");
    Serial.println(ip);

    //try connecting to configured access point
    waitForInput();
    return WifiConn;
}

void WebConfig::waitForInput(){
    while(1){
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
                WifiConn = new WifiConnection(externalWifiId, externalWifiPassword);
                //try connecting to configured access point 
                // if successfull -> break
                // else continue
                break;
            }
        }
    }
    cout << "Wifi settings succesfully configured!" << endl;
}

void WebConfig::displayWebpage(WiFiClient client) {
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

void WebConfig::handleResponse(WiFiClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    handleRequest(client);

    displayWebpage(client);
}

void WebConfig::handleRequest(WiFiClient client) {
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

bool WebConfig::isConfigured() {
  return !(externalWifiId == "" || externalWifiPassword == "");
}