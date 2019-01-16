#ifndef WPA_CONNECTION_H
#define WPA_CONNECTION_H

#include "WifiConnection.h"


class WPAconnection : public WifiConnection
{
public:
	WPAconnection(String id, String pw, String usrName);
	virtual ~WPAconnection() {}

	virtual void connect();
    virtual void disconnect();

private:
    String userName;
};

#endif // WPA_CONNECTION_H