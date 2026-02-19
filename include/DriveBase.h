#pragma once // acts as header gaurd

#include <Arduino.h> // include arduino library
#include <math.h>
#include "PIDController.h"
#include "Encoder.h"
#include "Imu.h"

class DriveBase {

  public:

  Encoder encoder;

  void init(int m1p1, int m1p2, int m2p1, int m2p2, int pwmA, int pwmB);

  // used to free PID instances
  void freeMemory();

  // move function at set speed
  void moveForward(int speed);
  void moveBack(int speed);
  void turnRight(int speed);
  void turnLeft(int speed);

  // move functions that move at global speed:
  void moveForward();
  void moveBack();
  void turnRight();
  void turnLeft();

  // move functions at set position (in)
  void moveForwardIn(float position);
  void moveBackIn(float position);

  // turning fuctions at set angle (deg)
  void turnRightDeg(float angle);
  void turnLeftDeg(float angle);

  // generalized motion functions with PID
  // note can be used in a global scope where previous positions are accounted for in the next movement or 
  // user can resent the position/angle every movement to make the command more relative
  void move(float position); // position in inches
  void turn(float angle); // in degrees

  void stop();

  void resetEncoder();
  void resetGyro();

  // get position functions
  float getCurrentWheelPosition();
  float getCurrentWheelDegree();
  float getCurrentRobotAngle();

  // get target functions
  float getTargetPosition();
  float getTargetAngle();

  // at target functions
  bool atTargetPosition();
  bool atTargetAngle();

  // speed get and set
  void setGlobalSpeed(int pwm);
  int getGlobalSpeed();

  private:

  const float GEARBOX_RATIO = 57.7;
  const float ENCODER_RATIO = 97/38; // wheel / encoder wheel in mm
  const float WHEEL_DIAMETER = 4.875; // in
  const float ENCODER_DIAMETER = 1.49606; // in

  int globalSpeed = 255; // default is 255 (max speed);

  float targetPosition;
  float targetAngle;
  
  // motor power pins
  int motor1Pin1;
  int motor1Pin2;
  int motor2Pin1;
  int motor2Pin2;

  // motor pwm pins
  int pwmPin1;
  int pwmPin2;

  // move pid vars
  const float MOVE_P = 40;
  const float MOVE_I = 1;
  const float MOVE_INTEGRAL_BOUND = 50;
  const float MOVE_OUTPUT_BOUND = 255;

  // turn pid vars
  const float TURN_P = 20;
  const float TURN_I = 0.5;
  const float TURN_INTEGRAL_BOUND = 100;
  const float TURN_OUTPUT_BOUND_TURN = 255;

  // how close to target can we get before we say we are there
  const float POSITION_TOLERANCE = 0.5; // in
  const float ANGLE_TOLERANCE = 1; // deg

  PIDController* movePID; // declare obj using dynamic storage duration method
                          // allows usage of new keyword so obj can be initialized in .cpp init fuction
                          // will need to delete obj when not being used to prevent memory leaks
  PIDController* turnPID;
  // Encoder encoder;
  Imu gyro;
    
};