#pragma once // acts as header gaurd

#include <Arduino.h> // include arduino library
#include <math.h>
#include "PIDController.h"
#include "Encoder.h"
#include "Imu.h"
#include "MotionProfile.h"

class DriveBase {

  public:

  DriveBase();

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

  // generalized motion functions with PID
  // note can be used in a global scope where previous positions are accounted for in the next movement or 
  // user can resent the position/angle every movement to make the command more relative
  void move(float position); // position in inches
  void turn(float angle); // in degrees

  void stop();

  void updateMove();
  void updateTurn();

  void resetEncoder();
  void resetGyro();

  // get position functions
  float getCurrentWheelPosition();
  float getCurrentWheelDegree();
  float getCurrentRobotAngle();

  float getRobotVelocity();
  float getRobotAcc();

  float getTurnVel();
  float getTurnAcc();

  // get target functions
  float getTargetPosition();
  float getTargetAngle();

  // at target functions
  bool atTargetPosition();
  bool atTargetAngle();

  // speed get and set
  void setGlobalSpeed(int pwm);
  int getGlobalSpeed();

  void setStartTime();

  // float targetVel = 0;
  // float out = 0;
  // float out2 = 0;
  // float out3 = 0;
  // float out4 = 0;
  // float out5 = 0;

  private:

  const float GEARBOX_RATIO = 57.7;
  const float ENCODER_RATIO = 97/38; // wheel / encoder wheel in mm
  const float WHEEL_DIAMETER = 4.875; // in
  const float ENCODER_DIAMETER = 1.49606; // in
  const float MAX_VEL = 7.2; // encoder ticks / sec
  const float MAX_ACC = 25; // encdoer ticks / sec ^2
  const float MAX_VEL_TURN = 63; // degrees / sec
  const float MAX_ACC_TURN = 100; // degrees / sec ^2

  unsigned long startTime = 0;
  unsigned long lastTimeVel = 0;
  unsigned long lastTimeAcc = 0;
  unsigned long lastTimeVelAngle = 0;
  unsigned long lastTimeAccAngle = 0;

  const int MAX_PWM = 255;
  int globalSpeed = 255; // default is 255 (max speed);

  float targetPosition;
  float targetAngle;

  float lastPosition;
  float lastAngle;
  float lastVelocityPos;
  float lastVelocityAngle;
  
  // motor power pins
  int motor1Pin1;
  int motor1Pin2;
  int motor2Pin1;
  int motor2Pin2;

  // motor pwm pins
  int pwmPin1;
  int pwmPin2;

  // move pid vars
  const float MOVE_P = 15;
  const float MOVE_I = 0;
  const float MOVE_INTEGRAL_BOUND = 50;
  const float MOVE_OUTPUT_BOUND = 255;
  const float MOVE_FF = 24;
  const float TURN_FF = 4;

  // turn pid vars
  const float TURN_P = 5;
  const float TURN_I = 0;
  const float TURN_INTEGRAL_BOUND = 100;
  const float TURN_OUTPUT_BOUND_TURN = 255;

  // how close to target can we get before we say we are there
  const float POSITION_TOLERANCE = 0.75; // in
  const float ANGLE_TOLERANCE = 2; // deg

  PIDController* movePID; // declare obj using dynamic storage duration method
                          // allows usage of new keyword so obj can be initialized in .cpp init fuction
                          // will need to delete obj when not being used to prevent memory leaks
  PIDController* turnPID;
  // Encoder encoder;
  Imu gyro;
  MotionProfile profile;

  int clamp(int value, int min_val, int max_val);
};