#include "HeaderFiles/WPAconnection.h"


WPAconnection::WPAconnection(string id, string pw, string usrName): WifiConnection(id, pw), userName(usrName){

}

void WPAconnection::connect(){
    cout<<"Connecting to a WPA access point: "<< getSsid() <<endl;
}


void WPAconnection::disconnect(){
    cout<<"Ending WPA connection"<< getSsid() <<endl;  
}