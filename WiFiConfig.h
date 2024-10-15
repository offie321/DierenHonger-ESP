// WiFiConfig.h

#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include <WiFi.h>
#include <WiFiMulti.h>

extern WiFiMulti wifiMulti; // Declare it externally so it can be used across files

void setupWiFi();

#endif
