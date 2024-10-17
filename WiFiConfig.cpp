#include "WiFiConfig.h"

WiFiMulti wifiMulti;

void setupWiFi() {
    wifiMulti.addAP("", ""); // Add your WiFi credentials ("wifihost", "password")
}