#include "HX711Handler.h"

HX711Handler::HX711Handler(uint8_t dataPin, uint8_t clockPin) : _dataPin(dataPin), _clockPin(clockPin) {}

void HX711Handler::begin() {
    scale.begin(_dataPin, _clockPin);
    scale.set_scale(483.004944); // Adjust this as per your calibration
    scale.tare(20);            // Reset the scale to zero
}

float HX711Handler::readWeight() {
    if (scale.is_ready()) {
        return scale.get_units(1);  // Return the weight
    }
    return NAN;  // If not ready, return 'Not a Number'
}

bool HX711Handler::isReady() {
    return scale.is_ready();
}
