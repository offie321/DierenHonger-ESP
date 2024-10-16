#include <Arduino.h>
#include "WiFiConfig.h"
#include "ButtonHandler.h"
#include "ServerCommunication.h" // Include your server communication header
#include "HX711Handler.h"
#include "StepperMotorHandler.h"
#include "LEDControl.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define BUTTON_PIN 23
#define LED_PIN 22

// Define necessary global variables
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 3600, 60000); // NTP client for getting the time

unsigned long lastHttpRequest = 0;
unsigned long httpInterval = 5000;  // Check LED state every 5 seconds
unsigned long lastWeightRequest = 0;
unsigned long weightInterval = 60000; // Send weight every 60 seconds
unsigned long lastFetchTime = 0;
unsigned long fetchInterval = 600000; // Fetch feeding times every 10 minutes

HX711Handler scale(19, 18);

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    
    setupWiFi();
    scale.begin();
    setupStepperMotor();  

    timeClient.begin(); // Start the NTP client
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

    // Update time from NTP server
    timeClient.update(); 
    String currentTime = timeClient.getFormattedTime().substring(0, 5); // Get current time in HH:MM format

    // Fetch feeding times periodically (e.g., every 10 minutes)
    if (millis() - lastFetchTime >= fetchInterval) {
        fetchFeedingTimes();
        lastFetchTime = millis();
    }

    // Get feeding times from server
    String* feedingTimes = getFeedingTimes();
    int numFeedingTimes = getNumFeedingTimes();

    // Check if the current time matches any feeding time
    for (int i = 0; i < numFeedingTimes; i++) {
        if (currentTime == feedingTimes[i]) {
            Serial.println("Feeding time match: " + currentTime);
            runStepperMotor();  // Call the function to run the stepper motor
            delay(5000);  // Keep the motor running for a few seconds
            break; // Exit loop after running the motor
        }
    }

    delay(1000); // Check every second
}

void runStepperMotor() {
    Serial.println("Stepper Motor Running");
    myStepper.step(stepsPerRevolution); // Adjust this according to how you want to dispense food
}