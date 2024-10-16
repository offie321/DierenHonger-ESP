#include "ServerCommunication.h"
#include <HTTPClient.h>
#include <ArduinoJson.h> // Make sure to install this library for JSON parsing

const int maxFeedingTimes = 10; // Maximum number of feeding times
String feedingTimes[maxFeedingTimes]; // Array to store feeding times
int numFeedingTimes = 0; // Number of feeding times

void sendWeightToServer(float weight) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String weightStr = String((int)weight);
        String url = "http://rpi-offie.local:8000/weight/" + weightStr; 
        http.begin(url); 

        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Response: " + response);
        } else {
            Serial.print("Error sending weight: ");
            Serial.println(httpResponseCode);
        }

        http.end(); 
    } else {
        Serial.println("WiFi not connected");
    }
}

void fetchFeedingTimes() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String apiUrl = "http://rpi-offie.local:8000/api/feed-schedule/times"; // Adjust the URL

        http.begin(apiUrl);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("Feeding Times: " + payload);

            // Clear previous feeding times
            numFeedingTimes = 0;

            // Parse the JSON response (assuming it's a simple array)
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonArray array = doc.as<JsonArray>();

            for (JsonVariant value : array) {
                if (numFeedingTimes < maxFeedingTimes) { // Avoid overflow
                    feedingTimes[numFeedingTimes++] = value.as<String>();
                }
            }
        } else {
            Serial.println("Error fetching feeding times");
        }

        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
}

String* getFeedingTimes() {
    return feedingTimes;
}

int getNumFeedingTimes() {
    return numFeedingTimes;
}