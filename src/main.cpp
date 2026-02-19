#include <Arduino.h>
#include "DriveBase.h"

DriveBase drive;

const int MOTOR1_PIN1 = 6; // motor 1 is on right side of robot
const int MOTOR1_PIN2 = 5;
const int MOTOR2_PIN1 = 4; // motor 2 is on left side of robot
const int MOTOR2_PIN2 = 3;
const int PWM_PINA = 7; // pwm for motor 1
const int PWM_PINB = 2; // pwm for motor 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  drive.init(MOTOR1_PIN1, MOTOR1_PIN2, MOTOR2_PIN1, MOTOR2_PIN2, PWM_PINA, PWM_PINB);
}

void loop() {
  drive.moveForward(255);
  // Serial.println("hi");
}