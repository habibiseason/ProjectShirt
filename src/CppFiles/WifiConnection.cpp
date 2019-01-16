#include "HeaderFiles/WifiConnection.h"

WifiConnection::WifiConnection(String id, String pw): ssid(id), password(pw){

}

void WifiConnection::connect(){
    cout<<"Connecting to Access point: "<< getSsid() <<endl;
    disconnect();
    WiFi.begin(ssid.c_str(), password.c_str());
    
    //try to connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting..");
    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WifiConnection::disconnect(){
    cout<<"Ending wifi connection with "<< getSsid() <<endl;
    WiFi.disconnect(true);
    //wait till disconnected
    while(WiFi.status() == WL_CONNECTED) {}
    Serial.println("Disconnected");
    delay(1000);
}

void WifiConnection::sendAllSensorData(){
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Stretchband
    String body = "type=stretchband&timestamp="+String(millis())+"&value=" + String(10);
    int code = http.POST(body);

    // IMU
    body = "type=IMU&imu_id=1&timestamp="+String(millis())+"&x=" + String(20)+ "&y=" +String(21)+ "&z=" + String(22);
    code = http.POST(body);
    body = "type=IMU&imu_id=2&timestamp="+String(millis())+"&x=" + String(30)+ "&y=" +String(31)+ "&z=" + String(32);
    code = http.POST(body);

    // Capacitive
    body = "type=capacitive&capacitive_id=1&timestamp="+String(millis())+ "&value=" + String(40);
    code = http.POST(body);
    body = "type=capacitive&capacitive_id=2&timestamp="+String(millis())+"&value=23";
    code = http.POST(body);

    // Gsr
    body = "type=gsr&timestamp="+String(millis())+"&value=" + String(50);
    code = http.POST(body);

    Serial.println(code);
    String response = http.getString();
    Serial.println(response);

    http.end();
}