#include "HeaderFiles/WebConfig.h"

WebConfig::WebConfig(WifiConnection *W): WifiConn(W){

}

WifiConnection* WebConfig::startBlocking(){
    cout << "Start blocking wifi access" << endl;
    return WifiConn;
}