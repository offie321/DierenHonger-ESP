#ifndef STEPPERMOTORHANDLER_H
#define STEPPERMOTORHANDLER_H

#include <Arduino.h>
#include <Stepper.h>

// Declare constants and variables for the stepper motor
const int stepsPerRevolution = 2048;
extern unsigned long lastStepperMovement;
extern unsigned long stepperInterval;

void setupStepperMotor();
void handleStepperMotorMovement();

#endif
