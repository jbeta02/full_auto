#include "DriveBase.h"

// :: is scope resolution operator
// allows the use of private variables from header within function body

void DriveBase::init(int m1p1, int m1p2, int m2p1, int m2p2, int pwmA, int pwmB) {

  // setup encoder
  encoder.init();
  encoder.resetAngle();

  // setup gyroscope
  gyro.init();

  // stup PID
  movePID = new PIDController(MOVE_P, MOVE_I, 0);
  movePID->setInegralSumBounds(-MOVE_INTEGRAL_BOUND, MOVE_INTEGRAL_BOUND);
  movePID->setOutputBounds(-MOVE_OUTPUT_BOUND, MOVE_OUTPUT_BOUND);

  turnPID = new PIDController(TURN_P, TURN_I, 0);
  turnPID->setInegralSumBounds(-TURN_INTEGRAL_BOUND, TURN_INTEGRAL_BOUND);
  turnPID->setOutputBounds(-TURN_OUTPUT_BOUND_TURN, TURN_OUTPUT_BOUND_TURN);

  // power pins (+ and -)
  motor1Pin1 = m1p1;
  motor1Pin2 = m1p2;
  motor2Pin1 = m2p1;
  motor2Pin2 = m2p2;

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // pwm pins
  pwmPin1 = pwmA;
  pwmPin2 = pwmB;

  pinMode(pwmPin1, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
}

// will need to delete Move obj since initialized using dynamic storage duration method
// used to prevent memory leaks
void DriveBase::freeMemory() {
  delete movePID;
  delete turnPID;
}

// movement functions at set speed
void DriveBase::moveForward(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  analogWrite(pwmPin1, speed);
	analogWrite(pwmPin2, speed);
}

void DriveBase::moveBack(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  analogWrite(pwmPin1, speed);
	analogWrite(pwmPin2, speed);
}

void DriveBase::turnRight(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  analogWrite(pwmPin1, speed);
	analogWrite(pwmPin2, speed);
}

void DriveBase::turnLeft(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  analogWrite(pwmPin1, speed);
	analogWrite(pwmPin2, speed);
}

// overload movment functions
void DriveBase::moveForward() {
  moveForward(globalSpeed);
}

void DriveBase::moveBack() {
  moveBack(globalSpeed);
}

void DriveBase::turnRight() {
  turnRight(globalSpeed);
}

void DriveBase::turnLeft() {
  turnLeft(globalSpeed);
}

// PID movment funtions (move and turn)
void DriveBase::move(float position) { // + forward, - back
  targetPosition = position;
  float error = position - getCurrentWheelPosition(); // calculate error
  float command = movePID->update(error); // get command determined by PID conroller 
                                //(using Arrow Operator to dereference movePID pointer then accessing update)
  // adjust speed based on command value (if at postion command will be 0)
  
  // check if error is pos or neg to determin how to move (pwm set to neg does not switch direction)
  if (error > 0) {
    moveForward(command);
  }
  else {
    moveBack(-command); // make negative command positive so it is functional by moveBack
  }

  // Serial.begin(2000000);
  // Serial.println(getCurrentWheelPosition());
  // Serial.println(command);
}

void DriveBase::turn(float angle) { // + right, - left
  targetAngle = angle;
  float error = angle - gyro.getAngle(); // calculate error
  float command = turnPID->update(error); // get command determined by PID conroller 
                                //(using Arrow Operator to dereference movePID pointer then accessing update)
  // adjust speed based on command value (if at postion command will be 0)

  if (error > 0) {
    turnRight(command);
  }

  else {
    turnLeft(-command);
  }
}

void DriveBase::stop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void DriveBase::resetEncoder() {
  encoder.resetAngle();
}

void DriveBase::resetGyro() {
  gyro.resetAngle();
}

float DriveBase::getCurrentWheelPosition() {
  return -encoder.getAngle() * (ENCODER_DIAMETER * M_PI / 360);
}

float DriveBase::getCurrentWheelDegree() {
  return -encoder.getAngle();
}

// clockwise is positive
float DriveBase::getCurrentRobotAngle() {
  return gyro.getAngle();
}

float DriveBase::getTargetPosition() {
  return targetPosition;
}

float DriveBase::getTargetAngle() {
  return targetAngle;
}

// how close to target can we get before we say we are there
bool DriveBase::atTargetPosition() {
  return getCurrentWheelPosition() > getTargetPosition() - POSITION_TOLERANCE && getCurrentWheelPosition() < getTargetPosition() + POSITION_TOLERANCE;
}

// how close to target can we get before we say we are there
bool DriveBase::atTargetAngle() {
  return getCurrentRobotAngle() > getTargetAngle() - ANGLE_TOLERANCE && getCurrentRobotAngle() < getTargetAngle() + ANGLE_TOLERANCE;
}

// getters and setters
void DriveBase::setGlobalSpeed(int pwm) {
  globalSpeed = pwm;
}

int DriveBase::getGlobalSpeed() {
  return globalSpeed;
}