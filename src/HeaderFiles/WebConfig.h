#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#define DEFAULT_PASSWORD	"Admin123"
#define AP_IP				192,168,5,1
#define AP_MASK				255,255,255,0

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
	virtual void waitForInput();

	virtual bool isConfigured();
	virtual void handleResponse(WiFiClient client);
	virtual void handleRequest(WiFiClient client);
	virtual void displayWebpage(WiFiClient client);

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
};


#endif  //WEB_CONFIG_H