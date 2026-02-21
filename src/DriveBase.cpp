#include "DriveBase.h"

// :: is scope resolution operator
// allows the use of private variables from header within function body

DriveBase::DriveBase()
  : profile(0.0f, 0.0f, 0.0f, 0.0f) { // initial dummy values
  movePID = nullptr;
  turnPID = nullptr;
}

void DriveBase::init(int m1p1, int m1p2, int m2p1, int m2p2, int pwmA, int pwmB) {

  // setup encoder
  encoder.init();
  encoder.resetAngle();

  // setup gyroscope
  gyro.init();

  // stup PID
  movePID = new PIDController(MOVE_P, 0, 0);
  movePID->setInegralSumBounds(-MOVE_INTEGRAL_BOUND, MOVE_INTEGRAL_BOUND);
  movePID->setOutputBounds(-MOVE_OUTPUT_BOUND, MOVE_OUTPUT_BOUND);

  turnPID = new PIDController(TURN_P, 0, 0);
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

  analogWrite(pwmPin1, clamp(speed, -MAX_PWM, MAX_PWM));
	analogWrite(pwmPin2, clamp(speed, -MAX_PWM, MAX_PWM));
}

void DriveBase::moveBack(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  analogWrite(pwmPin1, clamp(speed, -MAX_PWM, MAX_PWM));
	analogWrite(pwmPin2, clamp(speed, -MAX_PWM, MAX_PWM));
}

void DriveBase::turnRight(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  analogWrite(pwmPin1, clamp(speed, -MAX_PWM, MAX_PWM)); // clamp needed bc pwm past 255 causes unwanted slowed behavior
	analogWrite(pwmPin2, clamp(speed, -MAX_PWM, MAX_PWM));
}

void DriveBase::turnLeft(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  analogWrite(pwmPin1, clamp(speed, -MAX_PWM, MAX_PWM));
	analogWrite(pwmPin2, clamp(speed, -MAX_PWM, MAX_PWM));
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

  profile = MotionProfile(
    0,
    position,
    MAX_VEL,
    MAX_ACC
  );

  startTime = millis();
}

void DriveBase::updateMove() {

  float t = (millis() - startTime) / 1000.0f; // get seconds with millsec resolution

  MotionState s = profile.tCurve1D(t);

  float error = s.vel - getRobotVelocity(); // calculate error
  float feedback = movePID->update(error);
  float command = feedback + (s.vel * MOVE_FF); // feedback + feedforward
  // out = feedback;
  // out2 = s.vel * TURN_FF;
  // out3 = error;
  // out4 = getRobotVelocity();
  // targetVel = s.vel;
  
  // check if error is pos or neg to determin how to move (pwm set to neg does not switch direction)
  if (s.vel > 0) {
    moveForward(command);
  }
  else if (s.vel < 0) {
    moveBack(-command); // make negative command positive so it is functional by moveBack
  }
  else {
    stop();
  }
}

void DriveBase::turn(float angle) { // + right, - left
  targetAngle = angle;

  profile = MotionProfile(
    0,
    angle,
    MAX_VEL_TURN,
    MAX_ACC_TURN
  );

  startTime = millis();
}

void DriveBase::updateTurn() {
  float t = (millis() - startTime) / 1000.0f; // get seconds with millsec resolution

  MotionState s = profile.tCurve1D(t);

  float error = s.vel - getTurnVel(); // calculate error
  float feedback = turnPID->update(error);
  float command = feedback + (s.vel * TURN_FF);
  // out = feedback;
  // out2 = s.vel * TURN_FF;
  // out3 = error;
  // targetVel = s.vel;

  if (s.vel > 0) {
    turnRight(command);
  }
  else if (s.vel < 0) {
    turnLeft(-command); // make negative command positive so it is functional by moveBack
  }
  else {
    stop();
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

float DriveBase::getCurrentWheelPosition() { // in
  return -encoder.getAngle() * (ENCODER_DIAMETER * M_PI / 360);
}

float DriveBase::getCurrentWheelDegree() {
  return -encoder.getAngle();
}

// clockwise is positive
float DriveBase::getCurrentRobotAngle() {
  return gyro.getAngle();
}

float DriveBase::getRobotVelocity() {
  unsigned long currTime = millis();
  float currPos = getCurrentWheelPosition();
  float vel = (currPos - lastPosition) / ((currTime - lastTimeVel) / 1000.0f);
  lastPosition = currPos;
  lastTimeVel = currTime;
  return vel;
}

float DriveBase::getRobotAcc() {
  unsigned long currTime = millis();
  float currVel = getRobotVelocity();
  float acc = (currVel - lastVelocityPos) / ((currTime - lastTimeAcc) / 1000.0f);
  lastVelocityPos = currVel;
  lastTimeAcc = currTime;
  return acc;
}

float DriveBase::getTurnVel() {
  unsigned long currTime = millis();
  float currPos = getCurrentRobotAngle();
  float vel = (currPos - lastAngle) / ((currTime - lastTimeVelAngle) / 1000.0f);
  lastAngle = currPos;
  lastTimeVelAngle = currTime;
  return vel;
}

float DriveBase::getTurnAcc() {
  unsigned long currTime = millis();
  float currVel = getTurnVel();
  float acc = (currVel - lastVelocityAngle) / ((currTime - lastTimeAccAngle) / 1000.0f);
  lastVelocityAngle = currVel;
  lastTimeAccAngle = currTime;
  return acc;
}


float DriveBase::getTargetPosition() {
  return targetPosition;
}

float DriveBase::getTargetAngle() {
  return targetAngle;
}

// how close to target can we get before we say we are there
bool DriveBase::atTargetPosition() {
  return (getCurrentWheelPosition() > getTargetPosition() - POSITION_TOLERANCE && getCurrentWheelPosition() < getTargetPosition() + POSITION_TOLERANCE) || profile.atPosition();
}

// how close to target can we get before we say we are there
bool DriveBase::atTargetAngle() {
  return (getCurrentRobotAngle() > getTargetAngle() - ANGLE_TOLERANCE && getCurrentRobotAngle() < getTargetAngle() + ANGLE_TOLERANCE) || profile.atPosition();
}

// getters and setters
void DriveBase::setGlobalSpeed(int pwm) {
  globalSpeed = pwm;
}

int DriveBase::getGlobalSpeed() {
  return globalSpeed;
}

void DriveBase::setStartTime() {
  startTime = millis();
}

int DriveBase::clamp(int value, int min_val, int max_val) {
  if (value < min_val) {
    return min_val;
  } else if (value > max_val) {
    return max_val;
  } else {
    return value;
  }
}