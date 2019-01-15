#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;


class WifiConnection
{
public:
	WifiConnection(string id, string pw);
	virtual ~WifiConnection() {}

	virtual void connect();
    virtual void disconnect();

    virtual string getSsid() { return ssid; }
    virtual string getPassword() { return password; }

private:
    string ssid, password;
};

#endif // WIFI_CONNECTION_H