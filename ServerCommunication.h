#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H

#include <Arduino.h>
#include <WiFi.h>

void sendWeightToServer(float weight);
void fetchFeedingTimes(); // Declaration to fetch feeding times
String* getFeedingTimes(); // Declaration to get feeding times
int getNumFeedingTimes(); // Declaration to get the number of feeding times

#endif
