#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <Arduino.h>
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

private:
    String ssid, password;
};

#endif // WIFI_CONNECTION_H