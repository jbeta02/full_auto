#pragma once // acts as header gaurd

#include <I2Cdev.h> //For binary address for values
#include <Wire.h> //Wire library for MPU
#include <MPU6050.h> //MPU6050 library for Gyroscope

////////////////////////TODO: be aware of problem
// if the robot is moved during the intialization of the arduino, the acceleration will be captured by 
// the sensor. Therefore, always start robot in a motionless state. Also check gyro output before doing
// any official runs to check it is correct. 

// If the sensor is moving during initialization then the gyro will linearly increase the angle which will 
// cause bad angle readings and turns. To solve issue, reupload the software or simply press the reset button on 
// the arduino. Turning the robot on and off sometimes doesn't solve the issue so the other methods are preferred. 

class Imu { 
  
  public:
  void init();

  float getZ();
  float getAngle();
  void resetAngle();

  private:
  int16_t gz;
  MPU6050 mpu;
  float z;
  float gzOff;
  unsigned long lastTime;
  float angleZ;
  float startAngle = 0;
};