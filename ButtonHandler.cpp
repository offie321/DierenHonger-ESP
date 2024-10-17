#include "ButtonHandler.h"
#include "WiFiConfig.h"
#include "StepperMotorHandler.h"  // Include stepper motor handler
#include <HTTPClient.h>

int lastButtonState = HIGH;
int ledState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void set_remote_button() {
    if (wifiMulti.run() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://rpi-offie.local:8000/button_pressed");
        int httpCode = http.GET();
        if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}

void handleButtonPress() {
    int reading = digitalRead(23);  // Read the button state
    Serial.print("Button reading: "); 
    Serial.println(reading);  // Debugging button state

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
        Serial.println("Button state changed.");
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW && ledState == LOW) {  // Button pressed
            ledState = HIGH;
            set_remote_button();
            Serial.println("Button Pressed - Activating stepper motor");

            // Activate stepper motor to dispense food
            handleStepperMotorMovement();
            delay(5000);  // Adjust delay as needed
            Serial.println("Stepper motor activated");

        } else if (reading == HIGH && ledState == HIGH) {  // Button released
            ledState = LOW;
            Serial.println("Button Released - Motor off");
        }
    }
    lastButtonState = reading;
}
