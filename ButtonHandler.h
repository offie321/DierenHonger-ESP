// ButtonHandler.h

#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

extern int lastButtonState;
extern int ledState;
extern unsigned long lastDebounceTime;
extern unsigned long debounceDelay;

void handleButtonPress();
void set_remote_button(); // Function to notify server about button press

#endif
