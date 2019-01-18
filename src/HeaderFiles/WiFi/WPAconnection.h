#ifndef WPA_CONNECTION_H
#define WPA_CONNECTION_H

#include <esp_wpa2.h>   
#include "HeaderFiles/WiFi/WifiConnection.h"


class WPAconnection : public WifiConnection
{
public:
	WPAconnection(String id, String pw, String EAP_USERNAME);
	virtual ~WPAconnection() {}

	virtual void connect();
    //virtual void disconnect();

private:
    String EAP_USERNAME;
};

#endif // WPA_CONNECTION_H