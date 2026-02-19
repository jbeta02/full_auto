#include "Encoder.h"

// sensor data sheet https://look.ams-osram.com/m/7059eac7531a86fd/original/AS5600-DS000365.pdf

void Encoder::init() {
    Wire.begin(); //start i2C (establish arduino as conroller/master device)
    // set clock for I2C
    // Wire.setClock(800000L); don't need to set, default will be used

    // SCL -> Digital 21
    // SDA -> Digital 20
}

// subtract curr angle to reset
void Encoder::resetAngle() {
    globalAngleStart = globalAngle;
}

float Encoder::getAngle() {
    
    // get angle in degrees (still has wrap around)
    float angle = getWrapAngle();

    // track "quadrant" (only need 3) to add or subtract 360 to prevent wrap around
    if (angle >= 0 && angle < 90) {
        quadrant = 1;
    }

    else if (angle >= 90 && angle < 270) {
        quadrant = 2;
    }

    else if (angle >= 270 && angle < 360) {
        quadrant = 3;
    }

    // react to transitions
    if (quadrant != lastQuadrant) {

        // check for 1-3 or 3-1 transitions
        if (lastQuadrant == 3 && quadrant == 1) {
            angleSum += 360;
        }

        else if (lastQuadrant == 1 && quadrant == 3) {
            angleSum -= 360;
        }

        // update lastQuadrant
        lastQuadrant = quadrant;
    }

    globalAngle = angleSum + angle;

    // Serial.begin(2000000);
    // Serial.println(globalAngle / (97.03/33.81));

    return globalAngle - globalAngleStart;
}

float Encoder::getWrapAngle() {
    // convert angle in bits to degrees then track based on set origin
    // angle will wrape from 0 to 360
    if (!readStartAngle) {
        startAngle = readRawAngle() * 360.0f / 4096.0f;
        readStartAngle = true;
    }

    wrapAngle  = readRawAngle() * 360.0f / 4096.0f - startAngle; // convert to degrees: x bits * (360 degrees per rev / 2^12 bits per rev)

    // sometimes angle changes to negative during spin, subtracting on negitive keeps cycle consistent
    if (wrapAngle < 0) {
        wrapAngle = 360 + wrapAngle;
    }

    // Serial.begin(2000000);
    // Serial.println(wrapAngle);

    return wrapAngle;
}

int Encoder::isMagnetDetected(){
    Wire.beginTransmission(SENSOR_ADDRESS);
    Wire.write(0x0B); // 0x0C refers to register corresponding to STATUS

    // Serial.begin(2000000);

    // wait for proper return value then read
    // while(Wire.available() == 0); // returns number of bytes avialable (1)
    if (Wire.endTransmission() != 0) {
        //Serial.println("Error: AS5600 not responding!");
        return -1;
    }
    Wire.requestFrom(SENSOR_ADDRESS, 1); // requst 1 byte data from senosr

    int magnetStatus = Wire.read();

    Serial.begin(2000000);
    Serial.print("MD status: ");
    Serial.println(magnetStatus, BIN);
    Serial.println(magnetStatus);

    //MH: Too strong magnet - 100111 - DEC: 39 
    //ML: Too weak magnet - 10111 - DEC: 23     
    //MD: OK magnet - 110111 - DEC: 55
    // if (magnetStatus != 55) {
    //   Wire.begin(); //start i2C (establish arduino as conroller/master device)
    //   // set clock for I2C
    //   Wire.setClock(800000L);
    // }

    return magnetStatus; // change back to bool
}

int Encoder::readRawAngle() {
    //Serial.begin(2000000);
   // work on 0x0C RAW ANGLE (11:8)
    Wire.beginTransmission(SENSOR_ADDRESS);
    // send bytes in a queue, tell what data we want to read from sensor
    Wire.write(0x0C); // 0x0C refers to register corresponding to RAW ANGLE(11:8)

    if (Wire.endTransmission() != 0) {
        //Serial.println("Error: AS5600 not responding!");
        return -1; // helps with error checking
    }
    // requst 2 bytes data from senosr since raw angle is 12-bits and spans 2 registers
    Wire.requestFrom(SENSOR_ADDRESS, 2); // requst 2 bytes data from senosr (get msb and lsb)
    // request 2 bytes bc will be getting data from 

    if (Wire.available() < 2) { 
        //Serial.println("Error: No data received!");
        return -1; // helps with error checking
    }

    uint8_t msb = Wire.read(); // most significant byte. Read MSB (0x0C)
    uint8_t lsb = Wire.read(); // least significant byte. Read LSB (0x0D)

    // shift msb by 8 bits to make room for lsb, use or to combine shifted msb and lsb
    // and with 0x0FFF (00001111 11111111) to save only lower 12 bit values
    rawAngle = ((msb << 8) | lsb) & 0x0FFF;

    // 8 bit shift and or example for msb and lsb:
    // msb shift from 00000000 00001010 to 00001010 00000000
    // if msb is or'ed with lsb, lsb example value 00000000 01011100
    // then result will be 00001010 01011100 (msb and lsb)

    return rawAngle;
}