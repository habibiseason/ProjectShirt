#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#define DEFAULT_PASSWORD	"Admin123"
#define AP_IP				1,1,1,1
#define AP_MASK				255,255,0,0

#define SPECIAL_CHAR_BUFF	35

#include <Arduino.h>
#include <WiFi.h>
#include "HeaderFiles/WiFi/WifiConnection.h"
#include "HeaderFiles/WiFi/WPAconnection.h"
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

	virtual void handleResponse(WiFiClient client);
	virtual void handleRequest(WiFiClient client);
	virtual void displayWebpage(WiFiClient client, bool connectionStat);

private:
    WifiConnection *WifiConn;

	//WiFi credentials Access Point
	char *AP_ssid;
	char *AP_password;

	//External WiFi credentials
	String externalWifiId;
	String externalWifiPassword;
	String externalUsername;	//Used for WPA

	WiFiServer server;
	IPAddress ip;
	String request;
	String requestLine;

	bool _connection;
};


#endif  //WEB_CONFIG_H