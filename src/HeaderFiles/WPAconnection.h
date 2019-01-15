#ifndef WPA_CONNECTION_H
#define WPA_CONNECTION_H

#include "WifiConnection.h"


class WPAconnection : public WifiConnection
{
public:
	WPAconnection(string id, string pw, string usrName);
	virtual ~WPAconnection() {}

	virtual void connect();
    virtual void disconnect();

private:
    string userName;
};

#endif // WPA_CONNECTION_H