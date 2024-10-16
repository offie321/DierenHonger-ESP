#ifndef HX711_HANDLER_H
#define HX711_HANDLER_H

#include <Arduino.h>
#include "HX711.h"

class HX711Handler {
public:
    HX711Handler(uint8_t dataPin, uint8_t clockPin);

    void begin();
    float readWeight();
    bool isReady();

private:
    HX711 scale;
    uint8_t _dataPin;
    uint8_t _clockPin;
};

#endif
