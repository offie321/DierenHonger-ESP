#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "LEDControl.h"
#include "HX711Handler.h"

#define BUTTON_PIN 23
#define LED_PIN 22

unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds

HX711Handler scale(19, 18);

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    
    setupWiFi();
    scale.begin();  // Initialize the scale
}

void loop() {
    // Check LED state every 5 seconds
    if (millis() - lastHttpRequest >= httpInterval) {
        get_remote_led_state();
        lastHttpRequest = millis();
    }

    // Handle button press
    handleButtonPress();

    // Read and print weight from the scale
    if (scale.isReady()) {
        Serial.println(scale.readWeight());
    }
}