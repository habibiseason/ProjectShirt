#include "HeaderFiles/WifiConnection.h"

WifiConnection::WifiConnection(String id, String pw): ssid(id), password(pw){

}

void WifiConnection::connect(){
    cout<<"Connecting to Access point: "<< getSsid() <<endl;
}

void WifiConnection::disconnect(){
    cout<<"Ending wifi connection with "<< getSsid() <<endl;
}
