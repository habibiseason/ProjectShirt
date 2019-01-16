#include "HeaderFiles/WebConfig.h"

WebConfig::WebConfig(char* id, char* pass): ssid(id), password(pass), _connection(false){
    
    int passLength = static_cast<int>(strlen(password));
    if(passLength < 8){
        password = DEFAULT_PASSWORD;
        cout<< "Password was too short (<8), password set to default" << endl;
    }
    
    externalWifiId ="";
    externalWifiPassword = "";
    externalUsername = "";
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
    waitForConnection();
    return WifiConn;
}

void WebConfig::waitForConnection(){
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
                //disconnect AP
                WiFi.disconnect(true);
                server.stop();
                WiFi.softAPdisconnect();

                //print WiFi details
                Serial.println(externalWifiId);
                Serial.println(externalWifiPassword);
                Serial.println(externalUsername);

                //check for WPA or normal connection
                if (externalUsername.length() == 0){    //if username is not filled in -> start normal Wifi Connection
                    Serial.print("USername size = "); Serial.println(externalUsername.length());
                    WifiConn = new WifiConnection(externalWifiId, externalWifiPassword);
                }
                else{   //else start WPA Connection
                    Serial.print("USername size = "); Serial.println(externalUsername.length());
                    WifiConn = new WPAconnection(externalWifiId, externalWifiPassword, externalUsername);
                }
                break;
            }
        }
    }
    cout << "Wifi settings succesfully configured!" << endl;
}

void WebConfig::displayWebpage(WiFiClient client, bool connectionStat) {

    if (connectionStat){    //if device is trying to connect to the configured wifi point (connect button pressed)
        client.println("<html><head><title>Kinetic Analysis WifiConfig</title></head>");
        client.println("<body><center>");
        client.println("<h2>WiFi Configuration Wearable Sensor Shirt </h2>");
        client.println("<p>Trying to connect to <i>" + externalWifiId + "</i> <br>");
        client.println("Connection with <i>"+ (String)ssid + "</i> will be broken. <br><br>");
        client.println("<i>Device may take a minute to connect, if not try to reboot and try again.</i></p>");
        client.println("</body></html>");
    }
    else{   //default page
        client.println("<html><head><title>Kinetic Analysis WifiConfig</title></head>");
        client.println("<body><center>");
        client.println("<h2>WiFi Configuration Wearable Sensor Shirt </h2>");
        client.println("<p>Please, fill in the the WiFi credentials and press connect.<br>");
        client.println("<i>*Only fill in the Username field when trying to connect to a WPA connection</i></p>");
        client.println("<form action='/save' method='GET'>");
        client.println("<input type='text' name='ssid' placeholder='SSID' /> <br/>");
        client.println("<input type='password' name='password' placeholder='Password' /> <br/>");
        client.println("<input type='text' name='username' placeholder='Username (WPA)' /> <br/><br>");
        client.println("<input type='submit' value='Connect' />");
        client.println("</form>");
        client.println("</body></html>");
    }
}

void WebConfig::handleResponse(WiFiClient client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    handleRequest(client);

    displayWebpage(client, _connection);
}

void WebConfig::handleRequest(WiFiClient client) {
  if(request.indexOf("GET /save") >= 0) {
    String send_ssid, send_password, send_username;
    

    for(int i=request.indexOf("ssid=")+5; i < request.length(); i++) {
      if(request[i] == '&') break;
      send_ssid += request[i];
    }
    for(int i=request.indexOf("password=")+9; i < request.length(); i++) {
      if(request[i] == '&') break;
      send_password += request[i];
    }
    
    for(int i=request.indexOf("username=")+9; i < request.length(); i++) {
      if(request[i] == ' ') break;
      send_username += request[i];
    }

    externalWifiId = send_ssid;
    externalWifiPassword = send_password;
    externalUsername = send_username;

    //check for special characters
    replaceChar();

    //update AP screen
    _connection = true;
    displayWebpage(client, _connection);
    delay(50);
  }
}

void WebConfig::replaceChar(){
    const String specialChars[35] = {" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "\\", "]", "^", "_", "`", "{", "|", "}", "~"};
    const String specialChar_PE[35] = {"+", "%21", "%22", "%23", "%24", "%25", "%26", "%27", "%28", "%29", "%2A", "%2B", "%2C", "%2D", "%2E", "%2F", "%3A", "%3B", "%3C", "%3D", "%3E", "%3F", "%40", "%5B", "%5C", "%5D", "%5E", "%5F", "%60", "%7B", "%7C", "%7D", "%7E"};

    for (int i = 0; i<35; i++){
        externalWifiId.replace(specialChar_PE[i], specialChars[i]);
        externalWifiPassword.replace(specialChar_PE[i], specialChars[i]);
        externalUsername.replace(specialChar_PE[i], specialChars[i]);
    }
}

bool WebConfig::isConfigured() {
  return !(externalWifiId == "" || externalWifiPassword == "");
}