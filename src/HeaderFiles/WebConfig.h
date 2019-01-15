#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#include "WifiConnection.h"

class WebConfig
{
public:
	WebConfig(WifiConnection* W);
	virtual ~WebConfig() {}

	virtual WifiConnection* startBlocking();

private:
    WifiConnection *WifiConn;
};


#endif  //WEB_CONFIG_H