#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#define WEB_URL "http://kinetic-data.dynu.net/values.php"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <string>

using namespace std;


class WifiConnection
{
public:
	WifiConnection(String id, String pw);
	virtual ~WifiConnection() {}

	virtual void connect();
    virtual void disconnect();

    virtual String getSsid() { return ssid; }
    virtual String getPassword() { return password; }
    virtual void sendAllSensorData();

private:
    String ssid, password;
    HTTPClient http;
};

#endif // WIFI_CONNECTION_H