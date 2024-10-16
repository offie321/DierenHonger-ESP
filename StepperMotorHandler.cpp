#include "StepperMotorHandler.h"

// Reassigned ULN2003 Motor Driver Pins to avoid conflicts
#define IN1 16  // Changed from 19
#define IN2 4   // Changed from 18
#define IN3 5   
#define IN4 17  

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

unsigned long lastStepperMovement = 0;
unsigned long stepperInterval = 1000;  // Move stepper motor every 1 second

void setupStepperMotor() {
    myStepper.setSpeed(5);  // Set speed at 5 rpm
}

void handleStepperMotorMovement() {
    // Move stepper motor every 1 second
    if (millis() - lastStepperMovement >= stepperInterval) {
        // Move one revolution clockwise
        Serial.println("Stepper: clockwise");
        myStepper.step(stepsPerRevolution);
        delay(1000);

        // Move one revolution counterclockwise
        Serial.println("Stepper: counterclockwise");
        myStepper.step(-stepsPerRevolution);
        lastStepperMovement = millis();
    }
}
