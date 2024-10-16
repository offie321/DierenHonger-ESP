#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "ServerCommunication.h" // Include your server communication header
#include "HX711Handler.h"
#include "StepperMotorHandler.h"

#define BUTTON_PIN 23
#define LED_PIN 22

unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds
unsigned long lastWeightRequest = 0;
unsigned long weightInterval = 60000; // Send weight every 60 seconds

HX711Handler scale(19, 18);

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    
    setupWiFi();
    scale.begin();
    setupStepperMotor();  
}

void loop() {
    // Check LED state every 5 seconds
    if (millis() - lastHttpRequest >= httpInterval) {
        get_remote_led_state();
        lastHttpRequest = millis();
    }

    // Handle button press
    handleButtonPress();

    // Handle stepper motor movement
    handleStepperMotorMovement();

    // Read and print weight from the scale
    if (scale.isReady()) {
        float weight = scale.readWeight();
        Serial.println(weight);

        // Send weight data to the Raspberry Pi every 60 seconds
        if (millis() - lastWeightRequest >= weightInterval) {
            sendWeightToServer(weight);
            lastWeightRequest = millis();
        }
    }
}
