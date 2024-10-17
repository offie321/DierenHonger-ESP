#include "ServerCommunication.h"
#include <HTTPClient.h>

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
