#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "ServerCommunication.h" // Include your server communication header
#include "HX711Handler.h"
#include "StepperMotorHandler.h"
#include "LEDControl.h"

#include <HTTPClient.h>          // Include HTTPClient
#include <ArduinoJson.h>        // Include ArduinoJson for JSON handling

#define BUTTON_PIN 23
#define LED_PIN 22

// Global variables
unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds
unsigned long lastWeightRequest = 0;
unsigned long weightInterval = 60000; // Send weight every 60 seconds
unsigned long lastFeedingCheck = 0; // Declare lastFeedingCheck
unsigned long feedingCheckInterval = 60000; // Declare feedingCheckInterval (e.g., 60 seconds)
HX711Handler scale(19, 18);

// State variables
bool feedingTriggered = false; // State variable to track if feeding has been triggered
String feedingTimes[3]; // Declare an array for feeding times (adjust size as needed)

String getCurrentTime() {
    // Simulate current time as "HH:MM:SS"
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

    // Fetch feeding times every minute
    if (millis() - lastFeedingCheck >= feedingCheckInterval) {
        fetchFeedingTimes();
        lastFeedingCheck = millis();
        feedingTriggered = false; // Reset when new feeding times are fetched
    }

    // Get the current time as a string
    String currentTime = getCurrentTime(); 

    // Check feeding times against current time
    for (int i = 0; i < sizeof(feedingTimes) / sizeof(feedingTimes[0]); i++) {
        if (currentTime == feedingTimes[i] && !feedingTriggered) {  // Use state variable here
            Serial.println("Feeding time matched: " + feedingTimes[i]);
            handleStepperMotorMovement(); 
            delay(5000); // Run motor for 5 seconds
            feedingTriggered = true; // Set state variable to true to prevent re-triggering
            break; // Exit loop after matching feeding time
        } else if (currentTime.startsWith(feedingTimes[i]) && feedingTriggered) {
            // Do nothing if feeding has already been triggered for this time
            break; 
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
        http.begin("http://rpi-offie.local:8000/feed-schedule/times");
        int httpCode = http.GET();
        if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            // Example: Assume the response is a JSON array of strings
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
