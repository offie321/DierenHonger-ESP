#include "LEDControl.h"
#include "WiFiConfig.h"
#include <HTTPClient.h>

int led_state = 0;

void set_led(int led_state) {
    digitalWrite(22, led_state == 1 ? HIGH : LOW); // Set LED pin
}

void get_remote_led_state() {
    if (wifiMulti.run() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://rpi-offie.local:8000/get_led_state");
        int httpCode = http.GET();
        if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            led_state = payload.toInt();
            set_led(led_state);
            Serial.println(led_state);
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}