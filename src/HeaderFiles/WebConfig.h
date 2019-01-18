#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#define DEFAULT_PASSWORD	"Admin123"
#define AP_IP				1,1,1,1
#define AP_MASK				0,0,0,0

#include <Arduino.h>
#include <WiFi.h>
#include "WifiConnection.h"
#include "WPAconnection.h"
#include <string>

using namespace std;


class WebConfig
{
public:
	WebConfig(char* id, char* pass);
	virtual ~WebConfig() {}

	virtual WifiConnection* startAP();
	virtual void waitForConnection();

	virtual void replaceChar();

	virtual bool isConfigured();
	virtual void handleResponse(WiFiClient client);
	virtual void handleRequest(WiFiClient client);
	virtual void displayWebpage(WiFiClient client, bool connectionStat);

private:
    WifiConnection *WifiConn;

	char *ssid;
	char *password;

	String externalWifiId;
	String externalWifiPassword;
	String externalUsername;	//for WPA

	WiFiServer server;
	IPAddress ip;
	String request;
	String requestLine;

	bool _connection;
};


#endif  //WEB_CONFIG_H