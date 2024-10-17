#include "WiFiConfig.h"

WiFiMulti wifiMulti;

void setupWiFi() {
  wifiMulti.addAP("Ziggo9F8EC87", "crsa7zhehfRk"); // Add your WiFi credentials
    // wifiMulti.addAP("OFFIE", "Jemoeder"); // Add your WiFi credentials
}