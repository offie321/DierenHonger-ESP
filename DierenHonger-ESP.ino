#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "LEDControl.h"

#define BUTTON_PIN 23
#define LED_PIN 22

unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    
    setupWiFi();
}

void loop() {
    // Check LED state every 5 seconds
    if (millis() - lastHttpRequest >= httpInterval) {
        get_remote_led_state();
        lastHttpRequest = millis();
    }

    // Handle button press
    handleButtonPress();
}