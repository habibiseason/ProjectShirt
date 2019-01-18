#include "HeaderFiles/WPAconnection.h"


WPAconnection::WPAconnection(String id, String pw, String usrName): WifiConnection(id, pw), EAP_USERNAME(usrName){

}

void WPAconnection::connect(){
    WiFi.disconnect(true);

    string id = getSsid().c_str(), pass = getPassword().c_str();

    cout<<"Starting connection (WPA) with: "<< id <<endl;
    WiFi.mode(WIFI_STA);
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_USERNAME.c_str(), strlen(EAP_USERNAME.c_str()));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME.c_str(), strlen(EAP_USERNAME.c_str()));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)pass.c_str(), strlen(pass.c_str()));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);
    WiFi.begin(id.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting..");
    }

    Serial.print("IP address: ");Serial.println(WiFi.localIP());
    Serial.print("Connected to "); Serial.println(getSsid());
}
