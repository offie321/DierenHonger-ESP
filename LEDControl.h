// LEDControl.h

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>

extern int led_state;

void set_led(int led_state);
void get_remote_led_state();

#endif