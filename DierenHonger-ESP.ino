#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "ServerCommunication.h" // Include your server communication header
#include "HX711Handler.h"
#include "StepperMotorHandler.h"
#include "LEDControl.h"

#define BUTTON_PIN 23
#define LED_PIN 22

unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds
unsigned long lastWeightRequest = 0;
unsigned long weightInterval = 60000; // Send weight every 60 seconds

HX711Handler scale(19, 18);

String getCurrentTime() {
    // Simulate current time as "HH:MM:SS"
    // You can adjust this to simulate different times
    return "14:30:00"; // Example: 2:30 PM
}

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

    // // Handle button press
    // handleButtonPress();

    // // Handle stepper motor movement
    // handleStepperMotorMovement();

    // Fetch feeding times every minute
    if (millis() - lastFeedingCheck >= feedingCheckInterval) {
        fetchFeedingTimes();
        lastFeedingCheck = millis();
    }

    // Check feeding times against current time
    for (int i = 0; i < sizeof(feedingTimes)/sizeof(feedingTimes[0]); i++) {
        if (currentTime.startsWith(feedingTimes[i])) { // Check if current time matches any feeding time
            Serial.println("Feeding time matched: " + feedingTimes[i]);
            // Trigger stepper motor for a few seconds
            handleStepperMotorMovement(); // Adjust this to your specific logic for running the motor
            delay(5000); // Run motor for 5 seconds (adjust as needed)
            break; // Exit loop after matching feeding time
        }
    }

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

void fetchFeedingTimes() {
    if (wifiMulti.run() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://rpi-offie.local:8000/feeding-times/times");
        int httpCode = http.GET();
        if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
 // Example: Assume the response is a JSON array of strings
            // Parse the JSON and store in feedingTimes array (assuming up to 3 times)
            DynamicJsonDocument doc(1024); // Adjust size as necessary
            deserializeJson(doc, payload);
            for (int i = 0; i < doc.size() && i < 3; i++) {
                feedingTimes[i] = doc[i].as<String>();
                Serial.println("Feeding time: " + feedingTimes[i]);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}
