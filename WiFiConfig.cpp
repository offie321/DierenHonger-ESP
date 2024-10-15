#include "WiFiConfig.h"

WiFiMulti wifiMulti;

void setupWiFi() {
    wifiMulti.addAP("OFFIE", "Jemoeder"); // Add your WiFi credentials
}