#include "HeaderFiles/WiFi/WifiConnection.h"
#include "HeaderFiles/CommonInterface.h"

WifiConnection::WifiConnection(String id, String pw): ssid(id), password(pw){

}

void WifiConnection::connect(){
    cout<<"Starting connection (normal) with: "<< getSsid() <<endl;
    disconnect();
    WiFi.begin(ssid.c_str(), password.c_str());
    
    //try to connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting..");
    }

    Serial.print("IP address: ");Serial.println(WiFi.localIP());
    Serial.print("Connected to "); Serial.println(getSsid());
}

void WifiConnection::disconnect(){
    cout<<"Ending WiFi connection with "<< getSsid() <<endl;
    WiFi.disconnect(true);
    //wait till disconnected
    while(WiFi.status() == WL_CONNECTED) {}
    Serial.println("Disconnected");
    delay(1000);
}

void WifiConnection::sendAllSensorData(String sensor){
    http.begin(WEB_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Stretchband
    int code=-9;

    if(sensor.equals("stretch")) {
        if(!CommonInterface::stretchQueue.empty()) {
            int size = CommonInterface::stretchQueue.size();
            String ids = "";
            String timestamps = "";
            String values = "";
            while(!CommonInterface::stretchQueue.empty()) {
                stretchband_struct data = CommonInterface::stretchQueue.front();
                ids += String(data.sensorId)+",";
                timestamps += String(data.timestamp)+",";
                values += String(data.value)+",";
                CommonInterface::stretchQueue.pop();
            }

            String body = "type=stretchband&stretch_id="+ids+"&timestamp="+timestamps+"&value=" + values;
            code = http.POST(body);
        }
    } else if(sensor.equals("imu")) {
        if(!CommonInterface::imuQueue.empty()) {
            int size = CommonInterface::imuQueue.size();
            String ids = "";
            String timestamps = "";
            String x = ""; String y = ""; String z = "";

            while(!CommonInterface::imuQueue.empty()) {
                imu_struct data = CommonInterface::imuQueue.front();
                ids += String(data.sensorId) + ",";
                timestamps += String(data.timestamp) + ",";
                x += String(data.x) + ","; 
                y += String(data.y) + ","; 
                z += String(data.z) + ",";
                CommonInterface::imuQueue.pop();
            }
            String body = "type=IMU&imu_id="+ids+"&timestamp="+timestamps+"&x="+x+"&y="+y+"&z="+z;
            code = http.POST(body);
        }
    } else if(sensor.equals("capacitive")) {
        if(!CommonInterface::capacitiveQueue.empty()) {
            int size = CommonInterface::capacitiveQueue.size();
            String ids = "";
            String timestamps = "";
            String values = "";

            while(!CommonInterface::capacitiveQueue.empty()) {
                capacitive_struct data = CommonInterface::capacitiveQueue.front();
                ids += String(data.sensorId) + ",";
                timestamps += String(data.timestamp) + ",";
                values += String(data.value) + ",";
                CommonInterface::capacitiveQueue.pop();
            }

            String body = "type=capacitive&capacitive_id="+ids+"&timestamp="+timestamps+"&value="+values;
            code = http.POST(body);
        }
    } else if(sensor.equals("gsr")) {
        if(!CommonInterface::gsrQueue.empty()) {
            int size = CommonInterface::gsrQueue.size();
            String timestamps = "";
            String values = "";

            while(!CommonInterface::gsrQueue.empty()) {
                gsr_struct data = CommonInterface::gsrQueue.front();
                timestamps += String(data.timestamp) + ",";
                values += String(data.value) + ",";
                CommonInterface::gsrQueue.pop();
            }
            String body = "type=gsr&timestamp="+timestamps+"&value="+values;
            code = http.POST(body);
        }
    }

    http.end();
}