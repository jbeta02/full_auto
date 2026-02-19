#include "Imu.h"

//init and offset to base 130 (no movement)
void Imu::init() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  long sum = 0;
  for (int i = 0; i < 100; i++) {
    int16_t gz = mpu.getRotationZ();
    sum += gz;
    delay(10);
  }

  gzOff = sum / 100.0;
  lastTime = millis();
}

//Meant to be used in loop, reads gyro Z (yaw) angular velocity
float Imu::getZ(){
  gz = mpu.getRotationZ();
  z = (gz - gzOff) / 131.0;
  if(abs(z) < 0.45){
    z = 0;
  }
  return z; //negative values are clockwise
}

float Imu::getAngle(){
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // Convert ms to seconds

  angleZ += getZ() * deltaTime;
  lastTime = currentTime;

  return angleZ - startAngle;
}

void Imu::resetAngle() {
  startAngle = angleZ;
}