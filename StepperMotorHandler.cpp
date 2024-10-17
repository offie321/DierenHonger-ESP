#include "StepperMotorHandler.h"

// Reassigned ULN2003 Motor Driver Pins to avoid conflicts
#define IN1 16  // Driver pin for IN1
#define IN2 4   // Driver pin for IN2
#define IN3 5   // Driver pin for IN3
#define IN4 17  // Driver pin for IN4

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

unsigned long lastStepperMovement = 0;
unsigned long stepperInterval = 1000;  // Move stepper motor every 1 second

void setupStepperMotor() {
    Serial.begin(115200);  // Initialize serial communication
    myStepper.setSpeed(15);  // Set speed at 5 rpm
}

void handleStepperMotorMovement() {
    // Move stepper motor every 1 second
    if (millis() - lastStepperMovement >= stepperInterval) {
        Serial.println("Stepper: clockwise");
        myStepper.step(stepsPerRevolution);  // Move one revolution clockwise
        lastStepperMovement = millis();  // Update the last movement time
    }
}
